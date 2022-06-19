#include "Package.h"

#include <fstream>

#include "lz4.h"

namespace
{
	constexpr auto dst_size = LZ4_COMPRESSBOUND( 1024 );

	struct file_info
	{
		XE::uint64 count = 0;
		XE::uint64 version = 0;
	};

	struct item_info
	{
		XE::String uuid;
		XE::String name;
		XE::uint64 r_size = 0;
		XE::uint64 c_size = 0;
		XE::uint64 address = 0;
	};

	XE::uint64 read_str( const XE::uint8 * buffer, XE::String & str )
	{
		XE::uint64 sz = 0;
		
		buffer += XE::ReadLittleEndian( buffer, sz ); str.resize( sz );

		std::copy( buffer, buffer + sz, str.data() );

		return sz + sizeof( XE::uint64 );
	}
	XE::uint64 write_str( XE::uint8 * buffer, const XE::String & str )
	{
		buffer += XE::WriteLittleEndian( buffer, str.size() );
		std::copy( str.c_str(), str.c_str() + str.size(), buffer );
		return str.size() + sizeof( XE::uint64 );
	}
}

struct XE::Package::Private
{
	bool _Open = false;
	XE::MMapFile _MMap;

	XE::uint64 _Hash = 0;
	std::filesystem::path _Path;

	file_info _File;
	XE::Array< item_info > _Items;
	XE::UnorderedMap< XE::String, XE::uint64 > _ItemMap;
};

XE::Package::Package()
	:_p( XE::New< Private >() )
{

}

XE::Package::~Package()
{
	Close();

	XE::Delete( _p );
}

XE::uint64 XE::Package::GetVersion() const
{
	return _p->_File.version;
}

void XE::Package::SetVersion( XE::uint64 val )
{
	_p->_File.version = val;

	XE::WriteLittleEndian( _p->_MMap.GetAddress(), val );
}

bool XE::Package::ExistsEntry( const XE::UUID & val ) const
{
	return _p->_ItemMap.find( XE::ToString( val ) ) != _p->_ItemMap.end();
}

bool XE::Package::ExistsEntry( const XE::String & val ) const
{
	return _p->_ItemMap.find( val ) != _p->_ItemMap.end();
}

void XE::Package::GetEntryUUIDs( XE::Array< XE::UUID > & val ) const
{
	for ( const auto & it : _p->_ItemMap )
	{
		XE::UUID id; id.FromString( _p->_Items[it.second].uuid );
		val.emplace_back( std::move( id ) );
	}
}

void XE::Package::GetEntryNames( XE::Array< XE::String > & val ) const
{
	for ( const auto & it : _p->_ItemMap )
	{
		val.push_back( _p->_Items[it.second].name );
	}
}
/*
bool XE::Package::AddEntry( const XE::UUID & uuid, const XE::String & name, XE::MemoryView data )
{
	item_info item;

	if ( _p->_MMap.IsOpen() )
	{
		_p->_MMap.Close();
	}

	std::ofstream stream( _p->_Path, std::ios::binary | std::ios::app | std::ios::out );
	if ( stream.is_open() )
	{
		item.uuid = uuid.ToString();
		item.name = name;
		item.r_size = data.size();
		item.address = std::filesystem::file_size( _p->_Path );

		std::array< XE::int8, dst_size> dst;
		{
			XE::uint8 * p = reinterpret_cast<XE::uint8 *>( dst.data() );

			p += write_str( p, item.uuid );
			p += write_str( p, item.name );
			p += XE::WriteLittleEndian( p, item.r_size );
			p += XE::WriteLittleEndian( p, item.c_size );

			XE::uint64 sz = reinterpret_cast<XE::int8 *>( p ) - dst.data();
			if ( _p->_Hash != 0 )
			{
				std::transform( dst.data(), dst.data() + sz, dst.data(), [this]( XE::int8 val ) { return val ^ _p->_Hash; } );
			}
			
			XE::WriteLittleEndian( &sz, sz );
			stream.write( reinterpret_cast<const char *>( &sz ), sizeof( sz ) );

			stream.write( dst.data(), sz );
		}

		LZ4_stream_t lz4;
		{
			auto src = data.data();
			auto src_size = std::min( XE::uint64( 1024 ), data.size() );

			while ( src < data.data() + data.size() )
			{
				auto bytes = LZ4_compress_fast_continue( &lz4, src, dst.data(), src_size, dst_size, 0 );

				stream.write( dst.data(), bytes );

				src += src_size;
				src_size = std::min( XE::uint64( 1024 ), XE::uint64( ( data.data() + data.size() ) - src ) );

				item.c_size += bytes;
			}
		}

		stream.close();
	}

	if ( _p->_MMap.Open( _p->_Path ) )
	{
		_p->_File.count++;
		_p->_ItemMap.insert( { item.name, _p->_Items.size() } );
		_p->_ItemMap.insert( { item.uuid, _p->_Items.size() } );

		auto address = _p->_MMap.GetAddress();

		XE::WriteLittleEndian( address + sizeof( _p->_File.version ), _p->_File.count );

		address += item.address - sizeof( item.c_size );
		XE::WriteLittleEndian( address, item.c_size );

		_p->_Items.emplace_back( std::move( item ) );
	}
}
*/
bool XE::Package::GetEntry( const XE::UUID & uuid, std::ostream & stream ) const
{
	return GetEntry( uuid.ToString(), stream );
}

bool XE::Package::GetEntry( const XE::String & name, std::ostream & stream ) const
{
	auto it = _p->_ItemMap.find( name );
	if ( it != _p->_ItemMap.end() )
	{
		item_info & item = _p->_Items[it->second];
		auto address = reinterpret_cast<XE::int8 *>( _p->_MMap.GetAddress() + item.address );

		std::array<XE::int8, dst_size> dst;
		LZ4_streamDecode_t lz4;
		{
			XE::uint64 sz = 0;

			while ( sz < item.r_size )
			{
				auto bytes = LZ4_decompress_safe_continue( &lz4, address, dst.data(), item.c_size, dst_size );

				stream.write( dst.data(), bytes );

				sz += bytes;
			}

			return sz == item.r_size;
		}
	}
	return false;
}

bool XE::Package::Open( const std::filesystem::path & path, const XE::String & password /*= "" */ )
{
	Close();

	if ( _p->_MMap.Open( path ) )
	{
		_p->_Path = path;
		_p->_Hash = password.empty() ? 0 : XE::Hash( password );

		auto address = _p->_MMap.GetAddress();
		auto start = address;

		_p->_ItemMap.clear();
		address += XE::ReadLittleEndian( address, _p->_File.version );
		address += XE::ReadLittleEndian( address, _p->_File.count );

		_p->_Items.resize( _p->_File.count );

		std::array< XE::uint8, dst_size> dst;
		for ( size_t i = 0; i < _p->_File.count; i++ )
		{
			item_info item;
			{
				XE::uint64 sz = 0;
				{
					address += XE::ReadLittleEndian( address, sz );
					std::copy( address, address + sz, dst.data() );
					address += sz;
				}

				if ( _p->_Hash != 0 )
				{
					std::transform( dst.data(), dst.data() + sz, dst.data(), [this]( XE::uint8 val ) { return val ^ _p->_Hash; } );
				}

				auto p = dst.data();
				{
					p += read_str( p, item.uuid );
					p += read_str( p, item.name );

					p += XE::ReadLittleEndian( p, item.r_size );
					p += XE::ReadLittleEndian( p, item.c_size );
				}

				item.address = address - start;
			}
			address += item.c_size;

			_p->_Items[i] = std::move( item );
			_p->_ItemMap.insert( { item.name, i } );
			_p->_ItemMap.insert( { item.uuid, i } );
		}

		_p->_Open = true;
	}

	return _p->_Open;
}

void XE::Package::Close()
{
	if ( IsOpen() )
	{
		_p->_MMap.Close();
		_p->_ItemMap.clear();
		_p->_File.count = 0;
		_p->_File.version = 0;
	}
}

bool XE::Package::IsOpen() const
{
	return _p->_Open;
}

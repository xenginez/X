#undef OS_LINUX
#undef _HAS_OLD_IOSTREAMS_MEMBERS

#if COMPILER == COMPILER_MSVC
#	pragma warning(disable : 4267)
#	pragma warning(disable : 4834)
#	pragma warning(disable : 5033)
#elif COMPILER == COMPILER_CLANG
#	pragma clang diagnostic ignored"-Winconsistent-missing-override"
#else
#endif

#define _CRT_SECURE_NO_WARNINGS 1
#define _HAS_OLD_IOSTREAMS_MEMBERS 1
#include <bundle/bundle.cpp>
#include <bundle/bundle.h>
#undef BYTE
#undef check

#include <fstream>

#include "Zipper.h"

#define ZIP_MAX_SIZE GBYTE(4)
#define SECTION_MAX_SIZE KBYTE(64)

namespace _XE
{
	struct SectionInfo
	{
		XE::uint64 Index = 0;		// zip index
		XE::uint64 Start = 0;		// start address
		XE::uint64 CompressSize;	// compress size
		XE::uint64 DecompressSize;	// decompress size
	};

	struct FileInfo
	{
		XE::uint32 Type = 0;				// file type
		XE::String Name;					// file name
		XE::uint64 CompressSize = 0;		// file compress size
		XE::uint64 DecompressSize = 0;		// file decompress size
		XE::Array< SectionInfo > Sections;	// section info array
	};

	struct ZipInfo
	{
		XE::uint64 Size = 0;							// zip count
		XE::UnorderedMap< XE::String, FileInfo > Files;	// zip file map
	};

	struct StreamInfo
	{
		XE::uint64 _Address = 0;
		XE::MMapFile _Reader;
		std::ofstream _Writer;
	};

	void Encryption( std::istream & in, std::ostream & out, const char * password )
	{
		if( password == nullptr || password[0] == 0 )
		{
			out << in.rdbuf();
		}
		else
		{
			XE::uint64 hash_code = XE::Hash( password );

			while( in.peek() != EOF )
			{
				XE::uint64 value = 0;
				auto size = in.readsome( reinterpret_cast< char * >( &value ), sizeof( XE::uint64 ) );

				if( size == sizeof( XE::uint64 ) )
				{
					value ^= hash_code;
				}

				out.write( reinterpret_cast< char * >( &value ), size );
			}
		}
	}

	void Decryption( std::istream & in, std::ostream & out, const char * password )
	{
		Encryption( in, out, password );
	}
}


struct XE::Zipper::Private
{
	_XE::ZipInfo _Zip;
	bool _IsOpen = false;
	XE::String _Password;
	std::filesystem::path _Path;
	XE::Array< _XE::StreamInfo > _Streams;
};

XE::Zipper::Zipper()
	:_p( XE::New< Private >() )
{

}

XE::Zipper::Zipper( const std::filesystem::path & path )
	:_p( XE::New< Private >() )
{
	Open( path );
}

XE::Zipper::~Zipper()
{
	Close();

	XE::Delete( _p );
}

bool XE::Zipper::ExtractEntry( const XE::String & name ) const
{
	return _p->_Zip.Files.find( name ) != _p->_Zip.Files.end();
}

void XE::Zipper::GetEntryNames( XE::Array< XE::String > & names ) const
{
	for( const auto & it : _p->_Zip.Files )
	{
		names.push_back( it.first );
	}
}

bool XE::Zipper::AddEntry( const XE::String & name, std::istream & stream, XE::ZipType type /*= XE::ZipType::LZ4*/ )
{
	auto it = _p->_Zip.Files.find( name );

	if( it == _p->_Zip.Files.end() )
	{
		_XE::FileInfo file;

		file.Type = static_cast< XE::uint32 >( type );

		file.Name = name;
		auto pos = stream.tellg();
		stream.seekg( 0, std::ios::end );
		file.DecompressSize = stream.tellg() - pos;
		stream.seekg( pos, std::ios::beg );

		if( file.DecompressSize > 0 )
		{
			auto section_count = ( ( file.DecompressSize ) / SECTION_MAX_SIZE ) + 1;
			XE::Array< char > in_buf( SECTION_MAX_SIZE );
			XE::Array< char > out_buf( SECTION_MAX_SIZE );

			for( XE::uint64 i = 0; i < section_count; ++i )
			{
				_XE::SectionInfo section;

				XE::Array< _XE::StreamInfo >::iterator it = _p->_Streams.begin();
				for( ; it != _p->_Streams.end(); ++it )
				{
					if ( ( it->_Address + SECTION_MAX_SIZE ) < ZIP_MAX_SIZE )
					{
						break;
					}
				}

				if( it == _p->_Streams.end() )
				{
					_XE::StreamInfo info;
					_p->_Streams.emplace_back( std::move( info ) );

					it = _p->_Streams.end() - 1;

					auto p = _p->_Path.parent_path() / ( _p->_Path.stem().string() + XE::ToString( _p->_Streams.size() - 1 ).c_str() + _p->_Path.extension().string() );

					it->_Address = 0;
					it->_Reader.Open( p );
					it->_Writer.open( p, std::ios::out | std::ios::app | std::ios::binary );

					_p->_Zip.Size = _p->_Streams.size();
				}

				section.Index = std::distance( _p->_Streams.begin(), it );
				section.Start = it->_Address;

				size_t in_size = stream.readsome( in_buf.data(), SECTION_MAX_SIZE );
				size_t out_size = out_buf.size();

				if( bundle::pack( file.Type, in_buf.data(), in_size, out_buf.data(), out_size ) )
				{
					it->_Writer.seekp( it->_Address, std::ios::beg );
					it->_Writer.write( out_buf.data(), out_size );
					it->_Writer.flush();

					it->_Address = it->_Writer.tellp();

					it->_Reader.Reset();

					section.CompressSize = out_size;
					section.DecompressSize = in_size;

					file.Sections.push_back( section );

					file.CompressSize += section.CompressSize;
				}
			}

			_p->_Zip.Files.insert( { name, std::move( file ) } );

			return true;
		}
	}

	return false;
}

bool XE::Zipper::GetEntry( const XE::String & name, std::ostream & stream )
{
	auto it = _p->_Zip.Files.find( name );

	if( it != _p->_Zip.Files.end() )
	{
		XE::Array< char > in_buf( SECTION_MAX_SIZE );
		XE::Array< char > out_buf( SECTION_MAX_SIZE );

		const _XE::FileInfo & file = it->second;
		for( const auto section : file.Sections )
		{
			_p->_Streams[section.Index]._Writer.seekp( section.Start, std::ios::beg );

			size_t in_size = _p->_Streams[section.Index]._Reader.Read( section.Start, reinterpret_cast< XE::uint8 * >( in_buf.data() ), section.CompressSize );
			size_t out_size = out_buf.size();
			if( bundle::unpack( file.Type, in_buf.data(), in_size, out_buf.data(), out_size ) )
			{
				stream.write( out_buf.data(), section.DecompressSize );
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool XE::Zipper::GetEntryStream( const XE::String & name, std::ostream & stream, XE::uint64 offset, XE::uint64 size )
{
	auto it = _p->_Zip.Files.find( name );

	if( it != _p->_Zip.Files.end() )
	{
		XE::Array< char > in_buf( SECTION_MAX_SIZE );
		XE::Array< char > out_buf( SECTION_MAX_SIZE );

		const _XE::FileInfo & file = it->second;
		for( const auto section : file.Sections )
		{
			if( offset >= section.DecompressSize )
			{
				offset -= section.DecompressSize;
				continue;
			}

			size_t in_size = _p->_Streams[section.Index]._Reader.Read( section.Start, reinterpret_cast< XE::uint8 * >( in_buf.data() ), section.CompressSize );
			size_t out_size = out_buf.size();
			if( bundle::unpack( file.Type, in_buf.data(), in_size, out_buf.data(), out_size ) )
			{
				out_size = std::min( size, out_size - offset );
				stream.write( out_buf.data() + offset, out_size );

				offset = 0;
				size -= out_size;

				if( size == 0 )
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	return false;
}

bool XE::Zipper::Open( const std::filesystem::path & path, const XE::String & password /*= "" */ )
{
	Close();

	if( std::filesystem::exists( path ) )
	{
		std::ifstream file_stream( path, std::ios::binary );
		if( file_stream.is_open() )
		{
			XE::MemoryStream mem_stream;
			_XE::Decryption( file_stream, mem_stream, password.c_str() );
			if( mem_stream.view().size() != 0 )
			{
				mem_stream.read( reinterpret_cast< char * >( &_p->_Zip.Size ), sizeof( XE::uint64 ) );
				for( XE::uint64 i = 0; i < _p->_Zip.Size; ++i )
				{
					_XE::FileInfo file;
					mem_stream.read( reinterpret_cast< char * >( &file.Type ), sizeof( XE::uint32 ) );
					XE::uint64 size = 0;
					mem_stream.read( reinterpret_cast< char * >( &size ), sizeof( XE::uint64 ) );
					std::string name; name.resize( size );
					mem_stream.read( name.data(), size );
					file.Name = name;
					mem_stream.read( reinterpret_cast< char * >( &file.CompressSize ), sizeof( XE::uint64 ) );
					mem_stream.read( reinterpret_cast< char * >( &file.DecompressSize ), sizeof( XE::uint64 ) );

					mem_stream.read( reinterpret_cast< char * >( &size ), sizeof( XE::uint64 ) );
					for( XE::uint64 j = 0; j < size; ++j )
					{
						_XE::SectionInfo section;

						mem_stream.read( reinterpret_cast< char * >( &section.Index ), sizeof( XE::uint64 ) );
						mem_stream.read( reinterpret_cast< char * >( &section.Start ), sizeof( XE::uint64 ) );
						mem_stream.read( reinterpret_cast< char * >( &section.CompressSize ), sizeof( XE::uint64 ) );
						mem_stream.read( reinterpret_cast< char * >( &section.DecompressSize ), sizeof( XE::uint64 ) );

						file.Sections.push_back( std::move( section ) );
					}

					_p->_Zip.Files.insert( { file.Name, std::move( file ) } );
				}
			}

			_p->_Streams.resize( _p->_Zip.Size );
			for( size_t i = 0; i < _p->_Streams.size(); i++ )
			{
				auto p = path.parent_path() / ( path.stem().string() + XE::ToString( i ).c_str() + path.extension().string() );

				_p->_Streams[i]._Writer.open( p, std::ios::in | std::ios::out | std::ios::app | std::ios::binary );
				_p->_Streams[i]._Reader.Open( p );

				if( _p->_Streams[i]._Writer.is_open() == false || _p->_Streams[i]._Reader.IsOpen() == false )
				{
					Close();
					return false;
				}

				_p->_Streams[i]._Writer.seekp( 0, std::ios::end );
				_p->_Streams[i]._Address = _p->_Streams[i]._Writer.tellp();
			}
		}
	}
	else
	{
		std::filesystem::create_directory( path.parent_path() );
		std::ofstream ofs( path, std::ios::out );
		ofs.flush();
		ofs.close();
	}

	_p->_Path = path;
	_p->_IsOpen = true;
	_p->_Password = password;

	return true;
}

void XE::Zipper::Close()
{
	if( IsOpen() )
	{
		std::ofstream file_stream( _p->_Path, std::ios::out | std::ios::ate | std::ios::binary );
		if( file_stream.is_open() )
		{
			XE::MemoryStream mem_stream;

			mem_stream.write( reinterpret_cast< const char * >( &_p->_Zip.Size ), sizeof( XE::uint64 ) );
			for( auto it : _p->_Zip.Files )
			{
				const _XE::FileInfo & file = it.second;
				mem_stream.write( reinterpret_cast< const char * >( &file.Type ), sizeof( XE::uint32 ) );
				XE::uint64 size = file.Name.size();
				mem_stream.write( reinterpret_cast< const char * >( &size ), sizeof( XE::uint64 ) );
				mem_stream.write( file.Name.c_str(), size );
				mem_stream.write( reinterpret_cast< const char * >( &file.CompressSize ), sizeof( XE::uint64 ) );
				mem_stream.write( reinterpret_cast< const char * >( &file.DecompressSize ), sizeof( XE::uint64 ) );

				size = file.Sections.size();
				mem_stream.write( reinterpret_cast< const char * >( &size ), sizeof( XE::uint64 ) );
				for( const auto & section : file.Sections )
				{
					mem_stream.write( reinterpret_cast< const char * >( &section.Index ), sizeof( XE::uint64 ) );
					mem_stream.write( reinterpret_cast< const char * >( &section.Start ), sizeof( XE::uint64 ) );
					mem_stream.write( reinterpret_cast< const char * >( &section.CompressSize ), sizeof( XE::uint64 ) );
					mem_stream.write( reinterpret_cast< const char * >( &section.DecompressSize ), sizeof( XE::uint64 ) );
				}
			}

			_XE::Encryption( mem_stream, file_stream, _p->_Password.c_str() );
		}

		_p->_Zip.Size = 0;
		_p->_Zip.Files.clear();

		_p->_Path.clear();
		_p->_Streams.clear();
		_p->_Password.clear();

		_p->_IsOpen = false;
	}
}

bool XE::Zipper::IsOpen() const
{
	return _p->_IsOpen;
}

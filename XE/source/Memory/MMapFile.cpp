#include "MMapFile.h"

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
#include <windows.h>
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "AllocatorProxy.hpp"

struct XE::MMapFile::Private
{
#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
	void * _Mapping = nullptr;
#endif
	void * _File = nullptr;
	XE::uint64 _Size = 0;
	XE::uint8 * _Addr = nullptr;
	std::filesystem::path _Path;
};

XE::MMapFile::MMapFile()
	:_p( XE::New< Private >() )
{

}

XE::MMapFile::MMapFile( XE::uint64 size )
	: _p( XE::New< Private >() )
{
	Open( size );
}

XE::MMapFile::MMapFile( MMapFile && val )
	: _p( XE::New< Private >() )
{
	Swap( val );
}

XE::MMapFile::MMapFile( const std::filesystem::path & file )
	: _p( XE::New< Private >() )
{
	Open( file );
}

XE::MMapFile::~MMapFile()
{
	Close();

	XE::Delete( _p );
}

XE::uint8 & XE::MMapFile::operator[]( XE::uint64 val )
{
	return _p->_Addr[val];
}

XE::uint8 XE::MMapFile::operator[]( XE::uint64 val ) const
{
	return _p->_Addr[val];
}

void XE::MMapFile::Swap( MMapFile & val )
{
	std::swap( _p, val._p );
}

bool XE::MMapFile::IsOpen() const
{
	return _p->_Size != 0;
}

XE::uint64 XE::MMapFile::GetSize() const
{
	return _p->_Size;
}

XE::uint8 * XE::MMapFile::GetAddress() const
{
	return _p->_Addr;
}

const std::filesystem::path & XE::MMapFile::GetFilePath() const
{
	return _p->_Path;
}

void XE::MMapFile::Write( XE::uint64 index, const XE::uint8 * data, XE::uint64 size )
{
	std::copy( data, data + size, _p->_Addr + index );
}

XE::uint64 XE::MMapFile::Read( XE::uint64 index, XE::uint8 * data, XE::uint64 size ) const
{
	size = std::min< XE::uint64 >( size, _p->_Size - index );
	std::copy( _p->_Addr + index, _p->_Addr + index + size, data );
	return size;
}

void XE::MMapFile::Reset()
{
	if( IsOpen() )
	{
		auto path = _p->_Path;
		Close();
		Open( path );
	}
}

#if PLATFORM_OS & ( OS_WINDOWS | OS_XBOX )
void XE::MMapFile::Open( XE::uint64 size )
{
	Close();

	HANDLE mapping_ = ::CreateFileMappingA( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size & 0xFFFFFFFF00000000, size & 0x00000000FFFFFFFF, NULL );

	if( mapping_ != INVALID_HANDLE_VALUE && mapping_ != 0 )
	{
		_p->_Size = size;

		_p->_Addr = static_cast< XE::uint8 * >( ::MapViewOfFile( mapping_, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 ) );

		_p->_File = nullptr;

		_p->_Mapping = mapping_;

		_p->_Path = "";
	}
}

void XE::MMapFile::Open( const std::filesystem::path & path )
{
	Close();

	auto file_ = ::CreateFileA( path.string().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if( file_ != INVALID_HANDLE_VALUE )
	{
		auto mapping_ = ::CreateFileMappingA( file_, NULL, PAGE_READWRITE, 0, 0, NULL );
		if( mapping_ != INVALID_HANDLE_VALUE && mapping_ != 0 )
		{
			_p->_Size = ::GetFileSize( file_, NULL );

			_p->_Addr = static_cast< XE::uint8 * >( ::MapViewOfFile( mapping_, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 ) );

			_p->_File = file_;
			
			_p->_Mapping = mapping_;

			_p->_Path = path;

			return;
		}
		else
		{
			::CloseHandle( file_ );
		}
	}
}

void XE::MMapFile::Close()
{
	if( IsOpen() )
	{
		if( _p->_Addr )
		{
			::UnmapViewOfFile( _p->_Addr );
			_p->_Addr = nullptr;
		}

		if( _p->_Mapping )
		{
			::CloseHandle( _p->_Mapping );
			_p->_Mapping = nullptr;
		}

		if( _p->_File )
		{
			::CloseHandle( _p->_File );
			_p->_File = nullptr;
		}

		_p->_Size = 0;

		_p->_Path.clear();
	}
}
#else
void XE::MMapFile::Open( XE::uint64 size )
{
	Close();

	auto file_ = open( "/dev/zero", O_RDONLY );
	if( file_ != -1 )
	{
		struct stat sb;
		if( fstat( file_, &sb ) != -1 )
		{
			_p->_Size = size;

			_p->_Addr = static_cast< XE::uint8 * >( mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file_, 0 ) );

			_p->_Path = "";

			_p->_File = reinterpret_cast< void * >( file_ );

			return;
		}
		else
		{
			close( file_ );
		}
	}
}

void XE::MMapFile::Open( const std::filesystem::path & path )
{
	Close();

	auto file_ = open( path.string().c_str(), O_RDONLY );
	if( file_ != -1 )
	{
		struct stat sb;
		if( fstat( file_, &sb ) != -1 )
		{
			_p->_Size = sb.st_size;

			_p->_Addr = static_cast< XE::uint8 * >( mmap( NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_, 0 ) );

			_p->_Path = path;

			_p->_File = reinterpret_cast< void * >( file_ );

			return;
		}
		else
		{
			close( file_ );
		}
	}
}

void XE::MMapFile::Close()
{
	if( IsOpen() )
	{
		if( _p->_Addr != MAP_FAILED )
		{
			munmap( _p->_Addr, _p->_Size );
			_p->_Addr = nullptr;
		}

		int file_ reinterpret_cast< int >( _p->_File );
		if( file_ != -1 || file_ != 0 )
		{
			close( file_ );
			_p->_File = nullptr;
		}

		_p->_Size = 0;

		_p->_Path.clear();
	}
}
#endif

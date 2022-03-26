#include "Library.h"

struct XE::Library::Private
{
	XE::Set< std::filesystem::path > _Env;
};

XE::Library::Library()
	:_p( XE::New< Private >() )
{

}

XE::Library::~Library()
{
	XE::Delete( _p );
}

XE::Library * XE::Library::Instance()
{
	static XE::Library instance;
	return &instance;
}

#define _p XE::Library::Instance()->_p

void XE::Library::RegisterEnvPath( const std::filesystem::path & path )
{
	_p->_Env.insert( path );
}


#if PLATFORM_OS & OS_WINDOWS

#include <Windows.h>

XE::LibraryHandle XE::Library::Open( const String & val )
{
	HMODULE module = nullptr;

#ifdef XE_PROFILING
	if( module == nullptr )
	{
		module = ::LoadLibrary( ( val + "_profiling" ).c_str() );
	}
#endif

	if( module == nullptr )
	{
		module = ::LoadLibrary( val.c_str() );
	}

#ifdef XE_PROFILING
	if( module == nullptr )
	{
		module = ::LoadLibrary( ( val + "_profiling" + DLL_EXT_NAME ).c_str() );
	}
#endif

	if( module == nullptr )
	{
		module = ::LoadLibrary( ( val + DLL_EXT_NAME ).c_str() );
	}

	if( module == nullptr )
	{
		for( const auto & p : _p->_Env )
		{
#ifdef XE_PROFILING
			if( module == nullptr )
			{
				module = ::LoadLibrary( ( p / ( val + "_profiling" ).c_str() ).string().c_str() );
			}
#endif

			if( module == nullptr )
			{
				module = ::LoadLibrary( ( p / val.c_str() ).string().c_str() );
			}

#ifdef XE_PROFILING
			if( module == nullptr )
			{
				module = ::LoadLibrary( ( p / ( val + "_profiling" + DLL_EXT_NAME ).c_str() ).string().c_str() );
			}
#endif

			if( module == nullptr )
			{
				module = ::LoadLibrary( ( p / ( val + DLL_EXT_NAME ).c_str() ).string().c_str() );
			}

			if( module != nullptr )
			{
				break;
			}
		}
	}

	if( module != nullptr )
	{
		return XE::HandleCast< XE::Library >( ( XE::uint64 )( module ) );
	}

	return LibraryHandle::Invalid;
}

void * XE::Library::Symbol( XE::LibraryHandle handle, const String & name )
{
	return ::GetProcAddress( (HMODULE )( handle.GetValue() ), name.c_str() );
}

bool XE::Library::Close( XE::LibraryHandle handle )
{
	return ::FreeLibrary( (HMODULE )( handle.GetValue() ) );
}
#else

#include <dlfcn.h>

XE::LibraryHandle XE::Library::Open( const String & val )
{
	void * module = nullptr;

#ifdef XE_PROFILING
	if( module == nullptr )
	{
		module = ::dlopen( ( val + "_profiling" ).c_str(), RTLD_NOW );
	}
#endif

	if( module == nullptr )
	{
		module = ::dlopen( val.c_str(), RTLD_NOW );
	}

#ifdef XE_PROFILING
	if( module == nullptr )
	{
		module = ::dlopen( ( val + "_profiling" + DLL_EXT_NAME ).c_str(), RTLD_NOW );
	}
#endif

	if( module == nullptr )
	{
		module = ::dlopen( ( val + DLL_EXT_NAME ).c_str(), RTLD_NOW );
	}

	if( module == nullptr )
	{
		for( const auto & p : _p->_Env )
		{
#ifdef XE_PROFILING
			if( module == nullptr )
			{
				module = ::dlopen( ( p / ( val + "_profiling" ).c_str() ).string().c_str(), RTLD_NOW );
			}
#endif

			if( module == nullptr )
			{
				module = ::dlopen( ( p / val.c_str() ).string().c_str(), RTLD_NOW );
			}

#ifdef XE_PROFILING
			if( module == nullptr )
			{
				module = ::dlopen( ( p / ( val + "_profiling" + DLL_EXT_NAME ).c_str() ).string().c_str(), RTLD_NOW );
			}
#endif

			if( module == nullptr )
			{
				module = ::dlopen( ( p / ( val + DLL_EXT_NAME ).c_str() ).string().c_str(), RTLD_NOW );
			}

			if( module != nullptr )
			{
				break;
			}
		}
	}

	if( module != nullptr )
	{
		return XE::HandleCast< XE::Library >( ( XE::uint64 )( module ) );
	}

	return LibraryHandle::Invalid;
}

void * XE::Library::Symbol( XE::LibraryHandle handle, const String & name )
{
	return ::dlsym( reinterpret_cast< void * >( handle.GetValue() ), val.c_str() );
}

bool XE::Library::Close( XE::LibraryHandle handle )
{
	return static_cast< bool >( ::dlclose( reinterpret_cast< void * >( handle.GetValue() ) ) );
}
#endif

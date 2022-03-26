/*!
 * \file   Library.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/17
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __LIBRARY_H__752AAED6_001C_4D36_A353_57FC8E154A2A
#define __LIBRARY_H__752AAED6_001C_4D36_A353_57FC8E154A2A

#include "Handle.hpp"

BEG_XE_NAMESPACE

DECL_HANDLE( Library );

class XE_API Library : public XE::NonCopyable
{
private:
	struct Private;

private:
	Library();

	~Library();

	static Library * Instance();

public:
	static LibraryHandle Open( const String& name );

	template< typename T >
	static T SymbolT( XE::LibraryHandle handle, const String& name )
	{
		return reinterpret_cast< T >( Symbol( handle, name ) );
	}

	static void * Symbol( XE::LibraryHandle handle, const String& name );

	static bool Close( XE::LibraryHandle handle );

	static void RegisterEnvPath( const std::filesystem::path & path );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __LIBRARY_H__752AAED6_001C_4D36_A353_57FC8E154A2A

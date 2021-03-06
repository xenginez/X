/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __TYPE_H__69A9E04E_44A6_4DEE_B145_845B7E13E31C
#define __TYPE_H__69A9E04E_44A6_4DEE_B145_845B7E13E31C

#include "Base/String.h"
#include "Base/Exception.h"
#include "Memory/AllocatorProxy.hpp"

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_PTR( MetaInfo );
DECL_PTR( MetaType );
DECL_PTR( MetaEnum );
DECL_PTR( MetaClass );
DECL_PTR( MetaModule );
DECL_PTR( MetaMethod );
DECL_PTR( MetaProperty );
DECL_PTR( MetaFunction );
DECL_PTR( MetaVariable );
DECL_PTR( MetaAttribute );

DECL_PTR( Object );

enum class MetaInfoType
{
	ENUM,
	CLASS,
	MODULE,
	METHOD,
	PROPERTY,
	FUNCTION,
	VARIABLE,
};

enum class ArchiveType
{
	LOAD,
	SAVE,
};

class XE_API MetaException : public XE::RuntimeException
{
public:
	MetaException( MetaInfoCPtr Meta, const XE::String& Msg );

	~MetaException();

public:
	virtual char const* What() const;

public:
	MetaInfoCPtr GetMetaInfo() const;

private:
	String _Msg;
	MetaInfoCPtr _Meta;
};

template < class T > class ActiveSingleton
{
public:
	static ActiveSingleton< T >& Register()
	{
		static ActiveSingleton< T > t;

		Use( _Reference );

		return t;
	}

private:
	static void Use( T * )
	{
	}

	static void Use( ActiveSingleton< T > const & )
	{
		T Collector;
		Use( &Collector );
	}

private:
	static ActiveSingleton< T > & _Reference;
};
template< class T > ActiveSingleton< T > & ActiveSingleton< T >::_Reference = ActiveSingleton< T >::Register();
template< typename T > struct MetaTypeCollector;

END_XE_NAMESPACE

#endif // __TYPE_H__69A9E04E_44A6_4DEE_B145_845B7E13E31C

/*!
 * \file	QMetaStaticCall.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef QMETASTATICCALL_HPP__65D949EB_A207_4767_BEDC_5829B392A8D8
#define QMETASTATICCALL_HPP__65D949EB_A207_4767_BEDC_5829B392A8D8

#include "Type.h"

BEG_XS_NAMESPACE

template< typename R, typename ... T > R QMetaStaticCall( const QMetaObject * meta, const char * name, T ... args )
{
	if constexpr ( std::is_void_v< R > )
	{
		void * pargs[sizeof...( T )] = { &args... };

		meta->static_metacall( QMetaObject::InvokeMetaMethod, meta->indexOfMethod( QMetaObject::normalizedSignature( name ) ) - meta->methodOffset(), pargs );
	}
	else
	{
		R result;

		void * pargs[sizeof...( T ) + 1] = { &result, &args... };

		meta->static_metacall( QMetaObject::InvokeMetaMethod, meta->indexOfMethod( QMetaObject::normalizedSignature( name ) ) - meta->methodOffset(), pargs );

		return result;
	}
}

END_XS_NAMESPACE

#endif//QMETASTATICCALL_HPP__65D949EB_A207_4767_BEDC_5829B392A8D8

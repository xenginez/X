/*!
 * \file   Singleton.h
 *
 * \author ZhengYuanQing
 * \date   2022/06/03
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef SINGLETON_H__D09FB825_C94A_4310_AFF3_52EDD109471F
#define SINGLETON_H__D09FB825_C94A_4310_AFF3_52EDD109471F

#include "Delegate.hpp"
#include "NonCopyable.hpp"

BEG_XE_NAMESPACE

class XE_API SingletonBase : public XE::NonCopyable
{
protected:
	static XE::Pair< void *, bool > NewSigleton( XE::uint64 hash, XE::uint64 size, const XE::Delegate< void( void * ) > & destructor );
};

template< typename T > class Singleton : public XE::SingletonBase
{
public:
	static T * Instance()
	{
		static constexpr char __xe__sig__[] = __FUNCTION__;

		static T * _Instance = nullptr;

		if( _Instance == nullptr )
		{
			auto pair = NewSigleton( XE::Hash( __xe__sig__ ), sizeof( T ), []( void * p ) { reinterpret_cast< T * >( p )->~T(); } );
			if( pair.second == false )
			{
				_Instance = new ( pair.first ) T();
			}
		}

		return _Instance;
	}
};

END_XE_NAMESPACE

#endif // SINGLETON_H__D09FB825_C94A_4310_AFF3_52EDD109471F

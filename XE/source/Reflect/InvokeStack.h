/*!
 * \file   InvokeStack.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __INVOKESTACK_H__81F7580A_298D_4FB3_96F1_FCB02A73A54B
#define __INVOKESTACK_H__81F7580A_298D_4FB3_96F1_FCB02A73A54B

#include "Variant.h"
#include "MetaTypeList.hpp"

BEG_XE_NAMESPACE

class XE_API InvokeStack
{
public:
	InvokeStack();

	InvokeStack( InvokeStack && val );

	InvokeStack( const InvokeStack & val );

	template< typename ... Types > InvokeStack( Types &&...args )
	{
		Push( args... );
	}

	~InvokeStack();

public:
	InvokeStack & operator = ( InvokeStack && val );

	InvokeStack & operator = ( const InvokeStack & val );

public:
	void Push();

	void Push( InvokeStack * val );

	void Push( InvokeStack & val );

	void Push( XE::Variant * val );

	void Push( XE::Variant & val );

	void Push( const InvokeStack * val );

	void Push( const InvokeStack & val );

	void Push( const XE::Variant * val );

	void Push( const XE::Variant & val );

	template< typename T > void Push( T * val )
	{
		_Stack.push_back( val );
	}

	template< typename T > void Push( T & val )
	{
		_Stack.push_back( XE::RawPointer( val ) );
	}

	template< typename T, std::enable_if_t< std::is_trivial_v< T >, int > = 0 > void Push( T val )
	{
		_Stack.push_back( val );
	}

	template<  typename T, typename ...Types > void Push( T val, Types... args )
	{
		Push( std::forward< T >( val ) );

		Push( args... );
	}

public:
	template< typename T > T Pop()
	{
	#if COMPILER == COMPILER_MSVC
		Variant var = _Stack.back();
		_Stack.pop_back();
	#else
		Variant var = _Stack.front();
		_Stack.pop_front();
	#endif

		return var.Value< T >();
	}

public:
	void Clear();

public:
	bool Empty() const;

	XE::uint64 GetSize() const;

	ParameterType GetTypes() const;

private:
	VariantArray _Stack;
};

END_XE_NAMESPACE

#endif // __INVOKESTACK_H__81F7580A_298D_4FB3_96F1_FCB02A73A54B

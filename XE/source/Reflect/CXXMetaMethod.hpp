/*!
 * \file   CXXMetaMethod.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CXXMETAMETHOD_HPP__6B59966B_7814_4B60_BB9C_FE4062B5744A
#define __CXXMETAMETHOD_HPP__6B59966B_7814_4B60_BB9C_FE4062B5744A

#include "MetaMethod.h"

#include "TypeID.hpp"

BEG_XE_NAMESPACE

template< typename ... ParameterType  > class CXXMetaMethod< void( * )( ParameterType... ) > : public XE::MetaMethod
{
public:
	using MethodType = void( * )( ParameterType... );

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), true, false, nullptr, XE::MakeParameterType< ParameterType... >(), Owner, Module ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		_Callback( params->Pop< ParameterType >()... );
		return XE::Variant();
	}

private:
	MethodType _Callback;
};

template< typename ResultType, typename ... ParameterType  > class CXXMetaMethod< ResultType( * )( ParameterType... ) > : public XE::MetaMethod
{
public:
	using MethodType = ResultType( * )( ParameterType... );

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), true, false, ::TypeID< typename TypeTraits< ResultType >::raw_t >::Get(), XE::MakeParameterType< ParameterType... >(), Owner ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		return _Callback( params->Pop< ParameterType >()... );
	}

private:
	MethodType _Callback;
};

template< typename ClassType, typename ... ParameterType  > class CXXMetaMethod< void( ClassType:: * )( ParameterType... ) > : public XE::MetaMethod
{
public:
	using MethodType = void( ClassType:: * )( ParameterType... );

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), false, false, nullptr, XE::MakeParameterType< ParameterType... >(), Owner, Module ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		( params->Pop< ClassType * >()->*_Callback )( params->Pop< ParameterType >()... );
		return XE::Variant();
	}

private:
	MethodType _Callback;
};

template< typename ClassType, typename ... ParameterType  > class CXXMetaMethod< void( ClassType:: * )( ParameterType... ) const > : public XE::MetaMethod
{
public:
	using MethodType = void( ClassType:: * )( ParameterType... ) const;

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), false, true, nullptr, XE::MakeParameterType< ParameterType... >(), Owner, Module ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		( params->Pop< ClassType * >()->*_Callback )( params->Pop< ParameterType >()... );
		return XE::Variant();
	}

private:
	MethodType _Callback;
};

template< typename ResultType, typename ClassType, typename ... ParameterType  > class CXXMetaMethod< ResultType( ClassType:: * )( ParameterType... ) > : public XE::MetaMethod
{
public:
	using MethodType = ResultType( ClassType:: * )( ParameterType... );

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), false, false, ::TypeID< typename TypeTraits< ResultType >::raw_t >::Get(), XE::MakeParameterType< ParameterType... >(), Owner, Module ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		return ( params->Pop< ClassType * >()->*_Callback )( params->Pop< ParameterType >()... );
	}

private:
	MethodType _Callback;
};

template< typename ResultType, typename ClassType, typename ... ParameterType  > class CXXMetaMethod< ResultType( ClassType:: * )( ParameterType... ) const > : public XE::MetaMethod
{
public:
	using MethodType = ResultType( ClassType:: * )( ParameterType... ) const;

public:
	CXXMetaMethod( const XE::String & Name, MethodType Callback, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaMethod( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), false, true, ::TypeID< typename TypeTraits< ResultType >::raw_t >::Get(), XE::MakeParameterType< ParameterType... >(), Owner, Module ), _Callback( Callback )
	{
	}

public:
	virtual XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		return ( params->Pop< ClassType * >()->*_Callback )( params->Pop< ParameterType >()... );
	}

private:
	MethodType _Callback;
};

END_XE_NAMESPACE

#define OVERLOAD(TYPE, FUNC) (TYPE)&FUNC

#endif // __CXXMETAMETHOD_HPP__6B59966B_7814_4B60_BB9C_FE4062B5744A

/*!
 * \file	CXXMetaFunction.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CXXMETAFUNCTION_HPP__5E5D51FA_6BF7_4EDD_89F5_F6E19C3882CD
#define CXXMETAFUNCTION_HPP__5E5D51FA_6BF7_4EDD_89F5_F6E19C3882CD

#include "MetaFunction.h"

BEG_XE_NAMESPACE

template< typename ... ParameterType  > class CXXMetaFunction< void( ParameterType... ) > : public XE::MetaFunction
{
public:
	using FunctionType = void( * )( ParameterType... );

public:
	CXXMetaFunction( const XE::String & Name, FunctionType Callback, XE::MetaModuleCPtr Module )
		: XE::MetaFunction( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), nullptr, XE::MakeParameterType< ParameterType... >(), Module ), _Callback( Callback )
	{
	}

public:
	XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		_Callback( params->Pop< ParameterType >()... );
		return XE::Variant();
	}

private:
	FunctionType _Callback;
};

template< typename ResultType, typename ... ParameterType  > class CXXMetaFunction< ResultType( ParameterType... ) > : public XE::MetaFunction
{
public:
	using FunctionType = ResultType( * )( ParameterType... );

public:
	CXXMetaFunction( const XE::String & Name, FunctionType Callback, XE::MetaModuleCPtr Module )
		: XE::MetaFunction( Name + XE::ToString( XE::MakeParameterType< ParameterType... >() ), ::TypeID< ResultType >::Get(), XE::MakeParameterType< ParameterType... >(), Module ), _Callback( Callback )
	{
	}

public:
	XE::Variant Invoke( XE::InvokeStack * params ) const override
	{
		return _Callback( params->Pop< ParameterType >()... );
	}

private:
	FunctionType _Callback;
};

END_XE_NAMESPACE

#define IMPLEMENT_META_FUNCTION_1( _FUNCTION ) \
namespace XE \
{ \
	class __xe_function_type_##_FUNCTION##__ {}; \
	template< > struct MetaTypeCollector< __xe_function_type_##_FUNCTION##__ > \
	{ \
		using FunctionType = decltype( _FUNCTION ); \
		MetaFunctionCollector() \
		{ \
			static constexpr char __xe__sig__[] = __FUNCTION__; \
			static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFunction< FunctionType > >( XE::Hash( __xe__sig__ ), #_FUNCTION, _FUNCTION, nullptr ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< __xe_function_type_##_FUNCTION##__ > >::Register(); \
		} \
	}; \
};

#define IMPLEMENT_META_FUNCTION_2( _MODULE, _FUNCTION ) \
namespace XE \
{ \
	class __xe_function_type_##_MODULE##_##_FUNCTION##__ {}; \
	template< > struct MetaTypeCollector< __xe_function_type_##_MODULE##_##_FUNCTION##__ > \
	{ \
		using FunctionType = decltype( _FUNCTION ); \
		MetaFunctionCollector() \
		{ \
			static constexpr char __xe__sig__[] = __FUNCTION__; \
			static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaFunction< FunctionType > >( XE::Hash( __xe__sig__ ), #_FUNCTION, _MODULE::_FUNCTION, _MODULE::GetModule() ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< __xe_function_type_##_MODULE##_##_FUNCTION##__ > >::Register(); \
		} \
	}; \
};

#define IMPLEMENT_META_FUNCTION( ... )            MACRO_EXP_(MACRO_GLUE(IMPLEMENT_META_FUNCTION_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

#endif // CXXMETAFUNCTION_HPP__5E5D51FA_6BF7_4EDD_89F5_F6E19C3882CD

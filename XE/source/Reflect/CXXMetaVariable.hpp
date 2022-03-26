/*!
 * \file	CXXMetaVariable.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CXXMETAVARIABLE_HPP__888028E6_C248_44BD_94C8_5EB51E91B42A
#define CXXMETAVARIABLE_HPP__888028E6_C248_44BD_94C8_5EB51E91B42A

#include "MetaVariable.h"

BEG_XE_NAMESPACE

class XE_API MetaVariableImpl : public MetaVariable
{
public:
	MetaVariableImpl( const String & Name, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypePtr Value, MetaModulePtr Module )
		:MetaVariable( Name, IsConst, IsObject, IsPointer, IsSharedPtr, IsReference, Value, Module )
	{

	}

public:
	template< typename T > MetaVariableImpl & Attribute( const T & val )
	{
		static_assert( std::is_base_of_v< XE::MetaAttribute, XE::TypeTraits< T >::raw_t >, "does not belong to meta attribute" );

		_Attributes.push_back( XE::MakeShared< T >( val ) );

		return *this;
	}

	template< typename T, typename ... ARGS > MetaVariableImpl & Attribute( const T & val, ARGS &&... args )
	{
		Attribute( val );

		Attribute( std::forward< ARGS >( args )... );

		return *this;
	}
};

template< typename ValueType > class CXXMetaVariable : public MetaPropertyImpl
{
public:
	using VariableType = ValueType;

public:
	CXXMetaVariable( const String & Name, VariableType * Value, MetaModulePtr Module )
		:MetaVariableImpl( Name, true, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename TypeTraits< ValueType >::raw_t >::Get(), Module ), _Value( Value )
	{

	}

public:
	Variant Getter() const override
	{
		return *_Value;
	}

	void Setter( const Variant & val ) const override
	{
		if constexpr( !std::is_const_v< ValueType > )
		{
			*_Value = val.Value< ValueType >();
		}
		else
		{
			throw MetaException( shared_from_this(), "is const value" );
		}
	}

private:
	VariableType * _Value;
};

END_XE_NAMESPACE


#define IMPLEMENT_META_VARIABLE_1( _VARIABLE ) \
namespace XE \
{ \
	class __xe_variable_type_##_VARIABLE##__ {}; \
	template< > struct MetaTypeCollector< __xe_variable_type_##_VARIABLE##__ > \
	{ \
		MetaVariableCollector() \
		{ \
			static constexpr char __xe__sig__[] = __FUNCTION__; \
			static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaVariable< decltype( _VARIABLE ) > >( XE::Hash( __xe__sig__ ), #_VARIABLE, &_VARIABLE, nullptr ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< __xe_variable_type_##_VARIABLE##__ > >::Register(); \
		} \
	}; \
};

#define IMPLEMENT_META_VARIABLE_2( _MODULE, _VARIABLE ) \
namespace XE \
{ \
	class __xe_variable_type_##_MODULE##_##_VARIABLE##__ {}; \
	template< > struct MetaTypeCollector< __xe_variable_type_##_MODULE##_##_VARIABLE##__ > \
	{ \
		MetaVariableCollector() \
		{ \
			static constexpr char __xe__sig__[] = __FUNCTION__; \
			static auto meta = XE::MetaInfo::NewMetaInfo< XE::CXXMetaVariable< decltype( _MODULE::_VARIABLE ) > >( XE::Hash( __xe__sig__ ), #_VARIABLE, &_MODULE::_VARIABLE, _MODULE::GetModule() ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< __xe_variable_type_##_MODULE##_##_VARIABLE##__ > >::Register(); \
		} \
	}; \
};

#define IMPLEMENT_META_VARIABLE( ... )            MACRO_EXP_(MACRO_GLUE(IMPLEMENT_META_VARIABLE_,MACRO_ARGS_CONTER(__VA_ARGS__))(__VA_ARGS__))

#endif // CXXMETAVARIABLE_HPP__888028E6_C248_44BD_94C8_5EB51E91B42A

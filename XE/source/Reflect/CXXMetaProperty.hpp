/*!
 * \file   CXXMetaProperty.hpp
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CXXMETAPROPERTY_HPP__3B024B46_FC59_464C_8819_339019E204F5
#define __CXXMETAPROPERTY_HPP__3B024B46_FC59_464C_8819_339019E204F5

#include "MetaProperty.h"

BEG_XE_NAMESPACE

class XE_API MetaPropertyImpl : public MetaProperty
{
public:
	MetaPropertyImpl( const String & Name, bool IsStatic, bool IsConst, bool IsObject, bool IsPointer, bool IsSharedPtr, bool IsReference, MetaTypePtr Value, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaProperty( Name, IsStatic, IsConst, IsObject, IsPointer, IsSharedPtr, IsReference, Value, Owner, Module )
	{

	}

public:
	template< typename T > MetaPropertyImpl & Attribute( const T & val )
	{
		static_assert( std::is_base_of_v< XE::MetaAttribute, XE::TypeTraits< T >::raw_t >, "does not belong to meta attribute" );

		for( auto & attr : _Attributes )
		{
			XE_ASSERT( ::TypeID< T >::Get() != attr->GetMetaClass() && "" );
		}

		_Attributes.push_back( XE::MakeShared< T >( val ) );

		return *this;
	}

	template< typename T, typename ... ARGS > MetaPropertyImpl & Attribute( const T & val, ARGS &&... args )
	{
		Attribute( val );

		Attribute( std::forward< ARGS >( args )... );

		return *this;
	}
};

template< typename ValueType > class CXXMetaProperty< ValueType > : public MetaPropertyImpl
{
public:
	using PropertyType = ValueType;

public:
	CXXMetaProperty( const String & Name, PropertyType * Value, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, true, std::is_const_v< ValueType >, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _Value( Value )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_pointer_v<PropertyType> )
		{
			return *_Value;
		}
		else
		{
			return _Value;
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		if constexpr( !std::is_const_v< ValueType > )
		{
			*_Value = val.Value< std::remove_reference_t< ValueType > >();
		}
		else
		{
			throw MetaException( shared_from_this(), "is const value" );
		}
	}

private:
	PropertyType * _Value;
};

template< typename ClassType, typename ValueType > class CXXMetaProperty< ValueType( ClassType:: * ) > : public MetaPropertyImpl
{
public:
	using PropertyType = ValueType( ClassType:: * );

public:
	CXXMetaProperty( const String & Name, PropertyType Value, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, std::is_const_v< ValueType >, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _Value( Value )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( obj.Value< ClassType * >()->*( _Value ) );
		}
		else
		{
			return obj.Value< ClassType * >()->*( _Value );
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		if constexpr( !std::is_const_v< ValueType > )
		{
			obj.Value< ClassType * >()->*( _Value ) = val.Value< std::remove_reference_t< ValueType > >();
		}
		else
		{
			throw MetaException( shared_from_this(), "is const value" );
		}
	}

private:
	PropertyType _Value;
};

template< typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( * )( ), void( * )( _SetType ) > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( * )( );
	using SetFuncType = void( * )( _SetType );

public:
	CXXMetaProperty( const String & Name, GetFuncType get, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( _GetFunc() );
		}
		else
		{
			return _GetFunc();
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		_SetFunc( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( ClassType:: * )( ), void( ClassType:: * )( _SetType ) > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( );
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const String & Name, GetFuncType get, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( ( obj.Value< ClassType * >()->*_GetFunc )( ) );
		}
		else
		{
			return ( obj.Value< ClassType * >()->*_GetFunc )( );
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) const, void( ClassType:: * )( _SetType ) > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( ) const;
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const String & Name, GetFuncType get, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( ( obj.Value< ClassType * >()->*_GetFunc )( ) );
		}
		else
		{
			return ( obj.Value< ClassType * >()->*_GetFunc )( );
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename _GetType > class CXXMetaProperty< _GetType( * )( ) > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( * )( );

public:
	CXXMetaProperty( const String & Name, GetFuncType get, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( _GetFunc() );
		}
		else
		{
			return _GetFunc();
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename ClassType, typename _GetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( );

public:
	CXXMetaProperty( const String & Name, GetFuncType get, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( ( obj.Value< ClassType * >()->*_GetFunc )( ) );
		}
		else
		{
			return ( obj.Value< ClassType * >()->*_GetFunc )( );
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename ClassType, typename _GetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) const > : public MetaPropertyImpl
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( ) const;

public:
	CXXMetaProperty( const String & Name, GetFuncType get, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		if constexpr( std::is_reference_v< ValueType > )
		{
			return &( ( obj.Value< ClassType * >()->*_GetFunc )( ) );
		}
		else
		{
			return ( obj.Value< ClassType * >()->*_GetFunc )( );
		}
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename _SetType > class CXXMetaProperty< void( * )( _SetType ) > : public MetaPropertyImpl
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( * )( _SetType );

public:
	CXXMetaProperty( const String & Name, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		_SetFunc( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _SetType > class CXXMetaProperty< void( ClassType:: * )( _SetType ) > : public MetaPropertyImpl
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const String & Name, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _SetType > class CXXMetaProperty< void( ClassType:: * )( _SetType ) const > : public MetaPropertyImpl
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( ClassType:: * )( _SetType ) const;

public:
	CXXMetaProperty( const String & Name, SetFuncType set, MetaClassPtr Owner, MetaModulePtr Module )
		:MetaPropertyImpl( Name, false, false, !std::is_pointer_v< ValueType > && !std::is_reference_v< ValueType > && !std::is_weak_ptr_v< ValueType > && !std::is_shared_ptr_v< ValueType >, std::is_pointer_v< ValueType >, std::is_shared_ptr_v< ValueType > || std::is_weak_ptr_v< ValueType >, std::is_reference_v< ValueType >, TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	Variant Getter( const Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const Variant & obj, const Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

END_XE_NAMESPACE

#endif // __CXXMETAPROPERTY_HPP__3B024B46_FC59_464C_8819_339019E204F5

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

template< typename ValueType > class CXXMetaProperty< ValueType > : public XE::MetaProperty
{
public:
	using PropertyType = ValueType;

public:
	CXXMetaProperty( const XE::String & Name, PropertyType * Value, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, true, std::is_const_v< ValueType >, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _Value( Value )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		if constexpr( !std::is_const_v< ValueType > )
		{
			*_Value = val.Value< std::remove_reference_t< ValueType > >();
		}
		else
		{
			throw XE::MetaException( shared_from_this(), "is const value" );
		}
	}

private:
	PropertyType * _Value;
};

template< typename ClassType, typename ValueType > class CXXMetaProperty< ValueType( ClassType:: * ) > : public XE::MetaProperty
{
public:
	using PropertyType = ValueType( ClassType:: * );

public:
	CXXMetaProperty( const XE::String & Name, PropertyType Value, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, std::is_const_v< ValueType >, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _Value( Value )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		if constexpr( !std::is_const_v< ValueType > )
		{
			obj.Value< ClassType * >()->*( _Value ) = val.Value< std::remove_reference_t< ValueType > >();
		}
		else
		{
			throw XE::MetaException( shared_from_this(), "is const value" );
		}
	}

private:
	PropertyType _Value;
};

template< typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( * )( ), void( * )( _SetType ) > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( * )( );
	using SetFuncType = void( * )( _SetType );

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		_SetFunc( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( ClassType:: * )( ), void( ClassType:: * )( _SetType ) > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( );
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _GetType, typename _SetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) const, void( ClassType:: * )( _SetType ) > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( ) const;
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	GetFuncType _GetFunc;
	SetFuncType _SetFunc;
};

template< typename _GetType > class CXXMetaProperty< _GetType( * )( ) > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( * )( );

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename ClassType, typename _GetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( );

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename ClassType, typename _GetType > class CXXMetaProperty< _GetType( ClassType:: * )( ) const > : public XE::MetaProperty
{
public:
	using ValueType = _GetType;
	using GetFuncType = _GetType( ClassType:: * )( ) const;

public:
	CXXMetaProperty( const XE::String & Name, GetFuncType get, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _GetFunc( get )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
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

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		XE_ASSERT( false && "is read only!" );
	}

private:
	GetFuncType _GetFunc;
};

template< typename _SetType > class CXXMetaProperty< void( * )( _SetType ) > : public XE::MetaProperty
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( * )( _SetType );

public:
	CXXMetaProperty( const XE::String & Name, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		_SetFunc( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _SetType > class CXXMetaProperty< void( ClassType:: * )( _SetType ) > : public XE::MetaProperty
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( ClassType:: * )( _SetType );

public:
	CXXMetaProperty( const XE::String & Name, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

template< typename ClassType, typename _SetType > class CXXMetaProperty< void( ClassType:: * )( _SetType ) const > : public XE::MetaProperty
{
public:
	using ValueType = _SetType;
	using SetFuncType = void( ClassType:: * )( _SetType ) const;

public:
	CXXMetaProperty( const XE::String & Name, SetFuncType set, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module )
		: XE::MetaProperty( Name, false, false, XE::TypeTraits< ValueType >::flag, ::TypeID< typename XE::TypeTraits< ValueType >::raw_t >::Get(), Owner, Module ), _SetFunc( set )
	{

	}

public:
	XE::Variant Getter( const XE::Variant & obj ) const override
	{
		XE_ASSERT( false && "is only wirte!" );
		return Variant();
	}

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override
	{
		( obj.Value< ClassType * >()->*_SetFunc )( val.Value< std::remove_reference_t< ValueType > >() );
	}

private:
	SetFuncType _SetFunc;
};

END_XE_NAMESPACE

#endif // __CXXMETAPROPERTY_HPP__3B024B46_FC59_464C_8819_339019E204F5

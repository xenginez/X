/*!
 * \file	Asset.h
 *
 * \author	ZhengYuanQing
 * \date	2020/10/22
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASSET_H__728E58FA_63BE_4DF7_AF23_F7A0F10BCE88
#define ASSET_H__728E58FA_63BE_4DF7_AF23_F7A0F10BCE88

#include "Utils/UUID.h"

#include "Object.h"

BEG_XE_NAMESPACE

class XE_API Asset
{
public:
	Asset();

	Asset( const Asset & val );

	virtual ~Asset();

public:
	operator bool() const;

public:
	Asset & operator=( std::nullptr_t );

	Asset & operator=( const Asset & val );

public:
	const XE::UUID & GetUUID() const;

	void SetUUID( const XE::UUID & val );

	const XE::String & GetPath() const;

	void SetPath( const XE::String & val );

public:
	void Load();

	void AsyncLoad();

protected:
	virtual void LoadFinish( const XE::ObjectPtr & val );

protected:
	XE::ObjectPtr _Ptr;

private:
	XE::UUID _UUID;
	XE::String _Path;
};

template< typename T > class AssetPtr : public XE::Asset
{
public:
	AssetPtr() = default;

	AssetPtr( AssetPtr && val )
		:XE::Asset( val )
	{
	}

	AssetPtr( const AssetPtr & val )
		:XE::Asset( val )
	{
	}

	~AssetPtr() = default;

public:
	AssetPtr & operator=( std::nullptr_t )
	{
		XE::Asset::operator=( nullptr );
		return *this;
	}

	AssetPtr & operator=( Asset && val )
	{
		XE::Asset::operator=( val );
		return *this;
	}

	AssetPtr & operator=( const AssetPtr & val )
	{
		XE::Asset::operator=( val );
		return *this;
	}

public:
	T * Get() const
	{
		return static_cast< T * >( _Ptr.get() );
	}

	XE::SharedPtr< T > GetShared() const
	{
		return SP_CAST< T >( _Ptr );
	}

public:
	T * operator->() const
	{
		return Get();
	}

	T & operator*() const
	{
		return *Get();
	}

};

template< typename T > class AssetInstance : public XE::Asset
{
public:
	AssetInstance() = default;

	AssetInstance( AssetInstance && val )
		:XE::Asset( val )
	{
	}

	AssetInstance( const AssetInstance & val )
		:XE::Asset( val )
	{
	}

	~AssetInstance() = default;

public:
	AssetInstance & operator=( std::nullptr_t )
	{
		XE::Asset::operator=( nullptr );
		return *this;
	}

	AssetInstance & operator=( AssetInstance && val )
	{
		XE::Asset::operator=( val );
		return *this;
	}

	AssetInstance & operator=( const AssetInstance & val )
	{
		XE::Asset::operator=( val );
		return *this;
	}

public:
	T * Get() const
	{
		return static_cast< T * >( _Ptr.get() );
	}

	XE::SharedPtr< T > GetShared() const
	{
		return SP_CAST< T >( _Ptr );
	}

public:
	T * operator->() const
	{
		return Get();
	}

	T & operator*() const
	{
		return *Get();
	}

protected:
	void LoadFinish( const XE::ObjectPtr & val ) override
	{
		auto to = val->GetMetaClass()->ConstructPtr();
		val->GetMetaClass()->Clone( val, to );
		_Ptr = to.Value< XE::ObjectPtr >();
	}
};

DECL_XE_CLASS( Asset );
IMPLEMENT_META_TEMPLATE_CLASS( XE, AssetPtr, XE::Asset );
IMPLEMENT_META_TEMPLATE_CLASS( XE, AssetInstance, XE::Asset );

END_XE_NAMESPACE

#endif // ASSET_H__728E58FA_63BE_4DF7_AF23_F7A0F10BCE88

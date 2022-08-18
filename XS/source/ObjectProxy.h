/*!
 * \file	ObjectProxy.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef OBJECTPROXY_H__58FCABFE_FA64_4AB6_980B_9EC958C6B386
#define OBJECTPROXY_H__58FCABFE_FA64_4AB6_980B_9EC958C6B386

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API ObjectProxy
{
public:
	ObjectProxy( ObjectProxy * parent = nullptr );

public:
	virtual XE::MetaTypeCPtr GetType() const = 0;

	virtual const XE::String & GetName() const = 0;

	virtual XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const = 0;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( ::ClassID< T >::Get() ) );
	}

public:
	virtual XE::Variant GetValue() const = 0;

	virtual void SetValue( const XE::Variant & val ) = 0;

protected:
	XS::ObjectProxy * GetParent() const;

private:
	XS::ObjectProxy * _Parent;
};

class XS_API VariantObjectProxy : public ObjectProxy
{
public:
	VariantObjectProxy( const XE::Variant & obj, ObjectProxy * parent = nullptr );

public:
	XE::MetaTypeCPtr GetType() const override;

	const XE::String & GetName() const override;

	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const override;

public:
	XE::Variant GetValue() const override;

	void SetValue( const XE::Variant & val ) override;

private:
	XE::Variant _Value;
	XS::ObjectProxy * _Parent;
};

class XS_API ElementObjectProxy : public XS::ObjectProxy
{
public:
	ElementObjectProxy( XE::uint64 i, XE::MetaEnumeratorPtr & enumer, XS::ObjectProxy * parent );

public:
	XE::MetaTypeCPtr GetType() const override;

	const XE::String & GetName() const override;

	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const override;

public:
	XE::Variant GetValue() const override;

	void SetValue( const XE::Variant & val ) override;

private:
	XE::uint64 _Index;
	XE::MetaEnumeratorPtr & _Enumerator;
};

class XS_API PropertyObjectProxy : public XS::ObjectProxy
{
public:
	PropertyObjectProxy( const XE::MetaPropertyCPtr & prop, XS::ObjectProxy * parent );

public:
	XE::MetaTypeCPtr GetType() const override;

	const XE::String & GetName() const override;

	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const override;

public:
	XE::Variant GetValue() const override;

	void SetValue( const XE::Variant & val ) override;

private:
	XE::MetaPropertyCPtr _Property;
};

END_XS_NAMESPACE

#endif//OBJECTPROXY_H__58FCABFE_FA64_4AB6_980B_9EC958C6B386

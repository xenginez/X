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

class XS_API ObjectProxy : public QObject
{
	Q_OBJECT

public:
	ObjectProxy();

	ObjectProxy( const ObjectProxy & val );

	ObjectProxy & operator= ( const ObjectProxy & val );

	ObjectProxy( const XE::Variant & obj, const XE::MetaPropertyCPtr & prop = nullptr );

public:
	XE::MetaTypeCPtr GetType() const;

	XE::MetaAttributeCPtr FindAttribute( const XE::MetaClassCPtr & type ) const;

	template< typename T > XE::SharedPtr< const T > FindAttributeT() const
	{
		return SP_CAST< const T >( FindAttribute( ::ClassID< T >::Get() ) );
	}

public:
	XE::Variant GetValue() const;

	void SetValue( const XE::Variant & val );

private:
	XE::Variant _Object;
	XE::MetaPropertyCPtr _Property;
};

END_XS_NAMESPACE

#endif//OBJECTPROXY_H__58FCABFE_FA64_4AB6_980B_9EC958C6B386

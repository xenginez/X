/*!
 * \file   MetaClass.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETACLASS_H__0775C1BF_63EA_4CCE_84ED_90EBF29B082B
#define __IMETACLASS_H__0775C1BF_63EA_4CCE_84ED_90EBF29B082B

#include "MetaTypeList.hpp"
#include "MetaEnumerator.h"

BEG_XE_NAMESPACE

class XE_API MetaClass : public XE::MetaType
{
public:
	MetaClass( const XE::String & Name, XE::uint64 Size, bool IsAbs, bool IsCont, XE::MetaClassCPtr Super, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module, const XE::TemplateType & temps, const XE::MetaTypeCPtr & element );

	~MetaClass();

public:
	bool IsAbstract() const;

	bool IsTemplate() const;

	bool IsContainer() const;

public:
	XE::MetaClassCPtr GetSuper() const;

	XE::uint64 GetMethodSize() const;

	XE::uint64 GetPropertySize() const;

	XE::uint64 GetDerivedClassSize() const;

	XE::MetaTypeCPtr GetElementType() const;

public:
	const XE::TemplateType & GetTemplateTypes() const;

	const XE::Array< XE::MetaMethodCPtr > & GetMethods() const;

	const XE::Array< XE::MetaPropertyCPtr > & GetPropertys() const;

	const XE::Array< const XE::MetaClass * > & GetDerivedClasses() const;

public:
	bool CanConvert( const XE::MetaClass * val ) const;

	bool CanConvert( const XE::MetaClassCPtr & val ) const;

public:
	void VisitMethod( const XE::Delegate< void( const XE::MetaMethodCPtr & ) > & val ) const;

	void VisitProperty( const XE::Delegate< void( const XE::MetaPropertyCPtr & ) > & val ) const;

	void VisitDerivedClass( const  XE::Delegate< void( const XE::MetaClassCPtr & ) > & val ) const;

public:
	XE::MetaMethodCPtr FindMethod( XE::uint64 hash ) const;

	XE::MetaPropertyCPtr FindProperty( XE::uint64 hash ) const;

public:
	XE::MetaMethodCPtr FindMethod( const XE::String & Name ) const;

	XE::MetaPropertyCPtr FindProperty( const XE::String & Name ) const;

	XE::MetaMethodCPtr FindMethod( const XE::String & Name, const XE::ParameterType & Types ) const;

public:
	virtual void Destruct( void * ptr ) const = 0;

	virtual XE::Variant Construct() const = 0;

	virtual XE::Variant Construct( void * ptr ) const = 0;

	virtual void Clone( const XE::Variant & from, XE::Variant & to ) const = 0;

	virtual XE::MetaEnumeratorPtr GetEnumerator( const XE::Variant & container ) const = 0;

protected:
	void _RegisterMethod( const XE::MetaMethodPtr & val );

	void _RegisterProperty( const XE::MetaPropertyPtr & val );

private:
	bool _IsAbstract;
	bool _IsContainer;
	XE::MetaClassCWPtr _Super;
	XE::TemplateType _Templates;
	XE::MetaTypeCPtr _ElementType;
	XE::Array< XE::MetaMethodCPtr > _Methods;
	XE::Array< XE::MetaPropertyCPtr > _Propertys;
	XE::Array< const XE::MetaClass * > _DerivedClasses;
};

END_XE_NAMESPACE

#endif // __IMETACLASS_H__0775C1BF_63EA_4CCE_84ED_90EBF29B082B

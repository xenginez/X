/*!
 * \file	MetaAttribute.h
 *
 * \author	ZhengYuanQing
 * \date	2021/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef METAATTRIBUTE_H__E5DC9799_360B_4774_BE2A_44AD3F7F89AB
#define METAATTRIBUTE_H__E5DC9799_360B_4774_BE2A_44AD3F7F89AB

#include "Variant.h"

BEG_XE_NAMESPACE

class XE_API MetaAttribute : public XE::EnableSharedFromThis< MetaAttribute >
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	MetaAttribute() = default;

	virtual ~MetaAttribute() = default;

public:
	virtual XE::Variant Execute( const XE::Variant & val ) const;
};

class XE_API RangeAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	RangeAttribute( XE::float64 min, XE::float64 max );

public:
	XE::Variant Execute( const XE::Variant & val ) const override;

private:
	XE::float64 _Min = std::numeric_limits< XE::float64 >::min();
	XE::float64 _Max = std::numeric_limits< XE::float64 >::max();
};

class XE_API NonCloneAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	NonCloneAttribute() = default;
};

class XE_API NonEditorAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	NonEditorAttribute() = default;
};

class XE_API NonSerializeAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	NonSerializeAttribute() = default;
};

END_XE_NAMESPACE

#endif // METAATTRIBUTE_H__E5DC9799_360B_4774_BE2A_44AD3F7F89AB

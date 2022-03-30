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
};

class XE_API FlagAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;
};

class XE_API RangeAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	RangeAttribute( XE::float64 min, XE::float64 max, XE::float64 step = 1.0 );

public:
	XE::float64 GetMin() const;

	XE::float64 GetMax() const;

	XE::float64 GetStep() const;

private:
	XE::float64 _Step = 1.0;
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

class XE_API InspectorAttribute : public MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	virtual XE::MetaClassPtr GetMetaClass() const;

public:
	InspectorAttribute( const XE::String & type );

public:
	const XE::String & GetTypeName() const;

private:
	XE::String _Type;
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

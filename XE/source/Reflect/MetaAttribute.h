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

class XE_API FlagAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;
};

class XE_API LinkAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	LinkAttribute( const XE::String & name, const XE::Delegate<bool( const XE::Variant & )> & callback );

public:
	const XE::String & GetLinkName() const;

	const XE::Delegate<bool( const XE::Variant & link )> & GetLinkCallback() const;

private:
	XE::String _Name;
	XE::Delegate<bool( const XE::Variant & )> _Callback;
};

class XE_API RangeAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

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

class XE_API NonCloneAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	NonCloneAttribute() = default;
};

class XE_API InspectorAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	InspectorAttribute( const XE::String & type );

public:
	const XE::String & GetTypeName() const;

private:
	XE::String _Type;
};

class XE_API NonInspectorAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	NonInspectorAttribute() = default;
};

class XE_API NonSerializeAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	NonSerializeAttribute() = default;
};

class XE_API ParameterNamesAttribute : public XE::MetaAttribute
{
public:
	static XE::MetaClassPtr GetMetaClassStatic();

	XE::MetaClassPtr GetMetaClass() const override;

public:
	ParameterNamesAttribute( const XE::Array< XE::String > & names );

public:
	const XE::Array< XE::String > & GetNames() const;

private:
	XE::Array< XE::String > _Names;
};

END_XE_NAMESPACE

#endif // METAATTRIBUTE_H__E5DC9799_360B_4774_BE2A_44AD3F7F89AB

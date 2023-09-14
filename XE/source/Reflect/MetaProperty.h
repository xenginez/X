/*!
 * \file   MetaProperty.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621
#define __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621

#include "MetaInfo.h"

BEG_XE_NAMESPACE

class XE_API MetaProperty : public XE::MetaInfo
{
public:
	MetaProperty( const XE::String & Name, bool IsStatic, bool IsConst, XE::TypeFlag Flag, XE::MetaTypeCPtr Value, XE::MetaClassCPtr Owner, XE::MetaModuleCPtr Module );

	~MetaProperty();

public:
	bool IsConst() const;

	bool IsStatic() const;

	bool IsObject() const;

	bool IsPointer() const;

	bool IsSharedPtr() const;

	bool IsReference() const;

	XE::TypeFlag GetFlag() const;

public:
	XE::MetaTypeCPtr GetValueType() const;

public:
	XE::Variant Get( const XE::Variant & obj ) const;

	void Set( const XE::Variant & obj, const XE::Variant & val ) const;

protected:
	virtual XE::Variant Getter( const XE::Variant & obj ) const = 0;

	virtual void Setter( const XE::Variant & obj, const XE::Variant & val ) const = 0;

private:
	bool _IsConst;
	bool _IsStatic;
	XE::TypeFlag _Flag;
	XE::MetaTypeCWPtr _Value;
};

END_XE_NAMESPACE

#endif // __IMETAPROPERTY_H__5D5C09CC_58EB_41F1_9D84_BD6FC0BCB621

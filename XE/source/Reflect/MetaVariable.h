/*!
 * \file	MetaVariable.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA
#define METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA

#include "Variant.h"

BEG_XE_NAMESPACE

class XE_API MetaVariable : public XE::MetaInfo
{
public:
	MetaVariable( const XE::String & Name, bool IsConst, XE::TypeFlag Flag, XE::MetaTypeCPtr Value, XE::MetaModuleCPtr Module );

	~MetaVariable();

public:
	bool IsConst() const;

	bool IsObject() const;

	bool IsPointer() const;

	bool IsSharedPtr() const;

	bool IsReference() const;

	XE::TypeFlag GetFlag() const;

public:
	XE::MetaTypeCPtr GetValueType() const;

public:
	XE::Variant Get() const;

	void Set( const XE::Variant & val ) const;

protected:
	virtual XE::Variant Getter() const = 0;

	virtual void Setter( const XE::Variant & val ) const = 0;

private:
	bool _IsConst;
	XE::TypeFlag _Flag;
	XE::MetaTypeCWPtr _Value;
};

END_XE_NAMESPACE

#endif // METAVARIABLE_H__082CE86F_3DEE_4687_88D2_32D432AE58CA

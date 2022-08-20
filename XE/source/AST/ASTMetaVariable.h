/*!
 * \file	ASTMetaVariable.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETAVARIABLE_H__F3DD194D_8345_4223_8A30_EAE4996A0CB0
#define ASTMETAVARIABLE_H__F3DD194D_8345_4223_8A30_EAE4996A0CB0

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaVariable : public XE::MetaVariable
{
public:
	ASTMetaVariable( const XE::ASTInfoVariablePtr & val, const XE::MetaModuleCPtr & Module );

	~ASTMetaVariable() override;

public:
	XE::Variant Getter() const override;

	void Setter( const XE::Variant & val ) const override;

private:
	XE::ASTInfoVariablePtr _Variable;
};

END_XE_NAMESPACE

#endif//ASTMETAVARIABLE_H__F3DD194D_8345_4223_8A30_EAE4996A0CB0

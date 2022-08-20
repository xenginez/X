/*!
 * \file	ASTMetaFunction.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETAFUNCTION_H__476A6422_9F75_4633_AAC9_97BE990C0A1A
#define ASTMETAFUNCTION_H__476A6422_9F75_4633_AAC9_97BE990C0A1A

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaFunction : public XE::MetaFunction
{
public:
	ASTMetaFunction( const XE::ASTInfoFunctionPtr & val, const XE::MetaModuleCPtr & Module );

	~ASTMetaFunction() override;

public:
	XE::Variant Invoke( XE::InvokeStack * params ) const override;

private:
	XE::ASTInfoFunctionPtr _Function;
};

END_XE_NAMESPACE

#endif//ASTMETAFUNCTION_H__476A6422_9F75_4633_AAC9_97BE990C0A1A

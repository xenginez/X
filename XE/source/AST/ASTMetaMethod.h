/*!
 * \file	ASTMetaMethod.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETAMETHOD_H__252176A8_0BC2_48FE_A5C9_E45CDC68282B
#define ASTMETAMETHOD_H__252176A8_0BC2_48FE_A5C9_E45CDC68282B

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaMethod : public XE::MetaMethod
{
public:
	ASTMetaMethod( const XE::SharedPtr< XE::ASTMethod > & val );

	~ASTMetaMethod() override;

public:
	XE::Variant Invoke( XE::InvokeStack * params ) const override;

private:
	XE::SharedPtr< XE::ASTMethod > _Method;
};

END_XE_NAMESPACE

#endif//ASTMETAMETHOD_H__252176A8_0BC2_48FE_A5C9_E45CDC68282B

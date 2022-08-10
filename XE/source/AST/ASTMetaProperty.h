/*!
 * \file	ASTMetaProperty.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETAPROPERTY_H__AE5480C9_0FBB_452A_AAFB_0FC2DF369546
#define ASTMETAPROPERTY_H__AE5480C9_0FBB_452A_AAFB_0FC2DF369546

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaProperty : public XE::MetaProperty
{
public:
	ASTMetaProperty( const XE::SharedPtr< XE::ASTProperty > & val );

	~ASTMetaProperty() override;

public:
	XE::Variant Getter( const XE::Variant & obj ) const override;

	void Setter( const XE::Variant & obj, const XE::Variant & val ) const override;

private:
	XE::SharedPtr< XE::ASTProperty > _Property;
};

END_XE_NAMESPACE

#endif//ASTMETAPROPERTY_H__AE5480C9_0FBB_452A_AAFB_0FC2DF369546

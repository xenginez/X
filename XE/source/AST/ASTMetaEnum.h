/*!
 * \file	ASTMetaEnum.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETAENUM_H__7E7DF068_AEC4_4374_AD70_CCE5A8EFEB30
#define ASTMETAENUM_H__7E7DF068_AEC4_4374_AD70_CCE5A8EFEB30

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaEnum : public XE::MetaEnum
{
public:
	ASTMetaEnum( const XE::SharedPtr< XE::ASTEnum > & val );

	~ASTMetaEnum() override;

public:
	void Serialize( XE::OArchive & arc, const XE::Variant & val ) const override;

	void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const override;

private:
	XE::SharedPtr< XE::ASTEnum > _Enum;
};

END_XE_NAMESPACE

#endif//ASTMETAENUM_H__7E7DF068_AEC4_4374_AD70_CCE5A8EFEB30

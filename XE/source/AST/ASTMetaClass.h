/*!
 * \file	ASTMetaClass.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/10
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTMETACLASS_H__989D3CE6_D2EC_407C_8E78_352D9EDCC7C4
#define ASTMETACLASS_H__989D3CE6_D2EC_407C_8E78_352D9EDCC7C4

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTMetaClass : public XE::MetaClass
{
public:
	ASTMetaClass( const XE::SharedPtr< XE::ASTClass > & val );

	~ASTMetaClass() override;

public:
	void Destruct( void * ptr ) const override;

	XE::Variant Construct( void * ptr ) const override;

	void Clone( const XE::Variant & from, XE::Variant & to ) const override;

	void Serialize( XE::OArchive & arc, const XE::Variant & val ) const override;

	void Deserialize( XE::IArchive & arc, XE::Variant & val ) const override;

private:
	XE::SharedPtr< XE::ASTClass > _Class;
};

END_XE_NAMESPACE

#endif//ASTMETACLASS_H__989D3CE6_D2EC_407C_8E78_352D9EDCC7C4
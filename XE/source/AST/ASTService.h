/*!
 * \file	ASTService.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTSERVICE_H__82EACF61_3B7B_4BDD_9019_220C21E79A42
#define ASTSERVICE_H__82EACF61_3B7B_4BDD_9019_220C21E79A42

#include "Type.h"
#include "Core/Service.h"

BEG_XE_NAMESPACE

class XE_API ASTService : public XE::Service
{
	OBJECT( ASTService, XE::Service )

private:
	struct Private;

public:
	ASTService();

	~ASTService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	void Visit( XE::ASTContext * context, XE::ASTNode * node ) const;

	void Visit( XE::ASTContext * context, const XE::ASTNodePtr & node ) const;

public:
	void SetVariable( const XE::String & name, const XE::Variant & val );

	XE::Variant GetVariable( const XE::String & name );

public:
	void AddGlobalMacro( const XE::String & val );

	bool HasGlobalMacro( const XE::String & val ) const;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTSERVICE_H__82EACF61_3B7B_4BDD_9019_220C21E79A42

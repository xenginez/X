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
	void AddMacro( const XE::String & val );

	bool HasMacro( const XE::String & val ) const;

public:
	void AddCustomNodeExecute( const XE::MetaClassCPtr & type, const XE::Delegate< void( XE::ASTVisitor * ) > & callback );

	void AddCustomNodeCompile( const XE::MetaClassCPtr & type, const XE::Delegate< void( XE::ASTVisitor * ) > & callback );

	bool ExecuteCustomNode( const XE::MetaClassCPtr & type, XE::ASTVisitor * visitor ) const;

	bool CompileCustomNode( const XE::MetaClassCPtr & type, XE::ASTVisitor * visitor ) const;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//ASTSERVICE_H__82EACF61_3B7B_4BDD_9019_220C21E79A42

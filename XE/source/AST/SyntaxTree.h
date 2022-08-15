/*!
 * \file	SyntaxTree.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SYNTAXTREE_H__34DAA02C_5DC7_404B_92E2_C0F41E5D7AE2
#define SYNTAXTREE_H__34DAA02C_5DC7_404B_92E2_C0F41E5D7AE2

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API SyntaxTree : public XE::Object
{
	OBJECT( SyntaxTree, XE::Object )

public:
	SyntaxTree();

	~SyntaxTree() override;

public:
	const XE::Array<XE::ASTEnumPtr> & GetEnums() const;

	void SetEnums( const XE::Array<XE::ASTEnumPtr> & val );

	const XE::Array<XE::ASTClassPtr> & GetClasses() const;

	void SetClasses( const XE::Array<XE::ASTClassPtr> & val );

	const XE::Array<XE::ASTModulePtr> & GetModules() const;

	void SetModules( const XE::Array<XE::ASTModulePtr> & val );

	const XE::Array<XE::ASTMethodPtr> & GetMethods() const;

	void SetMethods( const XE::Array<XE::ASTMethodPtr> & val );

	const XE::Array<XE::ASTPropertyPtr> & GetPropertys() const;

	void SetPropertys( const XE::Array<XE::ASTPropertyPtr> & val );

	const XE::Array<XE::ASTFunctionPtr> & GetFunctions() const;

	void SetFunctions( const XE::Array<XE::ASTFunctionPtr> & val );

	const XE::Array<XE::ASTVariablePtr> & GetVariables() const;

	void SetVariables( const XE::Array<XE::ASTVariablePtr> & val );

public:
	void AddEnum( const XE::ASTEnumPtr & val );

	void AddClass( const XE::ASTClassPtr & val );

	void AddModule( const XE::ASTModulePtr & val );

	void AddMethod( const XE::ASTMethodPtr & val );

	void AddProperty( const XE::ASTPropertyPtr & val );

	void AddFunction( const XE::ASTFunctionPtr & val );

	void AddVariable( const XE::ASTVariablePtr & val );

private:
	XE::Array< XE::ASTEnumPtr > _Enums;
	XE::Array< XE::ASTClassPtr > _Classes;
	XE::Array< XE::ASTModulePtr > _Modules;
	XE::Array< XE::ASTMethodPtr > _Methods;
	XE::Array< XE::ASTPropertyPtr > _Propertys;
	XE::Array< XE::ASTFunctionPtr > _Functions;
	XE::Array< XE::ASTVariablePtr > _Variables;
};

END_XE_NAMESPACE

#endif//SYNTAXTREE_H__34DAA02C_5DC7_404B_92E2_C0F41E5D7AE2

/*!
 * \file	ASTInfo.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/20
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTINFO_H__A71C29F2_CA24_4D76_ABFE_D34F95DBFF76
#define ASTINFO_H__A71C29F2_CA24_4D76_ABFE_D34F95DBFF76

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTInfoMeta : public XE::Object
{
	OBJECT( ASTInfoMeta, XE::Object );

public:
	XE::String Name;
};

class XE_API ASTInfoType : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoType, XE::ASTInfoMeta )

};

class XE_API ASTInfoEnum : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoEnum, XE::ASTInfoMeta )

public:
	bool Flag = false;
	XE::Array< XE::String > Elements;
};

class XE_API ASTInfoClass : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoClass, XE::ASTInfoMeta )

public:
	XE::String SuperClass;
	XE::ASTInfoFunctionPtr Destruct;
	XE::ASTInfoFunctionPtr Construct;
	XE::Array< XE::ASTInfoFunctionPtr > Methods;
	XE::Array< XE::ASTInfoPropertyPtr > Propertys;
};

class XE_API ASTInfoModule : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoModule, XE::ASTInfoMeta )

public:
	XE::Array< XE::ASTInfoEnumPtr > Enums;
	XE::Array< XE::ASTInfoClassPtr > Classes;
	XE::Array< XE::ASTInfoModulePtr > Modules;
	XE::Array< XE::ASTInfoFunctionPtr > Functions;
	XE::Array< XE::ASTInfoVariablePtr > Variables;
};

class XE_API ASTInfoProperty : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoProperty, XE::ASTInfoMeta )

public:
	XE::ASTInfoTypePtr Type;
	XE::TypeFlag Flag = XE::TypeFlag::VALUE;
	XE::uint64 Offset = 0;
};

class XE_API ASTInfoFunction : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoFunction, XE::ASTInfoMeta )

public:
	XE::ASTInfoTypePtr Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::ASTInfoTypePtr > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::ASTInfoTypePtr > > LocalVariables;
};

class XE_API ASTInfoVariable : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoVariable, XE::ASTInfoMeta )

public:
	XE::ASTInfoTypePtr Type;
	XE::TypeFlag Flag = XE::TypeFlag::VALUE;
};

END_XE_NAMESPACE

#endif//ASTINFO_H__A71C29F2_CA24_4D76_ABFE_D34F95DBFF76

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
	XE::SharedPtr< XE::ASTInfoMethod > Destruct;
	XE::SharedPtr< XE::ASTInfoMethod > Construct;
	XE::Array< XE::SharedPtr< XE::ASTInfoMethod > > Methods;
	XE::Array< XE::SharedPtr< XE::ASTInfoProperty > > Propertys;
};

class XE_API ASTInfoModule : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoModule, XE::ASTInfoMeta )

public:
	XE::Array< XE::SharedPtr< XE::ASTInfoEnum > > Enums;
	XE::Array< XE::SharedPtr< XE::ASTInfoClass > > Classes;
	XE::Array< XE::SharedPtr< XE::ASTInfoModule > > Modules;
	XE::Array< XE::SharedPtr< XE::ASTInfoFunction > > Functions;
	XE::Array< XE::SharedPtr< XE::ASTInfoVariable > > Variables;
};

class XE_API ASTInfoMethod : public XE::ASTInfoMeta
{
	OBJECT( ASTInfoMethod, XE::ASTInfoMeta )

public:
	XE::ASTInfoTypePtr Result;
	XE::Array< XE::ASTNodePtr > StatementBody;
	XE::Array< XE::Pair< XE::String, XE::ASTInfoTypePtr > > Parameters;
	XE::Array< XE::Pair< XE::String, XE::ASTInfoTypePtr > > LocalVariables;
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

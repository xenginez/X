/*!
 * \file	ASTInstance.h
 *
 * \author	ZhengYuanQing
 * \date	2022/11/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTINSTANCE_H__15652B0B_7F00_4876_AAA4_124A75A8EF05
#define ASTINSTANCE_H__15652B0B_7F00_4876_AAA4_124A75A8EF05

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API ASTInstance
{
public:
	ASTInstance();

	~ASTInstance();

public:
	XE::uint64 GetStackPoint() const;

public:
	XE::uint64 PushStack( const XE::Variant & val );

	XE::Variant PopStack();

	XE::Variant & TopStack();

	XE::Variant & GetStack( XE::uint64 sp );

	XE::uint64 ResetStack( XE::uint64 sp );

private:
	XE::Array< XE::Variant > _Stack;
};

END_XE_NAMESPACE

#endif//ASTINSTANCE_H__15652B0B_7F00_4876_AAA4_124A75A8EF05

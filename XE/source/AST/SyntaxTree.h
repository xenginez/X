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

};

END_XE_NAMESPACE

#endif//SYNTAXTREE_H__34DAA02C_5DC7_404B_92E2_C0F41E5D7AE2

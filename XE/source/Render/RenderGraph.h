/*!
 * \file	RenderGraph.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERGRAPH_H__BF0926B7_7A99_4EAD_B53C_350D0A575F11
#define RENDERGRAPH_H__BF0926B7_7A99_4EAD_B53C_350D0A575F11

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderResource : public XE::EnableSharedFromThis< XE::RenderResource >
{
	friend class RenderBuilder;

public:
	RenderResource() = default;

	~RenderResource() = default;

public:

};
DECL_XE_CLASS( RenderResource );

class XE_API RenderBuilder : public XE::EnableSharedFromThis< XE::RenderBuilder >
{
public:
	RenderBuilder() = default;

	~RenderBuilder() = default;

public:
	XE::RenderResourcePtr Compile();

};
DECL_XE_CLASS( RenderBuilder );

class XE_API RenderPass : public XE::EnableSharedFromThis< XE::RenderPass >
{
public:
	RenderPass() = default;

	virtual ~RenderPass() = default;

public:
	virtual void Setup( const RenderBuilderPtr & builder ) = 0;

	virtual void Execute( const XE::RenderResourcePtr & resource ) = 0;
};
DECL_XE_CLASS( RenderPass );

END_XE_NAMESPACE

#endif//RENDERGRAPH_H__BF0926B7_7A99_4EAD_B53C_350D0A575F11

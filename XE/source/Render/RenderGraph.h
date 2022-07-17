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

class XE_API RenderPass : public XE::EnableSharedFromThis< RenderPass >
{
public:
	RenderPass() = default;

	virtual ~RenderPass() = default;

public:
	virtual void Setup( XE::RenderBuilder & val ) const = 0;

	virtual void Execute( XE::RenderPassData & val ) const = 0;
};
DECL_XE_CLASS( RenderPass );

class XE_API RenderGraph : public XE::EnableSharedFromThis< XE::RenderGraph >
{
public:
	RenderGraph() = default;

	~RenderGraph() = default;

public:
	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	const XE::Graph< XE::RenderPassPtr > & GetGraphPass() const;

	void SetGraphPass( const XE::Graph< XE::RenderPassPtr > & val );

private:
	XE::String _Name;
	XE::Graph< XE::RenderPassPtr > _Pass;
};
DECL_XE_CLASS( RenderGraph );

class XE_API RenderPassData
{
public:


private:
	XE::RenderService * Service = nullptr;
	XE::RenderResource * Resource = nullptr;
};
DECL_XE_CLASS( RenderPassData );

class XE_API RenderBuilder : public XE::NonCopyable
{
	friend class RenderService;

public:
	RenderBuilder() = default;

	~RenderBuilder() = default;

public:
	RenderBuilder & AsyncCompute();

public:
	RenderBuilder & Read( XE::RenderResourceHandle val );

	RenderBuilder & Write( XE::RenderResourceHandle val );

	RenderBuilder & Across( XE::RenderResourceHandle val );

	RenderBuilder & ReadWrite( XE::RenderResourceHandle val );

public:
	XE::RenderResourceHandle Create( const XE::String & val );

private:
	XE::RenderPassData Compile( const XE::RenderPassPtr & val );
};
DECL_XE_CLASS( RenderBuilder );

class XE_API RenderResource : public XE::EnableSharedFromThis< RenderResource >
{
	friend class RenderService;

public:
	RenderResource() = default;

	~RenderResource() = default;

private:
	XE::RenderServiceWPtr _Service;
	XE::Array< XE::Variant > _Resources;
	XE::Map< const XE::RenderPass *, XE::RenderPassData > _PassDataMap;
};
DECL_XE_CLASS( RenderResource );

class XE_API RenderExecutor : public XE::NonCopyable
{
	friend class RenderService;

public:
	void Execute( const XE::RenderResourcePtr & resource, const XE::RenderGraphPtr & graph, const XE::RenderTexturePtr & texture );

	void Submit();

private:

};
DECL_XE_CLASS( RenderExecutor );

END_XE_NAMESPACE

#endif//RENDERGRAPH_H__BF0926B7_7A99_4EAD_B53C_350D0A575F11

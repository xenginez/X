#include "RenderGraph.h"

BEG_META( XE::RenderPass )
END_META()

BEG_META( XE::RenderGraph )
type->Attribute( XE::InspectorAttribute( "XS::RenderGraphEditor" ) );
type->Property( "Name", &XE::RenderGraph::GetName, &XE::RenderGraph::SetName );
type->Property( "GraphPass", &XE::RenderGraph::GetGraphPass, &XE::RenderGraph::SetGraphPass )->Attribute( XE::NonInspectorAttribute() );
END_META()

IMPLEMENT_META( XE::RenderPassData );

IMPLEMENT_META( XE::RenderBuilder );

IMPLEMENT_META( XE::RenderResource );

IMPLEMENT_META( XE::RenderExecutor );

const XE::String & XE::RenderGraph::GetName() const
{
	return _Name;
}

void XE::RenderGraph::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::Graph< XE::RenderPassPtr > & XE::RenderGraph::GetGraphPass() const
{
	return _Pass;
}

void XE::RenderGraph::SetGraphPass( const XE::Graph< XE::RenderPassPtr > & val )
{
	_Pass = val;
}

XE::RenderBuilder & XE::RenderBuilder::AsyncCompute()
{
	return *this;
}

XE::RenderBuilder & XE::RenderBuilder::Read( XE::RenderResourceHandle val )
{
	return *this;
}

XE::RenderBuilder & XE::RenderBuilder::Write( XE::RenderResourceHandle val )
{
	return *this;
}

XE::RenderBuilder & XE::RenderBuilder::Across( XE::RenderResourceHandle val )
{
	return *this;
}

XE::RenderBuilder & XE::RenderBuilder::ReadWrite( XE::RenderResourceHandle val )
{
	return *this;
}

XE::RenderResourceHandle XE::RenderBuilder::Create( const XE::String & val )
{
	return {};
}

XE::RenderPassData XE::RenderBuilder::Compile( const XE::RenderPassPtr & val )
{
	return {};
}

void XE::RenderExecutor::Execute( const XE::RenderResourcePtr & resource, const XE::RenderGraphPtr & graph, const XE::RenderTexturePtr & texture )
{

}

void XE::RenderExecutor::Submit()
{

}

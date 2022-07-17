#include "RenderGraph.h"

#include "Core/CoreFramework.h"

BEG_META( XE::RenderPass )
type->Property( "Name", &XE::RenderPass::GetName, &XE::RenderPass::SetName );
END_META()

const XE::String & XE::RenderPass::GetName() const
{
	return _Name;
}

void XE::RenderPass::SetName( const XE::String & val )
{
	_Name = val;
}


BEG_META( XE::RenderGraph )
type->Attribute( XE::InspectorAttribute( "XS::RenderGraphEditor" ) );
type->Property( "Name", &XE::RenderGraph::GetName, &XE::RenderGraph::SetName );
type->Property( "GraphPass", &XE::RenderGraph::GetGraphPass, &XE::RenderGraph::SetGraphPass )->Attribute( XE::NonInspectorAttribute() );
END_META()

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


IMPLEMENT_META( XE::RenderPassData );

XE::Variant XE::RenderPassData::GetValue( XE::RenderResourceHandle val ) const
{
	return _Resources[val.GetValue()].second;
}

void XE::RenderPassData::SetValue( XE::RenderResourceHandle handle, const XE::Variant & val )
{
	_Resources[handle.GetValue()].second = val;
	_Resource->_Resources[_Resources[handle.GetValue()].first] = val;
}

XE::CoreFrameworkPtr XE::RenderPassData::GetFramework() const
{
	return XE::CoreFramework::GetCurrentFramework();
}


IMPLEMENT_META( XE::RenderBuilder );

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

XE::RenderResourceHandle XE::RenderBuilder::Get( const XE::String & val )
{
	return {};
}

XE::RenderResourceHandle XE::RenderBuilder::Create( const XE::String & val )
{
	return {};
}

XE::RenderPassData XE::RenderBuilder::Compile( const XE::RenderPassPtr & val )
{
	return {};
}

IMPLEMENT_META( XE::RenderResource );


IMPLEMENT_META( XE::RenderExecutor );

void XE::RenderExecutor::Execute( const XE::RenderResourcePtr & resource, const XE::RenderGraphPtr & graph, const XE::RenderTexturePtr & texture )
{

}

void XE::RenderExecutor::Submit()
{

}

#include "GraphNodes.h"

BEG_META( XS::GraphNode )
END_META()

XS::GraphNode::GraphNode()
{
	_ID = 0;
	_Name = GetMetaClass()->GetFullName();
}

XS::GraphNode::~GraphNode()
{
}

XE::uint64 XS::GraphNode::GetID() const
{
	return _ID;
}

void XS::GraphNode::SetID( XE::uint64 val )
{
	_ID = val;
}

const XE::String & XS::GraphNode::GetName() const
{
	return _Name;
}

void XS::GraphNode::SetName( const XE::String & val )
{
	_Name = val;
}

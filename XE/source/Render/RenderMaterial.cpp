#include "RenderMaterial.h"

BEG_META( XE::RenderMaterial )
END_META()

XE::RenderMaterial::RenderMaterial()
{

}

XE::RenderMaterial::~RenderMaterial()
{

}

bool XE::RenderMaterial::GetTwoSided() const
{
	return _TwoSided;
}

void XE::RenderMaterial::SetTwoSided( bool val )
{
	_TwoSided = val;
}

XE::RenderQueueType XE::RenderMaterial::GetRenderQueueType() const
{
	return _RenderQueueType;
}

void XE::RenderMaterial::SetRenderQueueType( XE::RenderQueueType val )
{
	_RenderQueueType = val;
}

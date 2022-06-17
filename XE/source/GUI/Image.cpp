#include "Image.h"

#include "Core/GraphicsService.h"

#include "imgui.h"

BEG_META( XE::Image )
END_META()

XE::Image::Image()
{

}

XE::Image::~Image()
{

}

void XE::Image::OnStartup()
{
	_Picture.Load();

// 	XE::GraphicsTextureDescriptor desc;
// 	GetFramework()->GetServiceT< XE::GraphicsService >()->DeviceCreateTexture();
}

void XE::Image::OnRender()
{
	ImGui::Image( _Texture.GetValue(), { GetFrame().width, GetFrame().height } );
}

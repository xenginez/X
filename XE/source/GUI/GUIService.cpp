#include "GUIService.h"

#include "imgui.h"
#include "imgui_internal.h"

IMPLEMENT_META( XE::GUIService );

namespace
{
	struct _ImGuiContext
	{
		_ImGuiContext()
		{

		}

		~_ImGuiContext()
		{

		}

		ImGuiContext * CTX = nullptr;
	};
}

struct XE::GUIService::Private
{

};

XE::GUIService::GUIService()
	:_p( XE::New< Private >() )
{

}

XE::GUIService::~GUIService()
{
	XE::Delete( _p );
}

void XE::GUIService::Prepare()
{
	ImGui::SetAllocatorFunctions( 
		[]( size_t sz, void * user_data ) { return XE::MemoryResource::Alloc( sz ); },
		[]( void * ptr, void * user_data ) { XE::MemoryResource::Free( ptr ); },
		nullptr );
}

void XE::GUIService::Startup()
{

}

void XE::GUIService::Update()
{

}

void XE::GUIService::Clearup()
{

}

ImGuiContext * XE::GUIService::GetImGuiContext()
{
	thread_local _ImGuiContext ctx;

	return ctx.CTX;
}
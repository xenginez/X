#include "StaticMeshComponent.h"

BEG_META( XE::StaticMeshComponent )
END_META()

XE::StaticMeshComponent::StaticMeshComponent()
{
}

XE::StaticMeshComponent::~StaticMeshComponent()
{
}

void XE::StaticMeshComponent::OnStartup()
{
	Super::OnStartup();

	_Mesh.AsyncLoad();
}

void XE::StaticMeshComponent::OnUpdate( XE::float32 dt )
{
}

void XE::StaticMeshComponent::OnClearup()
{
	_Mesh = nullptr;

	Super::OnClearup();
}

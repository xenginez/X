/*!
 * \file	StaticMeshComponent.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/09
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef STATICMESHCOMPONENT_H__CB09505A_D60F_4ABB_9293_3A56A080EDFA
#define STATICMESHCOMPONENT_H__CB09505A_D60F_4ABB_9293_3A56A080EDFA

#include "Core/Asset.h"

#include "RenderComponent.h"

BEG_XE_NAMESPACE

class XE_API StaticMeshComponent : public XE::RenderComponent
{
	OBJECT( StaticMeshComponent, XE::RenderComponent )

public:
	StaticMeshComponent();

	~StaticMeshComponent() override;

protected:
	void OnStartup() override;

	void OnUpdate( XE::float32 dt ) override;

	void OnClearup() override;

private:
	XE::AssetPtr< XE::RenderMesh > _Mesh;
};

END_XE_NAMESPACE

#endif//STATICMESHCOMPONENT_H__CB09505A_D60F_4ABB_9293_3A56A080EDFA
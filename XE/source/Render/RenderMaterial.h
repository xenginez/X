/*!
 * \file	RenderMaterial.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERMATERIAL_H__32195DDB_35FD_4231_BB52_FBB272667429
#define RENDERMATERIAL_H__32195DDB_35FD_4231_BB52_FBB272667429

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderMaterial : public XE::Object
{
	OBJECT( RenderMaterial, XE::Object )

public:
	RenderMaterial();

	~RenderMaterial() override;

public:

private:
	bool _TowSided = false;
	XE::RenderShaderPtr _Shader;
	XE::RenderQueueType _RenderQueueType = RenderQueueType::GEOMETRY;
};

END_XE_NAMESPACE

#endif//RENDERMATERIAL_H__32195DDB_35FD_4231_BB52_FBB272667429

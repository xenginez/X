/*!
 * \file	RenderMesh.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef RENDERMESH_H__8BB22F95_C71A_4DFB_ACBD_18BCD66C1D0A
#define RENDERMESH_H__8BB22F95_C71A_4DFB_ACBD_18BCD66C1D0A

#include "Reflect/Object.h"
#include "Math/Mesh.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API RenderMesh : public XE::Object
{
	OBJECT( RenderMesh, XE::Object )

public:
	RenderMesh();

	~RenderMesh() override;

private:
	XE::Mesh _Mesh;
};

END_XE_NAMESPACE

#endif//RENDERMESH_H__8BB22F95_C71A_4DFB_ACBD_18BCD66C1D0A

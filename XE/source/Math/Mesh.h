/*!
 * \file	Mesh.h
 *
 * \author	ZhengYuanQing
 * \date	2021/12/04
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MESH_H__B1166AD3_51F0_4AF2_AFAA_989105440512
#define MESH_H__B1166AD3_51F0_4AF2_AFAA_989105440512

#include "Vec3.h"

BEG_XE_NAMESPACE

class XE_API Mesh
{
public:
	XE::Array< XE::Vec3f > vertices;
	XE::Array< XE::uint32 > triangles;
};
DECL_XE_CLASS( Mesh );

END_XE_NAMESPACE

#endif//MESH_H__B1166AD3_51F0_4AF2_AFAA_989105440512

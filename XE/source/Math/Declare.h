/*!
 * \file	Declare.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/12
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__A7A4C315_60C9_46C3_9BA5_BE7AAD3E9E96
#define DECLARE_H__A7A4C315_60C9_46C3_9BA5_BE7AAD3E9E96

#include "Reflect/Reflect.h"

BEG_XE_NAMESPACE

class Ray;
class OBB;
class AABB;
class Line;
class Quat;
class Mesh;
class Mathf;
class Plane;
class Color;
class FColor;
class Sphere;
class Capsule;
class float16;
class Frustum;
class Triangle;
class Transform;
class ConvexHull;
class HeightField;

template< typename T > class Vec2;
template< typename T > class Vec3;
template< typename T > class Vec4;
template< typename T > class Rect;
template< typename T, XE::uint64 R, XE::uint64 C > class Mat;

template< typename _Ty, typename _By > class QTree;
template< typename _Ty, typename _By > class OCTree;

END_XE_NAMESPACE

#endif // DECLARE_H__A7A4C315_60C9_46C3_9BA5_BE7AAD3E9E96

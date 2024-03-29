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

#include "Reflect/Declare.h"

BEG_XE_NAMESPACE

class int128;
class uint128;
class float16;
class fixed32;
class fixed64;

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
template< typename T > using Mat3 = Mat< T, 3, 3 >;
template< typename T > using Mat4 = Mat< T, 4, 4 >;

template< typename _Ty > class MakeRect;
template< typename _Ty > class MakeAABB;

END_XE_NAMESPACE

#endif // DECLARE_H__A7A4C315_60C9_46C3_9BA5_BE7AAD3E9E96

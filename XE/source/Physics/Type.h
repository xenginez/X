/*!
 * \file	Type.h
 *
 * \author	ZhengYuanQing
 * \date	2022/11/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__571C57BF_5056_4C91_8695_D95E7A44A3D7
#define TYPE_H__571C57BF_5056_4C91_8695_D95E7A44A3D7

#include <array>

#include "Math/Math.h"
#include "Utils/Layer.h"
#include "Utils/Buffer.h"
#include "Utils/Flags.hpp"
#include "Utils/Handle.hpp"
#include "Utils/Concurrent.hpp"

#include "Declare.h"

BEG_XE_NAMESPACE

DECL_HANDLE( PhysicsBody );
DECL_HANDLE( PhysicsWorld );
DECL_HANDLE( PhysicsShape );
DECL_HANDLE( PhysicsMaterial );
DECL_HANDLE( PhysicsConstraint );

static constexpr XE::uint64 PHYSICS_MAX_FRAME = 2;
static constexpr XE::uint64 PHYSICS_MAX_WORLD = 16;
static constexpr XE::uint64 PHYSICS_MAX_BODY = 1024;
static constexpr XE::uint64 PHYSICS_MAX_SHAPE = 2048;
static constexpr XE::uint64 PHYSICS_MAX_COMMAND = 2048;
static constexpr XE::uint64 PHYSICS_MAX_MATERIAL = 2048;
static constexpr XE::uint64 PHYSICS_MAX_CONSTRAINT = 2048;

enum class PhysicsBodyType
{
	SOFT_BODY,
	CLOTH_BODY,
	FLUID_BODY,
	STATIC_RIGID_BODY,
	DYNAMIC_RIGID_BODY,
};

enum class PhysicsShapeType
{
	BOX,
	PLANE,
	SPHERE,
	CAPSULE,
	CONVEXHULL,
	HEIGHTFIELD,
	TRIANGLEMESH,
};

enum class PhysicsLockAxisType
{
	LINEAR_X = 1 << 0,
	LINEAR_Y = 1 << 1,
	LINEAR_Z = 1 << 2,
	ANGULAR_X = 1 << 3,
	ANGULAR_Y = 1 << 4,
	ANGULAR_Z = 1 << 5,
};
DECL_FLAGS( PhysicsLockAxisType );

struct XE_API PhysicsWorldDesc
{
	XE::Vec3f Gravity;
	XE::AABB BoundBox;
	XE::Variant UserData;
	XE::Delegate< void( const XE::EventPtr & ) > EventCallback;
};

struct XE_API PhysicsShapeDesc
{
	XE::Variant UserData;
	PhysicsShapeType Type;
	XE::Mat4x4f LocalTransform;
	XE::PhysicsMaterialHandle Material;
	std::variant< std::monostate, XE::AABB, XE::Plane, XE::Sphere, XE::Capsule, XE::ConvexHull, XE::HeightField, XE::Mesh > Data;
};

struct XE_API PhysicsMaterialDesc
{
	XE::String Name;
	XE::float32 Restitution;
	XE::float32 StaticFriction;
	XE::float32 DynamicFriction;
};

struct XE_API PhysicsSoftBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsClothBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsFluidBodyDesc
{

	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mesh & ) > SyncMeshCallback;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsStaticRigidBodyDesc
{
	XE::Vec3f CMass;
	XE::float32 Mass;
	XE::float32 LinearDamping;
	XE::float32 AngularDamping;
	XE::Vec3f MaxLinearVelocity;
	XE::Vec3f MaxAngularVelocity;
};

struct XE_API PhysicsDynamicRigidBodyDesc : public PhysicsStaticRigidBodyDesc
{
	XE::uint32 IterationCount;
	XE::float32 SleepThreshold;
	XE::float32 StabilizationThreshold;
	XE::float32 ContactReportsThreshold;
	PhysicsLockAxisTypeFlags LockAxisFlags;
	XE::Delegate< void( XE::PhysicsBodyHandle, const XE::Mat4x4f & ) > SyncTransformCallback;
};

struct XE_API PhysicsBodyDesc
{
	XE::Layer Mask;
	XE::String Name;
	XE::Variant UserData;
	XE::PhysicsBodyType Type;
	XE::Mat4x4f WorldTransform;
	std::variant< std::monostate, XE::PhysicsSoftBodyDesc, XE::PhysicsClothBodyDesc, XE::PhysicsFluidBodyDesc, XE::PhysicsStaticRigidBodyDesc, XE::PhysicsDynamicRigidBodyDesc > Data;
};

struct XE_API PhysicsConstraintDesc
{
	XE::Vec3f Local0, Local1;
	XE::PhysicsBodyHandle Body0, Body1;

	XE::AxisType DriveLockAxis;
	XE::float32 DriveStiffness;
	XE::float32 DriveDamping;
	XE::float32 DriveMaxForce;
	bool DriveIsAcceleration;

	XE::AxisType LinearLockAxis;
	XE::Pair< XE::float32, XE::float32 > LinearXLimit;
	XE::Pair< XE::float32, XE::float32 > LinearYLimit;
	XE::Pair< XE::float32, XE::float32 > LinearZLimit;

	XE::AxisType AngularLockAxis;
	XE::Pair< XE::float32, XE::float32 > AngularXLimit;
	XE::Pair< XE::float32, XE::float32 > AngularYLimit;
	XE::Pair< XE::float32, XE::float32 > AngularZLimit;
};

struct XE_API PhysicsRaycastHit
{
	XE::Vec3f Normal;
	XE::Vec3f Position;
	XE::float32 Distance;
	XE::PhysicsBodyHandle Body;
	XE::PhysicsShapeHandle Shape;
};


enum class PhysicsCommandType
{
	CREATE_WORLD,
	CREATE_BODY,
	CREATE_SHAPE,
	CREATE_MATERIAL,
	CREATE_CONSTRAINT,

	SET_GRAVITY,
	SET_WORLD_TRANSFORM,
	SET_LOCAL_TRANSFROM,
	SET_LINEAR_DAMPING,
	SET_ANGULAR_DAMPING,
	SET_LINEAR_VELOCITY,
	SET_ANGULAR_VELOCITY,

	ATTACH_SHAPE,
	DETACH_SHAPE,

	ADD_FORCE,
	ADD_TORQUE,
	CLEAR_FORCE,
	CLEAR_TORQUE,
	SLEEP,
	WAKEUP,

	DESTROY_CONSTRAINT,
	DESTROY_MATERIAL,
	DESTROY_SHAPE,
	DESTROY_BODY,
	DESTROY_WORLD,

	END,
};

struct XE_API PhysicsItem
{
	XE::PhysicsCommandType Type;
	XE::uint64 Handle;
	XE::uint64 Code;
	XE::uint64 Size;
};

struct XE_API PhysicsFrame
{
public:
	void Reset()
	{
		CommandSize = 0;
		Buffers.unsafe().Clear();
	}

	std::mutex Mutex;
	std::atomic< XE::uint64 > CommandSize = 0;
	XE::Concurrent< XE::Buffer, std::mutex > Buffers;
	std::array< XE::PhysicsItem, PHYSICS_MAX_COMMAND > Command;
};

END_XE_NAMESPACE

#endif//TYPE_H__571C57BF_5056_4C91_8695_D95E7A44A3D7

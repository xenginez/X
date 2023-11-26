/*!
 * \file	PhysicsService.h
 *
 * \author	ZhengYuanQing
 * \date	2021/12/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PHYSICSSERVICE_H__ADE17909_C884_4703_AC6A_C27AD29B171E
#define PHYSICSSERVICE_H__ADE17909_C884_4703_AC6A_C27AD29B171E

#include "Core/Service.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API PhysicsService : public XE::Service
{
	OBJECT( PhysicsService, Service )

private:
	struct Private;

public:
	PhysicsService();

	~PhysicsService();

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

protected:
	virtual void OnSimulate( XE::PhysicsFrame * frame ) = 0;

public:
	XE::PhysicsWorldHandle Create( const XE::PhysicsWorldDesc & desc );

	XE::PhysicsBodyHandle Create( const XE::PhysicsBodyDesc & desc );

	XE::PhysicsShapeHandle Create( const XE::PhysicsShapeDesc & desc );

	XE::PhysicsMaterialHandle Create( const XE::PhysicsMaterialDesc & desc );

	XE::PhysicsConstraintHandle Create( const XE::PhysicsConstraintDesc & desc );

public:
	const XE::PhysicsWorldDesc & GetDesc( XE::PhysicsWorldHandle handle ) const;

	const XE::PhysicsBodyDesc & GetDesc( XE::PhysicsBodyHandle handle ) const;

	const XE::PhysicsShapeDesc & GetDesc( XE::PhysicsShapeHandle handle ) const;

	const XE::PhysicsMaterialDesc & GetDesc( XE::PhysicsMaterialHandle handle ) const;

	const XE::PhysicsConstraintDesc & GetDesc( XE::PhysicsConstraintHandle handle ) const;

public:
	XE::PhysicsWorldHandle GetPhysicsWorld( XE::PhysicsBodyHandle handle ) const;

	XE::PhysicsBodyHandle GetPhysicsBody( XE::PhysicsShapeHandle handle ) const;

public:
	void Destroy( XE::PhysicsWorldHandle handle );

	void Destroy( XE::PhysicsBodyHandle handle );

	void Destroy( XE::PhysicsShapeHandle handle );

	void Destroy( XE::PhysicsMaterialHandle handle );

	void Destroy( XE::PhysicsConstraintHandle handle );

public:
	void SetGravity( XE::PhysicsWorldHandle handle, const XE::Vec3f & gravity );

	virtual bool Raycast( XE::PhysicsWorldHandle handle, const XE::Ray & ray, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::AABB & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::Plane & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::Sphere & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::Capsule & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::ConvexHull & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Sweep( XE::PhysicsWorldHandle handle, const XE::Mesh & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::AABB & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::Plane & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::Sphere & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::Capsule & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::ConvexHull & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

	virtual bool Overlap( XE::PhysicsWorldHandle handle, const XE::Mesh & geometry, const XE::Mat4x4f & transform, const XE::Vec3f & dir, XE::float32 distance, const XE::Layer & mask, XE::PhysicsRaycastHit & hit ) = 0;

public:
	void AttachShape( XE::PhysicsBodyHandle handle, XE::PhysicsShapeHandle shape );

	void DetachShape( XE::PhysicsBodyHandle handle, XE::PhysicsShapeHandle shape );

	const XE::Array< XE::PhysicsShapeHandle > & GetShapes( XE::PhysicsBodyHandle handle );

	void SetWorldTransform( XE::PhysicsBodyHandle handle, const XE::Mat4x4f & transform );

public:
	void AddForce( XE::PhysicsBodyHandle handle, const XE::Vec3f & force );

	void AddTorque( XE::PhysicsBodyHandle handle, const XE::Vec3f & torque );

	void AddForceAndTorque( XE::PhysicsBodyHandle handle, const XE::Vec3f & force, const XE::Vec3f & torque );

	void ClearForce( XE::PhysicsBodyHandle handle );

	void ClearTorque( XE::PhysicsBodyHandle handle );

	void SetLinearDamping( XE::PhysicsBodyHandle handle, XE::float32 damping );

	void SetAngularDamping( XE::PhysicsBodyHandle handle, XE::float32 damping );

	void SetLinearVelocity( XE::PhysicsBodyHandle handle, const XE::Vec3f & velocity );

	void SetAngularVelocity( XE::PhysicsBodyHandle handle, const XE::Vec3f & velocity );

public:
	void Sleep( XE::PhysicsBodyHandle handle );

	void Wakeup( XE::PhysicsBodyHandle handle );

	virtual bool IsSleeping( XE::PhysicsBodyHandle handle ) const = 0;

public:
	void SetLocalTransfrom( XE::PhysicsShapeHandle handle, const XE::Mat4x4f & transform );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//PHYSICSSERVICE_H__ADE17909_C884_4703_AC6A_C27AD29B171E

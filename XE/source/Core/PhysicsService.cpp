#include "PhysicsService.h"

#include "Utils/RefCount.h"

#include "CoreFramework.h"
#include "ThreadService.h"
#include "TimerService.h"

IMPLEMENT_META( XE::PhysicsService );

BEG_XE_NAMESPACE

struct PHIWorld : public XE::RefCount
{
	XE::PhysicsWorldDesc Desc;
	XE::PhysicsWorldHandle Handle;
};

struct PHIBody : public XE::RefCount
{
	XE::PhysicsBodyDesc Desc;
	XE::PhysicsWorldHandle World;
	XE::PhysicsBodyHandle Handle;
	XE::Array< XE::PhysicsShapeHandle > Shapes;
	XE::Array< XE::PhysicsConstraintHandle > Constraints;
};

struct PHIShape : public XE::RefCount
{
	XE::PhysicsBodyHandle Body;
	XE::PhysicsShapeDesc Desc;
	XE::PhysicsShapeHandle Handle;
};

struct PHIMaterial : public XE::RefCount
{
	XE::PhysicsMaterialDesc Desc;
	XE::PhysicsMaterialHandle Handle;
};

struct PHIConstraint : public XE::RefCount
{
	XE::PhysicsConstraintDesc Desc;
	XE::PhysicsConstraintHandle Handle;
};

END_XE_NAMESPACE

struct XE::PhysicsService::Private
{
	XE::float32 _Dutation = 0;

	std::mutex _FrameMutex;
	XE::uint64 _SubmitFrame = 0;
	std::array< XE::PhysicsFrame, XE::PHYSICS_MAX_FRAME > _Frames;

	std::array< XE::PHIWorld, PHYSICS_MAX_WORLD > _Worlds;
	std::array< XE::PHIBody, PHYSICS_MAX_BODY > _Bodys;
	std::array< XE::PHIShape, PHYSICS_MAX_SHAPE > _Shapes;
	std::array< XE::PHIMaterial, PHYSICS_MAX_MATERIAL > _Materials;
	std::array< XE::PHIConstraint, PHYSICS_MAX_CONSTRAINT > _Constraints;

	XE::QueueHandleAllocator< XE::PhysicsWorldHandle, PHYSICS_MAX_WORLD > _WorldHandleAlloc;
	XE::QueueHandleAllocator< XE::PhysicsBodyHandle, PHYSICS_MAX_BODY > _BodyHandleAlloc;
	XE::QueueHandleAllocator< XE::PhysicsShapeHandle, PHYSICS_MAX_SHAPE > _ShapeHandleAlloc;
	XE::QueueHandleAllocator< XE::PhysicsMaterialHandle, PHYSICS_MAX_MATERIAL > _MaterialHandleAlloc;
	XE::QueueHandleAllocator< XE::PhysicsConstraintHandle, PHYSICS_MAX_CONSTRAINT > _ConstraintHandleAlloc;
};

XE::PhysicsService::PhysicsService()
	:_p( XE::New< Private >() )
{

}

XE::PhysicsService::~PhysicsService()
{
	XE::Delete( _p );
}

void XE::PhysicsService::Prepare()
{

}

void XE::PhysicsService::Startup()
{

}

void XE::PhysicsService::Update()
{
	auto timer = GetFramework()->GetServiceT< XE::TimerService >();
	auto thread = GetFramework()->GetServiceT< XE::ThreadService >();

	_p->_Dutation += timer->GetDeltaTime();
	if( _p->_Dutation >= timer->GetFixedDeltaTime() )
	{
		_p->_Dutation = 0;

		XE::uint64 frame = _p->_SubmitFrame;

		{
			XE::SharedPtr< std::unique_lock< std::mutex > >lock = XE::MakeShared< std::unique_lock< std::mutex > >( _p->_Frames[frame].Mutex );

			thread->PostTask( XE::ThreadType::PHYSICS, [this, frame, lock]()
			{
				OnSimulate( &_p->_Frames[frame] );
			} );
		}

		{
			std::unique_lock< std::mutex > lock( _p->_Frames[( frame + 1 ) % PHYSICS_MAX_FRAME].Mutex );

			_p->_SubmitFrame = ( frame + 1 ) % PHYSICS_MAX_FRAME;

			_p->_Frames[_p->_SubmitFrame].Reset();
		}
	}
}

void XE::PhysicsService::Clearup()
{
	for( auto & it : _p->_Frames )
	{
		std::unique_lock< std::mutex > lock( it.Mutex );

		it.Reset();
	}

	_p->_SubmitFrame = 0;
	_p->_Dutation = 0;
}

XE::PhysicsWorldHandle XE::PhysicsService::Create( const XE::PhysicsWorldDesc & desc )
{
	auto handle = _p->_WorldHandleAlloc.Alloc();

	_p->_Worlds[handle.GetValue()].Handle = handle;
	_p->_Worlds[handle.GetValue()].Desc = desc;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CREATE_WORLD;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Worlds[handle.GetValue()].Inc();

	return handle;
}

XE::PhysicsBodyHandle XE::PhysicsService::Create( const XE::PhysicsBodyDesc & desc )
{
	auto handle = _p->_BodyHandleAlloc.Alloc();

	_p->_Bodys[handle.GetValue()].Handle = handle;
	_p->_Bodys[handle.GetValue()].Desc = desc;

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CREATE_BODY;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Bodys[handle.GetValue()].Inc();

	return handle;
}

XE::PhysicsShapeHandle XE::PhysicsService::Create( const XE::PhysicsShapeDesc & desc )
{
	auto handle = _p->_ShapeHandleAlloc.Alloc();

	_p->_Shapes[handle.GetValue()].Handle = handle;
	_p->_Shapes[handle.GetValue()].Desc = desc;

	if( desc.Material )
	{
		_p->_Materials[desc.Material].Inc();
	}

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CREATE_SHAPE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Shapes[handle.GetValue()].Inc();

	return handle;
}

XE::PhysicsMaterialHandle XE::PhysicsService::Create( const XE::PhysicsMaterialDesc & desc )
{
	auto it = std::find_if( _p->_Materials.begin(), _p->_Materials.end(), [&desc]( const XE::PHIMaterial & val ) { return desc.Name == val.Desc.Name; } );
	auto handle = it != _p->_Materials.end() ? it->Handle : XE::PhysicsMaterialHandle();

	if( !handle )
	{
		handle = _p->_MaterialHandleAlloc.Alloc();

		_p->_Materials[handle.GetValue()].Handle = handle;
		_p->_Materials[handle.GetValue()].Desc = desc;

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CREATE_MATERIAL;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	}

	_p->_Materials[handle.GetValue()].Inc();

	return handle;
}

XE::PhysicsConstraintHandle XE::PhysicsService::Create( const XE::PhysicsConstraintDesc & desc )
{
	auto handle = _p->_ConstraintHandleAlloc.Alloc();

	_p->_Constraints[handle.GetValue()].Handle = handle;
	_p->_Constraints[handle.GetValue()].Desc = desc;

	if( desc.Body0 )
	{
		_p->_Bodys[desc.Body0].Inc();
	}
	if( desc.Body1 )
	{
		_p->_Bodys[desc.Body1].Inc();
	}

	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CREATE_CONSTRAINT;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Constraints[handle.GetValue()].Inc();

	return handle;
}

const XE::PhysicsWorldDesc & XE::PhysicsService::GetDesc( XE::PhysicsWorldHandle handle ) const
{
	return _p->_Worlds[handle.GetValue()].Desc;
}

const XE::PhysicsBodyDesc & XE::PhysicsService::GetDesc( XE::PhysicsBodyHandle handle ) const
{
	return _p->_Bodys[handle.GetValue()].Desc;
}

const XE::PhysicsShapeDesc & XE::PhysicsService::GetDesc( XE::PhysicsShapeHandle handle ) const
{
	return _p->_Shapes[handle.GetValue()].Desc;
}

const XE::PhysicsMaterialDesc & XE::PhysicsService::GetDesc( XE::PhysicsMaterialHandle handle ) const
{
	return _p->_Materials[handle.GetValue()].Desc;
}

const XE::PhysicsConstraintDesc & XE::PhysicsService::GetDesc( XE::PhysicsConstraintHandle handle ) const
{
	return _p->_Constraints[handle.GetValue()].Desc;
}

XE::PhysicsWorldHandle XE::PhysicsService::GetPhysicsWorld( XE::PhysicsBodyHandle handle ) const
{
	return _p->_Bodys[handle.GetValue()].World;
}

XE::PhysicsBodyHandle XE::PhysicsService::GetPhysicsBody( XE::PhysicsShapeHandle handle ) const
{
	return _p->_Shapes[handle.GetValue()].Body;
}

void XE::PhysicsService::Destroy( XE::PhysicsWorldHandle handle )
{
	if( _p->_Worlds[handle.GetValue()].Dec() == 0 )
	{
		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DESTROY_WORLD;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_WorldHandleAlloc.Free( handle );
		_p->_Worlds[handle.GetValue()].Reset();
	}
}

void XE::PhysicsService::Destroy( XE::PhysicsBodyHandle handle )
{
	if( _p->_Bodys[handle.GetValue()].Dec() == 0 )
	{
		for( auto shape : _p->_Bodys[handle.GetValue()].Shapes )
		{
			if( shape )
			{
				Destroy( shape );
			}
		}
		for( auto constraint : _p->_Bodys[handle.GetValue()].Constraints )
		{
			if( constraint )
			{
				Destroy( constraint );
			}
		}

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DESTROY_BODY;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_BodyHandleAlloc.Free( handle );
		_p->_Bodys[handle.GetValue()].Reset();
	}
}

void XE::PhysicsService::Destroy( XE::PhysicsShapeHandle handle )
{
	if( _p->_Shapes[handle.GetValue()].Dec() == 0 )
	{
		if( _p->_Shapes[handle.GetValue()].Body )
		{
			DetachShape( _p->_Shapes[handle.GetValue()].Body, handle );
		}
		if( _p->_Shapes[handle.GetValue()].Desc.Material )
		{
			Destroy( _p->_Shapes[handle.GetValue()].Desc.Material );
		}

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DESTROY_SHAPE;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_ShapeHandleAlloc.Free( handle );
		_p->_Shapes[handle.GetValue()].Reset();
	}
}

void XE::PhysicsService::Destroy( XE::PhysicsMaterialHandle handle )
{
	if( _p->_Materials[handle.GetValue()].Dec() == 0 )
	{
		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DESTROY_MATERIAL;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_MaterialHandleAlloc.Free( handle );
		_p->_Materials[handle.GetValue()].Reset();
	}
}

void XE::PhysicsService::Destroy( XE::PhysicsConstraintHandle handle )
{
	if( _p->_Constraints[handle.GetValue()].Dec() == 0 )
	{
		if( _p->_Constraints[handle.GetValue()].Desc.Body0 )
		{
			Destroy( _p->_Constraints[handle.GetValue()].Desc.Body0 );
		}
		if( _p->_Constraints[handle.GetValue()].Desc.Body1 )
		{
			Destroy( _p->_Constraints[handle.GetValue()].Desc.Body1 );
		}

		auto frame = _p->_SubmitFrame;
		auto index = _p->_Frames[frame].CommandSize++;

		_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DESTROY_CONSTRAINT;
		_p->_Frames[frame].Command[index].Handle = handle.GetValue();

		_p->_ConstraintHandleAlloc.Free( handle );
		_p->_Constraints[handle.GetValue()].Reset();
	}
}

void XE::PhysicsService::SetGravity( XE::PhysicsWorldHandle handle, const XE::Vec3f & gravity )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_GRAVITY;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( gravity );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( gravity );
	} );
}

void XE::PhysicsService::AttachShape( XE::PhysicsBodyHandle handle, XE::PhysicsShapeHandle shape )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::ATTACH_SHAPE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = shape.GetValue();
}

void XE::PhysicsService::DetachShape( XE::PhysicsBodyHandle handle, XE::PhysicsShapeHandle shape )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::DETACH_SHAPE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = shape.GetValue();
}

const XE::Array< XE::PhysicsShapeHandle > & XE::PhysicsService::GetShapes( XE::PhysicsBodyHandle handle )
{
	return _p->_Bodys[handle.GetValue()].Shapes;
}

void XE::PhysicsService::SetWorldTransform( XE::PhysicsBodyHandle handle, const XE::Mat4x4f & transform )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_WORLD_TRANSFORM;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( transform );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( transform );
	} );
}

void XE::PhysicsService::AddForce( XE::PhysicsBodyHandle handle, const XE::Vec3f & force )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::ADD_FORCE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( force );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( force );
	} );
}

void XE::PhysicsService::AddTorque( XE::PhysicsBodyHandle handle, const XE::Vec3f & torque )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::ADD_TORQUE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( torque );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( torque );
	} );
}

void XE::PhysicsService::AddForceAndTorque( XE::PhysicsBodyHandle handle, const XE::Vec3f & force, const XE::Vec3f & torque )
{
	AddForce( handle, force );
	AddTorque( handle, torque );
}

void XE::PhysicsService::ClearForce( XE::PhysicsBodyHandle handle )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CLEAR_FORCE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::PhysicsService::ClearTorque( XE::PhysicsBodyHandle handle )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::CLEAR_TORQUE;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::PhysicsService::SetLinearDamping( XE::PhysicsBodyHandle handle, XE::float32 damping )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_LINEAR_DAMPING;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( damping );
}

void XE::PhysicsService::SetAngularDamping( XE::PhysicsBodyHandle handle, XE::float32 damping )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_ANGULAR_DAMPING;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Code = XE::BitCast< XE::uint64 >( damping );
}

void XE::PhysicsService::SetLinearVelocity( XE::PhysicsBodyHandle handle, const XE::Vec3f & velocity )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_LINEAR_VELOCITY;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( velocity );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( velocity );
	} );
}

void XE::PhysicsService::SetAngularVelocity( XE::PhysicsBodyHandle handle, const XE::Vec3f & velocity )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_ANGULAR_VELOCITY;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( velocity );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( velocity );
	} );
}

void XE::PhysicsService::Sleep( XE::PhysicsBodyHandle handle )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SLEEP;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::PhysicsService::Wakeup( XE::PhysicsBodyHandle handle )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::WAKEUP;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
}

void XE::PhysicsService::SetLocalTransfrom( XE::PhysicsShapeHandle handle, const XE::Mat4x4f & transform )
{
	auto frame = _p->_SubmitFrame;
	auto index = _p->_Frames[frame].CommandSize++;

	_p->_Frames[frame].Command[index].Type = XE::PhysicsCommandType::SET_LOCAL_TRANSFROM;
	_p->_Frames[frame].Command[index].Handle = handle.GetValue();
	_p->_Frames[frame].Command[index].Size = sizeof( transform );

	_p->_Frames[frame].Buffers( [&]( XE::Buffer & buffer )
	{
		_p->_Frames[frame].Command[index].Code = buffer.WirtePos();
		buffer.Wirte( transform );
	} );
}

#include "Level.h"

#include "Utils/Logger.h"
#include "Core/ThreadService.h"

#include "Entity.h"
#include "GameObject.h"

BEG_META( XE::Level )
END_META()

BEG_XE_NAMESPACE

XE_INLINE constexpr XE::uint64 Hash( const XE::TemplateType & val )
{
	XE::uint64 value = 0;

	for ( const auto & it : val )
	{
		value ^= it->GetHashCode();
	}

	return value;
}

END_XE_NAMESPACE

namespace
{
	using Chunk = std::array< XE::uint8, KBYTE( 16 ) >;

	struct Archetype
	{
		XE::Bitmap _Bits;
		XE::List< Chunk > Chunks;
		XE::uint64 ComponentsSize;
		XE::TemplateType ComponentsMeta;
		XE::UnorderedMap< XE::EntityHandle, XE::uint64 > EntityIndexMap;
	};
}

struct XE::Level::Private
{
	bool _Enable = true;
	XE::Transform _Transform;
	XE::Array< XE::GameObjectPtr > _GameObjects;

	XE::Graph< XE::EntitySystemPtr > _Systems;
	XE::UnorderedMap< XE::uint64, Archetype > _ArchetypeHashMap;
	XE::UnorderedMap< XE::EntityHandle, XE::uint64 > _EntityHashMap;
	XE::QueueHandleAllocator< XE::EntityHandle > _EntityHandleAllocator;

	XE::ConcurrentQueue< XE::Delegate< void() > > _TaskQueue;
};

XE::Level::Level()
	:_p( XE::New< Private >() )
{

}

XE::Level::~Level()
{
	XE::Delete( _p );
}

void XE::Level::Startup()
{
	for ( size_t i = 0; i < _p->_GameObjects.size(); i++ )
	{
		_p->_GameObjects[i]->_Level = XE_THIS( XE::Level );
		_p->_GameObjects[i]->_Transform.SetParent( &_p->_Transform );

		if ( _p->_GameObjects[i]->GetEnable() )
		{
			_p->_GameObjects[i]->Startup();
		}
	}
}

void XE::Level::Update( XE::float32 dt )
{
	XE::Delegate< void() > task;
	while ( _p->_TaskQueue.try_pop( task ) )
	{
		task();
	}

	for ( auto & it : _p->_GameObjects )
	{
		if ( it->GetEnable() )
		{
			if ( it->_Start )
			{
				it->Update( dt );
			}
			else
			{
				it->Startup();
			}
		}
	}

	{

	}

	while ( _p->_TaskQueue.try_pop( task ) )
	{
		task();
	}
}

void XE::Level::Clearup()
{
	for ( auto & it : _p->_GameObjects )
	{
		if ( it->_Start )
		{
			it->Clearup();
		}
	}
}

bool XE::Level::GetEnable() const
{
	return _p->_Enable;
}

void XE::Level::SetEnable( bool val )
{
	_p->_Enable = val;
}

XE::Transform & XE::Level::GetTransform()
{
	return _p->_Transform;
}

XE::GameObjectPtr XE::Level::GetGameObject( const XE::String & val )
{
	if ( val.empty() )
	{
		return nullptr;
	}

	auto pos = val.find( "/" );
	if ( pos == XE::String::npos )
	{
		auto it = std::find_if( _p->_GameObjects.begin(), _p->_GameObjects.end(), [&]( const XE::GameObjectPtr & obj ) { return obj->GetName() == val; } );
		return it != _p->_GameObjects.end() ? *it : nullptr;
	}

	XE::String name( val.begin(), val.begin() + pos );
	auto it = std::find_if( _p->_GameObjects.begin(), _p->_GameObjects.end(), [&]( const XE::GameObjectPtr & obj ) { return obj->GetName() == name; } );
	return it != _p->_GameObjects.end() ? ( *it )->GetGameObjectFromChild( { val.begin() + pos + 1, val.end() } ) : nullptr;
}

XE::GameObjectPtr XE::Level::AddGameObject( const XE::String & name, const XE::GameObjectPtr & parent /*= nullptr*/ )
{
	XE::GameObjectPtr obj = XE::MakeShared< XE::GameObject >();

	obj->_Name = name;
	obj->_Level = XE_THIS( XE::Level );

	if ( parent )
	{
		obj->SetParent( parent );
	}
	else
	{
		obj->_Transform.SetParent( &_p->_Transform );
	}

	_p->_GameObjects.push_back( obj );

	return obj;
}

XE::EntityHandle XE::Level::AddEntity( const XE::TemplateType & components )
{
	return {};
}

bool XE::Level::RemoveEntity( const XE::EntityHandle & val )
{
	return false;
}

bool XE::Level::AddSystem( const XE::TemplateType & val )
{
	return true;
}

bool XE::Level::RemoveSystem( const XE::TemplateType & val )
{
	return true;
}

bool XE::Level::AddComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component )
{
	return false;
}

bool XE::Level::RemoveComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component )
{
	return false;
}

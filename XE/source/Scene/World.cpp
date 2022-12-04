#include "World.h"

#include "Utils/Logger.h"

#include "Entity.h"
#include "GameObject.h"

BEG_META( XE::World )
END_META()

BEG_XE_NAMESPACE

XE_INLINE XE::uint64 Hash( const XE::TemplateType & val )
{
	XE::uint64 value = 0;

	for ( const auto & it : val )
	{
		value ^= it->GetHashCode();
	}

	return value;
}

class Entity
{
public:
	Entity()
	{

	}

	Entity( const Entity & val )
	{

	}

public:
	XE::uint64 Hash;
	XE::EntityHandle Handle;
	XE::Array< XE::Variant > Components;
};
DECL_PTR( Entity );
DECL_XE_CLASS( Entity );

END_XE_NAMESPACE

BEG_META( XE::Entity )
type->Property( "Handle", &XE::Entity::Handle );
type->Property( "Components", &XE::Entity::Components );
END_META()

namespace
{
	static constexpr XE::uint64 ChunkSize = KBYTE( 16 );

	using HashID = XE::uint64;

	struct Chunk
	{
		XE::Bitmap Bits;
		XE::uint8 Data[ChunkSize];
		//////////////////////////////////////////////////////////////////////////
		// 0x00000000 ComponentA, ComponentA, ComponentA, ComponentA, ComponentA
		// 0x00000005 ComponentB, ComponentB, ComponentB, ComponentB, ComponentB
		// 0x0000000A ComponentC, ComponentC, ComponentC, ComponentC, ComponentC
		// 0x0000000F ComponentD, ComponentD, ComponentD, ComponentD, ComponentD
		// 0x00000014 ...
		//////////////////////////////////////////////////////////////////////////
	};
	using ChunkList = XE::List< Chunk * >;

	struct IndexID
	{
		XE::uint64 Index;
		ChunkList::iterator Data;
	};

	struct Archetype
	{
		ChunkList Chunks;
		XE::uint64 ArchetypeSize;
		XE::TemplateType ComponentTypes;
		XE::Map< XE::EntityHandle, IndexID > EntityIndexMap;
	};

	XE::uint8 * CalcComponentPtr( XE::uint8 * data, XE::uint64 offset, XE::uint64 index, XE::uint64 count, XE::uint64 size )
	{
		return data + ( offset * count ) + ( index * size );
	}
}

struct XE::World::Private
{
	bool _Enable = true;
	XE::String _Name;
	XE::Transform _Transform;
	XE::Array< XE::GameObjectPtr > _GameObjects;

	XE::EntitySystemGroup _SystemGroup;
	XE::Map< HashID, Archetype > _ArchetypeMap;
	XE::Map< XE::EntityHandle, HashID > _EntityMap;
	XE::EntityHandleAllocator _EntityHandleAllocator;

	ChunkList _FreeChunks;
	XE::ConcurrentQueue< XE::Delegate< void() > > _TaskQueue;
};

XE::World::World()
	:_p( XE::New< Private >() )
{

}

XE::World::~World()
{
	XE::Delete( _p );
}

void XE::World::Startup()
{
	for ( size_t i = 0; i < _p->_GameObjects.size(); i++ )
	{
		_p->_GameObjects[i]->_World = XE_THIS( XE::World );
		_p->_GameObjects[i]->_Transform.SetParent( &_p->_Transform );

		if ( _p->_GameObjects[i]->GetEnable() )
		{
			_p->_GameObjects[i]->Startup();
		}
	}
}

void XE::World::Update( XE::float32 dt )
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

	_p->_SystemGroup.Execute( this, dt );

	while ( _p->_TaskQueue.try_pop( task ) )
	{
		task();
	}

	if ( _p->_FreeChunks.size() > 2 )
	{
		XE::Delete( _p->_FreeChunks.back() );
		_p->_FreeChunks.pop_back();
	}
}

void XE::World::Clearup()
{
	XE::Delegate< void() > task;

	while ( _p->_TaskQueue.try_pop( task ) )
	{
		task();
	}

	for ( auto & it : _p->_GameObjects )
	{
		if ( it->_Start )
		{
			it->Clearup();
		}
	}

	while ( _p->_TaskQueue.try_pop( task ) )
	{
		task();
	}

	_p->_GameObjects.clear();

	_p->_EntityMap.clear();
	_p->_ArchetypeMap.clear();
	_p->_EntityHandleAllocator.Reset();

	_p->_FreeChunks.clear();
}

bool XE::World::GetEnable() const
{
	return _p->_Enable;
}

void XE::World::SetEnable( bool val )
{
	_p->_Enable = val;
}

const XE::String & XE::World::GetName() const
{
	return _p->_Name;
}

void XE::World::SetName( const XE::String & val )
{
	_p->_Name = val;
}

XE::Transform & XE::World::GetTransform()
{
	return _p->_Transform;
}

XE::GameObjectPtr XE::World::GetGameObject( const XE::String & val )
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

XE::GameObjectPtr XE::World::AddGameObject( const XE::String & name, const XE::GameObjectPtr & parent /*= nullptr*/ )
{
	XE::GameObjectPtr obj = XE::MakeShared< XE::GameObject >();

	obj->_Name = name;
	obj->_World = XE_THIS( XE::World );

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

void XE::World::SetSystemGraph( const XE::EntitySystemGraph & val )
{
	_p->_SystemGroup._Graph = val;
}

XE::EntityHandle XE::World::AddEntity( const XE::TemplateType & components )
{
	IndexID index;
	HashID hash = XE::Hash( components );
	XE::EntityHandle handle = _p->_EntityHandleAllocator.Alloc();

	auto arche_it = _p->_ArchetypeMap.find( hash );
	if ( arche_it != _p->_ArchetypeMap.end() )
	{
		for ( auto it = arche_it->second.Chunks.begin(); it != arche_it->second.Chunks.end(); ++it )
		{
			if ( !( *it )->Bits.All() )
			{
				index.Data = it;
				index.Index = ( *it )->Bits.FindFirstFalse();

				( *it )->Bits.Set( index.Index, true );

				break;
			}
		}
	}
	else
	{
		arche_it = _p->_ArchetypeMap.insert( { hash, {} } ).first;

		arche_it->second.ComponentTypes = components;
		for ( const auto & it : components )
		{
			arche_it->second.ArchetypeSize += it->GetSize();
		}

		Chunk * chunk = nullptr;
		if ( !_p->_FreeChunks.empty() )
		{
			chunk = std::move( _p->_FreeChunks.front() );
			_p->_FreeChunks.pop_front();
		}
		else
		{
			chunk = XE::New< Chunk >();
			chunk->Bits.Reset( KBYTE( 16 ) / arche_it->second.ArchetypeSize );
		}

		arche_it->second.Chunks.emplace_back( std::move( chunk ) );

		index.Data = arche_it->second.Chunks.begin();
		index.Index = 0;

		( *index.Data )->Bits.Set( 0, true );
	}

	XE::uint64 offset = 0;
	for ( const auto & comp : components )
	{
		if ( auto cls = SP_CAST< const XE::MetaClass >( comp ) )
		{
			cls->Construct( CalcComponentPtr( ( *index.Data )->Data, offset, index.Index, ( *index.Data )->Bits.Count(), cls->GetSize() ) );
		}

		offset += comp->GetSize();
	}

	arche_it->second.EntityIndexMap.insert( { handle, index } );

	_p->_EntityMap.insert( { handle, hash } );

	return handle;
}

void XE::World::RemoveEntity( const XE::EntityHandle & val )
{
	auto entity_it = _p->_EntityMap.find( val );
	if ( entity_it != _p->_EntityMap.end() )
	{
		auto arche_it = _p->_ArchetypeMap.find( entity_it->second );
		if ( arche_it != _p->_ArchetypeMap.end() )
		{
			auto index_it = arche_it->second.EntityIndexMap.find( val );
			if ( index_it != arche_it->second.EntityIndexMap.end() )
			{
				XE::uint64 offset = 0;
				for ( const auto & type : arche_it->second.ComponentTypes )
				{
					if ( auto cls = SP_CAST< const XE::MetaClass >( type ) )
					{
						auto index = index_it->second;
						cls->Destruct( CalcComponentPtr( ( *index.Data )->Data, offset, index.Index, ( *index.Data )->Bits.Count(), cls->GetSize() ) );
						offset += cls->GetSize();
					}
				}

				( *index_it->second.Data )->Bits.Set( index_it->second.Index, false );
				if ( ( *index_it->second.Data )->Bits.None() )
				{
					_p->_FreeChunks.emplace_back( std::move( ( *index_it->second.Data ) ) );
					arche_it->second.Chunks.erase( index_it->second.Data );
				}

				arche_it->second.EntityIndexMap.erase( index_it );
			}
		}
	}
}

void XE::World::AddComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component )
{
	HashID hash;
	IndexID old_index, new_index;
	XE::Map< HashID, Archetype >::iterator old_it, new_it;

	auto entity_it = _p->_EntityMap.find( handle );
	if ( entity_it != _p->_EntityMap.end() )
	{
		old_it = _p->_ArchetypeMap.find( entity_it->second );
		if ( old_it == _p->_ArchetypeMap.end() )
		{
			XE_ERROR( "the archetype of {%1} was not found.", handle.GetValue() );

			return;
		}
		if ( std::find( old_it->second.ComponentTypes.begin(), old_it->second.ComponentTypes.end(), component ) != old_it->second.ComponentTypes.end() )
		{
			XE_ERROR( "Type {%1} already exists", component->GetFullName() );

			return;
		}
		old_index = old_it->second.EntityIndexMap[handle];

		auto components = old_it->second.ComponentTypes; components.push_back( component );
		hash = XE::Hash( components );

		new_it = _p->_ArchetypeMap.find( hash );
		if ( new_it == _p->_ArchetypeMap.end() )
		{
			new_it = _p->_ArchetypeMap.insert( { hash, {} } ).first;

			new_it->second.ComponentTypes = components;
			for ( const auto & it : components )
			{
				new_it->second.ArchetypeSize += it->GetSize();
			}

			Chunk * chunk = nullptr;
			if ( _p->_FreeChunks.empty() )
			{
				chunk = XE::New< Chunk >();
			}
			else
			{
				chunk = std::move( _p->_FreeChunks.front() );
				_p->_FreeChunks.pop_front();
			}
			chunk->Bits.Reset( KBYTE( 16 ) / new_it->second.ArchetypeSize );

			new_it->second.Chunks.emplace_back( std::move( chunk ) );
		}

		for ( auto it = new_it->second.Chunks.begin(); it != new_it->second.Chunks.end(); ++it )
		{
			if ( !( *it )->Bits.All() )
			{
				new_index.Data = it;
				new_index.Index = ( *it )->Bits.FindFirstFalse();

				( *it )->Bits.Set( new_index.Index, true );

				break;
			}
		}

		XE::uint64 offset = 0;
		for ( const auto & it : old_it->second.ComponentTypes )
		{
			auto size = it->GetSize();
			auto old_p = CalcComponentPtr( ( *old_index.Data )->Data, offset, old_index.Index, ( *old_index.Data )->Bits.Count(), size );
			auto new_p = CalcComponentPtr( ( *new_index.Data )->Data, offset, new_index.Index, ( *new_index.Data )->Bits.Count(), size );
			std::copy( old_p, old_p + size, new_p );
			offset += size;
		}
		component->Construct( CalcComponentPtr( ( *new_index.Data )->Data, offset, new_index.Index, ( *new_index.Data )->Bits.Count(), component->GetSize() ) );

		( *old_index.Data )->Bits.Set( old_index.Index, false );
		if ( ( *old_index.Data )->Bits.None() )
		{
			_p->_FreeChunks.emplace_back( std::move( *old_index.Data ) );
			old_it->second.Chunks.erase( old_index.Data );
		}
		old_it->second.EntityIndexMap.erase( old_it->second.EntityIndexMap.find( handle ) );

		new_it->second.EntityIndexMap.insert( { handle, new_index } );

		entity_it->second = hash;
	}
}

void XE::World::RemoveComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component )
{
	HashID hash;
	IndexID old_index, new_index;
	XE::Map< HashID, Archetype >::iterator old_it, new_it;

	auto entity_it = _p->_EntityMap.find( handle );
	if ( entity_it != _p->_EntityMap.end() )
	{
		old_it = _p->_ArchetypeMap.find( entity_it->second );
		if ( old_it == _p->_ArchetypeMap.end() )
		{
			XE_ERROR( "the archetype of {%1} was not found.", handle.GetValue() );

			return;
		}
		old_index = old_it->second.EntityIndexMap[handle];

		auto & components = old_it->second.ComponentTypes;
		components.erase( std::find( components.begin(), components.end(), component ) );
		hash = XE::Hash( components );

		new_it = _p->_ArchetypeMap.find( hash );
		if ( new_it == _p->_ArchetypeMap.end() )
		{
			new_it = _p->_ArchetypeMap.insert( { hash, {} } ).first;

			new_it->second.ComponentTypes = components;
			for ( const auto & it : components )
			{
				new_it->second.ArchetypeSize += it->GetSize();
			}

			Chunk * chunk = nullptr;
			if ( _p->_FreeChunks.empty() )
			{
				chunk = XE::New< Chunk >();
			}
			else
			{
				chunk = std::move( _p->_FreeChunks.front() );
				_p->_FreeChunks.pop_front();
			}
			chunk->Bits.Reset( KBYTE( 16 ) / new_it->second.ArchetypeSize );

			new_it->second.Chunks.emplace_back( std::move( chunk ) );
		}

		for ( auto it = new_it->second.Chunks.begin(); it != new_it->second.Chunks.end(); ++it )
		{
			if ( !( *it )->Bits.All() )
			{
				new_index.Data = it;
				new_index.Index = ( *it )->Bits.FindFirstFalse();

				( *it )->Bits.Set( new_index.Index, true );

				break;
			}
		}

		XE::uint64 old_offset = 0, new_offset = 0;
		for ( const auto & it : old_it->second.ComponentTypes )
		{
			auto size = it->GetSize();
			if ( it != component )
			{
				auto old_p = CalcComponentPtr( ( *old_index.Data )->Data, old_offset, old_index.Index, ( *old_index.Data )->Bits.Count(), size );
				auto new_p = CalcComponentPtr( ( *new_index.Data )->Data, new_offset, new_index.Index, ( *new_index.Data )->Bits.Count(), size );
				std::copy( old_p, old_p + size, new_p );
				new_offset += size;
			}
			else
			{
				component->Destruct( CalcComponentPtr( ( *old_index.Data )->Data, old_offset, old_index.Index, ( *old_index.Data )->Bits.Count(), size ) );
			}
			old_offset += size;
		}

		( *old_index.Data )->Bits.Set( old_index.Index, false );
		if ( ( *old_index.Data )->Bits.None() )
		{
			_p->_FreeChunks.emplace_back( std::move( *old_index.Data ) );
			old_it->second.Chunks.erase( old_index.Data );
		}
		old_it->second.EntityIndexMap.erase( old_it->second.EntityIndexMap.find( handle ) );

		new_it->second.EntityIndexMap.insert( { handle, new_index } );

		entity_it->second = hash;
	}
}

XE::EntityComponent * XE::World::GetComponent( const XE::MetaClassCPtr & type, const XE::EntityHandle & handle ) const
{
	auto entity_it = _p->_EntityMap.find( handle );
	if ( entity_it != _p->_EntityMap.end() )
	{
		auto arche_it = _p->_ArchetypeMap.find( entity_it->second );
		if ( arche_it != _p->_ArchetypeMap.end() )
		{
			auto & arche_type = arche_it->second;

			auto idx_it = arche_type.EntityIndexMap.find( handle );
			if ( idx_it != arche_type.EntityIndexMap.end() )
			{
				auto comp_it = std::find( arche_type.ComponentTypes.begin(), arche_type.ComponentTypes.end(), type );
				if ( comp_it != arche_type.ComponentTypes.end() )
				{
					XE::uint64 offset = 0;

					for ( auto it = arche_type.ComponentTypes.begin(); it != comp_it; ++it )
					{
						offset += ( *it )->GetSize();
					}

					return reinterpret_cast<XE::EntityComponent *>( CalcComponentPtr( ( *idx_it->second.Data )->Data, offset, idx_it->second.Index, ( *idx_it->second.Data )->Bits.Count(), ( *comp_it )->GetSize() ) );
				}
			}
		}
	}

	return nullptr;
}

void XE::World::Each( const XE::TemplateType & components, const XE::TemplateType & include, const XE::TemplateType & exclude, const XE::Delegate< void( XE::InvokeStack * ) > & callback ) const
{
	XE::InvokeStack stack;

	for ( const auto & arche_it : _p->_ArchetypeMap )
	{
		auto & arche_type = arche_it.second;

		if ( std::all_of( components.begin(), components.end(), [&arche_type]( const XE::MetaTypeCPtr & val ) { return std::find( arche_type.ComponentTypes.begin(), arche_type.ComponentTypes.end(), val ) != arche_type.ComponentTypes.end(); } ) &&
			( include.empty() || std::all_of( include.begin(), include.end(), [&arche_type]( const XE::MetaTypeCPtr & val ) { return std::find( arche_type.ComponentTypes.begin(), arche_type.ComponentTypes.end(), val ) != arche_type.ComponentTypes.end(); } ) ) &&
			( exclude.empty() || !std::any_of( exclude.begin(), exclude.end(), [&arche_type]( const XE::MetaTypeCPtr & val ) { return std::find( arche_type.ComponentTypes.begin(), arche_type.ComponentTypes.end(), val ) != arche_type.ComponentTypes.end(); } ) ) )
		{
			XE::StackMemoryResource< KBYTE( 1 ) > resource;
			XE::Array< XE::uint64 > sizes( components.size(), &resource );
			XE::Array< XE::uint64 > offsets( components.size(), &resource );

			for ( const auto & type : components )
			{
				XE::uint64 offset = 0;
				auto it = std::find( arche_type.ComponentTypes.begin(), arche_type.ComponentTypes.end(), type );
				for ( auto it = arche_type.ComponentTypes.begin(); it != it; ++it )
				{
					offset += ( *it )->GetSize();
				}
				offsets.push_back( offset );
				sizes.push_back( type->GetSize() );
			}

			for ( auto & chunk : arche_type.Chunks )
			{
				for ( size_t i = 0; i < chunk->Bits.Count(); i++ )
				{
					if ( chunk->Bits.Test( i ) )
					{
						stack.Clear();

						for ( size_t i = 0; i < components.size(); i++ )
						{
							stack.Push( XE::Variant( CalcComponentPtr( chunk->Data, offsets[i], i, chunk->Bits.Count(), sizes[i] ), components[i].get() ) );
						}

						callback( &stack );
					}
				}
			}
		}
	}
}

void XE::World::AddCommand( const XE::Delegate< void() > & callback )
{
	_p->_TaskQueue.push( callback );
}

void XE::World::Serialize( XE::OArchive & archive ) const
{
	archive 
		& ARCHIVE_NVP( "Enable", _p->_Enable ) 
		& ARCHIVE_NVP( "Name", _p->_Name ) 
		& ARCHIVE_NVP( "Transfrom", _p->_Transform ) 
		& ARCHIVE_NVP( "GameObjects", _p->_GameObjects ) 
		& ARCHIVE_NVP( "SystemGroup", _p->_SystemGroup )
		& ARCHIVE_NVP( "EntityHandleAllocator", _p->_EntityHandleAllocator );
	
	XE::Array< XE::EntityPtr > Entities;
	{
		for ( const auto & entity_it : _p->_EntityMap )
		{
			XE::EntityPtr entity = XE::MakeShared< XE::Entity >();
			{
				entity->Hash = entity_it.second;
				entity->Handle = entity_it.first;
				auto arche_it = _p->_ArchetypeMap.find( entity_it.second );
				if ( arche_it == _p->_ArchetypeMap.end() ) continue;

				auto index_it = arche_it->second.EntityIndexMap.find( entity_it.first );
				if ( index_it == arche_it->second.EntityIndexMap.end() ) continue;

				XE::uint64 offset = 0;
				for ( const auto & type : arche_it->second.ComponentTypes )
				{
					XE::uint64 size = type->GetSize();
					entity->Components.push_back(
						XE::Variant( 
							CalcComponentPtr( ( *( index_it->second.Data ) )->Data, offset, index_it->second.Index, ( *( index_it->second.Data ) )->Bits.Count(), size ),
							type.get()
						)
					);
					offset += size;
				}
			}
			Entities.emplace_back( std::move( entity ) );
		}
	}
	archive & ARCHIVE_NVP( "Entities", Entities );
}

void XE::World::Deserialize( XE::IArchive & archive )
{
	XE::Array< XE::EntityPtr > Entities;
	archive
		& ARCHIVE_NVP( "Enable", _p->_Enable )
		& ARCHIVE_NVP( "Name", _p->_Name )
		& ARCHIVE_NVP( "Transfrom", _p->_Transform )
		& ARCHIVE_NVP( "GameObjects", _p->_GameObjects )
		& ARCHIVE_NVP( "SystemGroup", _p->_SystemGroup )
		& ARCHIVE_NVP( "EntityHandleAllocator", _p->_EntityHandleAllocator )
		& ARCHIVE_NVP( "Entities", Entities );

	for ( const auto & entity : Entities )
	{
		_p->_EntityMap.insert( { entity->Handle, entity->Hash } );
		auto arche_it = _p->_ArchetypeMap.find( entity->Hash );
		if ( arche_it == _p->_ArchetypeMap.end() )
		{
			arche_it = _p->_ArchetypeMap.insert( { entity->Hash, {} } ).first;
			for ( const auto & comp : entity->Components )
			{
				arche_it->second.ArchetypeSize += comp.GetType()->GetSize();
				arche_it->second.ComponentTypes.push_back( comp.GetType() );
			}
		}

		auto chunk_it = std::find_if( arche_it->second.Chunks.begin(), arche_it->second.Chunks.end(), []( const Chunk * chunk ) { return !chunk->Bits.All(); } );
		if ( chunk_it == arche_it->second.Chunks.end() )
		{
			Chunk * chunk = nullptr;
			if ( _p->_FreeChunks.empty() )
			{
				chunk = XE::New< Chunk >();
			}
			else
			{
				chunk = _p->_FreeChunks.front();
				_p->_FreeChunks.pop_front();
			}

			chunk->Bits.Reset( ChunkSize / arche_it->second.ArchetypeSize, false );

			arche_it->second.Chunks.push_back( chunk );
			chunk_it = --arche_it->second.Chunks.end();
		}

		IndexID index;
		index.Data = chunk_it;
		index.Index = ( *chunk_it )->Bits.FindFirstFalse();

		( *chunk_it )->Bits.Set( index.Index, true );

		arche_it->second.EntityIndexMap.insert( { entity->Handle, index } );

		XE::uint64 offset = 0;
		for ( const auto & comp : entity->Components )
		{
			auto size = comp.GetType()->GetSize();
			auto new_p = CalcComponentPtr( ( *index.Data )->Data, offset, index.Index, ( *index.Data )->Bits.Count(), comp.GetType()->GetSize() );
			auto old_p = reinterpret_cast<const XE::uint8 *>( comp.Value< const XE::EntityComponent * >() );
			std::copy( old_p, old_p + size, new_p );
			offset += size;
		}
	}
}

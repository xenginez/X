#include "GameObject.h"

#include "Utils/Logger.h"

#include "World.h"

BEG_META( XE::GameObject )
END_META()

XE::GameObject::GameObject()
{

}

XE::GameObject::~GameObject()
{

}

void XE::GameObject::Startup()
{
	if ( auto parent = _Parent.lock() )
	{
		_Transform.SetParent( &parent->_Transform );
	}

	for ( auto & it : _Components )
	{
		it->_GameObject = XE_THIS( XE::GameObject );

		if ( it->GetEnable() )
		{
			it->Startup();
		}
	}

	_Start = true;
}

void XE::GameObject::Update( XE::float32 dt )
{
	for ( auto & it : _Components )
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
}

void XE::GameObject::Clearup()
{
	for ( auto & it : _Components )
	{
		if ( it->_Start )
		{
			it->Clearup();
		}
	}

	_Start = false;
}

bool XE::GameObject::GetEnable() const
{
	return _Enable;
}

void XE::GameObject::SetEnable( bool val )
{
	_Enable = val;
}

const XE::String & XE::GameObject::GetName() const
{
	return _Name;
}

void XE::GameObject::SetName( const XE::String & val )
{
	_Name = val;
}

XE::GameObjectPtr XE::GameObject::GetParent() const
{
	return _Parent.lock();
}

void XE::GameObject::SetParent( const XE::GameObjectPtr & val )
{
	if ( auto parent = _Parent.lock() )
	{
		auto it = std::find( parent->_Children.begin(), parent->_Children.end(), XE_THIS( XE::GameObject ) );
		if ( it != parent->_Children.end() )
		{
			parent->_Children.erase( it );
		}
	}

	_Parent = val;

	if ( auto parent = _Parent.lock() )
	{
		_Transform.SetParent( &parent->_Transform );

		parent->_Children.push_back( XE_THIS( XE::GameObject ) );
	}
}

XE::WorldPtr XE::GameObject::GetWorld() const
{
	return _World.lock();
}

XE::Transform & XE::GameObject::GetTransform()
{
	return _Transform;
}

XE::GameObjectComponentPtr XE::GameObject::AddComponent( const XE::MetaClassCPtr & val )
{
	for ( const auto & it : _Components )
	{
		if ( it->GetMetaClass() == val )
		{
			XE_ERROR( "{%0} the type already exists.", val->GetFullName() );

			return nullptr;
		}
	}

	if ( XE::GameObjectComponentPtr comp = val->ConstructPtr().Value< XE::GameObjectComponentPtr >() )
	{
		_Components.push_back( comp );

		return comp;
	}

	return nullptr;
}

XE::GameObjectComponentPtr XE::GameObject::GetComponent( const XE::MetaClassCPtr & val )
{
	for ( auto & it : _Components )
	{
		if ( it->GetMetaClass()->CanConvert( val ) )
		{
			return it;
		}
	}

	return nullptr;
}

XE::GameObjectPtr XE::GameObject::GetGameObjectFromChild( const XE::String & val )
{
	if ( val.empty() )
	{
		return nullptr;
	}

	auto pos = val.find( "/" );
	if ( pos == XE::String::npos )
	{
		auto it = std::find_if( _Children.begin(), _Children.end(), [&]( const XE::GameObjectPtr & obj ) { return obj->GetName() == val; } );
		return it != _Children.end() ? *it : nullptr;
	}

	XE::String name( val.begin(), val.begin() + pos );
	auto it = std::find_if( _Children.begin(), _Children.end(), [&]( const XE::GameObjectPtr & obj ) { return obj->GetName() == name; } );
	return it != _Children.end() ? ( *it )->GetGameObjectFromChild( { val.begin() + pos + 1, val.end() } ) : nullptr;
}

BEG_META( XE::GameObjectComponent )
END_META()

XE::GameObjectComponent::GameObjectComponent()
{

}

XE::GameObjectComponent::~GameObjectComponent()
{

}

void XE::GameObjectComponent::Startup()
{
	OnStartup();

	_Start = true;
}

void XE::GameObjectComponent::Update( XE::float32 dt )
{
	if ( GetEnable() )
	{
		OnUpdate( dt );
	}
}

void XE::GameObjectComponent::Clearup()
{
	OnClearup();

	_Start = false;
}

void XE::GameObjectComponent::OnStartup()
{

}

void XE::GameObjectComponent::OnUpdate( XE::float32 dt )
{

}

void XE::GameObjectComponent::OnClearup()
{

}

bool XE::GameObjectComponent::GetEnable() const
{
	return _Enable;
}

void XE::GameObjectComponent::SetEnable( bool val )
{
	_Enable = val;
}

XE::WorldPtr XE::GameObjectComponent::GetWorld() const
{
	return GetGameObject()->GetWorld();
}

XE::Transform & XE::GameObjectComponent::GetTransform() const
{
	return GetGameObject()->GetTransform();
}

XE::GameObjectPtr XE::GameObjectComponent::GetGameObject() const
{
	return _GameObject.lock();
}

XE::CoreFrameworkPtr XE::GameObjectComponent::GetFramework() const
{
	return XE::CoreFramework::GetCurrentFramework();
}

XE::GameObjectPtr XE::GameObjectComponent::GetParentGameObject() const
{
	return GetGameObject()->GetParent();
}

XE::ServicePtr XE::GameObjectComponent::GetService( const XE::String & val ) const
{
	return GetFramework()->GetService( XE::Reflection::FindClass( val ) );
}

XE::ServicePtr XE::GameObjectComponent::GetService( const XE::MetaClassCPtr & val ) const
{
	return GetFramework()->GetService( val );
}

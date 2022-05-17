/*!
 * \file	GameObject.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GAMEOBJECT_H__64440D10_05E6_4874_BEDA_A10226D65AAD
#define GAMEOBJECT_H__64440D10_05E6_4874_BEDA_A10226D65AAD

#include "Type.h"

#include "Core/Service.h"
#include "Math/Transform.h"
#include "Core/CoreFramework.h"

BEG_XE_NAMESPACE

class XE_API GameObject final : public XE::Object
{
	OBJECT( GameObject, XE::Object )

private:
	friend class Level;

public:
	GameObject();

	~GameObject() override;

public:
	void Startup();

	void Update( XE::float32 dt );

	void Clearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

	XE::GameObjectPtr GetParent() const;

	void SetParent( const XE::GameObjectPtr & val );

public:
	XE::LevelPtr GetLevel() const;

	XE::Transform & GetTransform();

	XE::GameObjectPtr GetGameObjectFromChild( const XE::String & val );

public:
	XE::GameObjectComponentPtr AddComponent( const XE::MetaClassCPtr & val );

	XE::GameObjectComponentPtr GetComponent( const XE::MetaClassCPtr & val );

	template< typename T > XE::SharedPtr< T > AddComponent()
	{
		return SP_CAST< T >( AddComponent( ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() ) );
	}

	template< typename T > XE::SharedPtr< T > GetComponent()
	{
		return SP_CAST< T >( GetComponent( ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() ) );
	}

private:
	bool _Start = false;
	bool _Enable = true;

private:
	XE::String _Name;
	XE::Transform _Transform;
	XE::Array< XE::GameObjectComponentPtr > _Components;

private:
	XE::LevelWPtr _Level;
	XE::GameObjectWPtr _Parent;
	XE::Array< XE::GameObjectPtr > _Children;
};

class XE_API GameObjectComponent : public XE::Object
{
	OBJECT( GameObjectComponent, XE::Object )

private:
	friend class GameObject;

public:
	GameObjectComponent();

	~GameObjectComponent() override;

public:
	void Startup();

	void Update( XE::float32 dt );

	void Clearup();

protected:
	virtual void OnStartup();

	virtual void OnUpdate( XE::float32 dt );

	virtual void OnClearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

public:
	XE::LevelPtr GetLevel() const;

	XE::Transform & GetTransform() const;

	XE::GameObjectPtr GetGameObject() const;

	XE::GameObjectPtr GetParentGameObject() const;

	XE::CoreFrameworkPtr GetCoreFramework() const;

	XE::ServicePtr GetService( const XE::String & val ) const;

	XE::ServicePtr GetService( const XE::MetaClassCPtr & val ) const;

	template< typename T > XE::SharedPtr< T > GetService() const
	{
		return SP_CAST< T >( GetService( ClassID< typename XE::TypeTraits< T >::raw_t >::Get() ) );
	}

private:
	bool _Start = false;
	bool _Enable = true;
	XE::GameObjectWPtr _GameObject;
};

END_XE_NAMESPACE

#endif//GAMEOBJECT_H__64440D10_05E6_4874_BEDA_A10226D65AAD

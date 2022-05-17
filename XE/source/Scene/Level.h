/*!
 * \file   Level.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/25
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227
#define LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227

#include "Type.h"

#include "Math/Transform.h"

BEG_XE_NAMESPACE

class XE_API Level : public XE::Object
{
	OBJECT( Level, XE::Object );

private:
	struct Private;

public:
	Level();

	~Level() override;

public:
	void Startup();

	void Update( XE::float32 dt );

	void Clearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

public:
	XE::Transform & GetTransform();

public:
	XE::GameObjectPtr GetGameObject( const XE::String & val );

	XE::GameObjectPtr AddGameObject( const XE::String & name, const XE::GameObjectPtr & parent = nullptr );

public:
	XE::EntityHandle AddEntity( const XE::TemplateType & components );

	bool RemoveEntity( const XE::EntityHandle & val );

	bool AddSystem( const XE::TemplateType & val );

	bool RemoveSystem( const XE::TemplateType & val );

	bool AddComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component );

	bool RemoveComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component );

	template< typename ... T > XE::EntityHandle AddEntity()
	{
		static_assert( std::conjunction_v< std::is_base_of_v< XE::EntityComponent, T >... >, "not a subtype of XE::EntityComponent." );

		return AddEntity( XE::MakeTemplateType< T... >() );
	}

	template< typename ... T > bool AddSystem()
	{
		static_assert( std::conjunction_v< std::is_base_of_v< XE::EntitySystem, T >... >, "not a subtype of XE::EntitySystem." );

		return AddSystem( XE::MakeTemplateType< T... >() );
	}

	template< typename ... T > bool RemoveSystem()
	{
		static_assert( std::conjunction_v< std::is_base_of_v< XE::EntitySystem, T >... >, "not a subtype of XE::EntitySystem." );

		return RemoveSystem( XE::MakeTemplateType< T... >() );
	}

	template< typename T > bool AddComponent( const XE::EntityHandle & handle )
	{
		static_assert( std::is_base_of_v< XE::EntityComponent, T >, "not a subtype of XE::EntityComponent." );

		return AddComponent( handle, ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() );
	}

	template< typename T > bool RemoveComponent( const XE::EntityHandle & handle )
	{
		return RemoveComponent( handle, ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() );
	}

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227

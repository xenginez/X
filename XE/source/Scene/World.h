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

class XE_API World : public XE::Object
{
	OBJECT( World, XE::Object );

private:
	struct Private;

public:
	World();

	~World() override;

public:
	void Startup();

	void Update( XE::float32 dt );

	void Clearup();

public:
	bool GetEnable() const;

	void SetEnable( bool val );

	const XE::String & GetName() const;

	void SetName( const XE::String & val );

public:
	XE::Transform & GetTransform();

public:
	XE::GameObjectPtr GetGameObject( const XE::String & val );

	XE::GameObjectPtr AddGameObject( const XE::String & name, const XE::GameObjectPtr & parent = nullptr );

public:
	void SetSystemGraph( const XE::EntitySystemGraph & val );

public:
	XE::EntityHandle AddEntity( const XE::TemplateType & components );

	void RemoveEntity( const XE::EntityHandle & val );

	template< typename ... T > XE::EntityHandle AddEntity()
	{
		static_assert( std::conjunction_v< std::is_base_of_v< XE::EntityComponent, T >... >, "not a subtype of XE::EntityComponent." );

		return AddEntity( XE::MakeTemplateType< T... >() );
	}

public:
	void AddComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component );

	void RemoveComponent( const XE::EntityHandle & handle, const XE::MetaClassCPtr & component );

	XE::EntityComponent * GetComponent( const XE::MetaClassCPtr & type, const XE::EntityHandle & handle ) const;

	template< typename T > void AddComponent( const XE::EntityHandle & handle )
	{
		static_assert( std::is_base_of_v< XE::EntityComponent, T >, "not a subtype of XE::EntityComponent." );

		AddComponent( handle, ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() );
	}

	template< typename T > void RemoveComponent( const XE::EntityHandle & handle )
	{
		RemoveComponent( handle, ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() );
	}

	template< typename T > T * GetComponent( const XE::EntityHandle & handle ) const
	{
		static_assert( std::is_base_of_v< XE::EntityComponent, T >, "not a subtype of XE::EntityComponent." );

		return static_cast<T *>( GetComponent( ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get(), handle ) );
	}

public:
	void Each( const XE::TemplateType & components, const XE::TemplateType & include, const XE::TemplateType & exclude, const XE::Delegate< void( XE::InvokeStack * ) > & callback ) const;
	
	template< typename F > void Each( F && callback, const XE::TemplateType & include = {}, const XE::TemplateType & exclude = {} ) const
	{
		typename XE::FunctionTraits< F >::pod_tuple_type pod_tuple;

		Each( 
			std::apply( []( auto &&... args ) { return XE::MakeParameterType< decltype( args )... >(); }, std::move( pod_tuple ) ),
			include,
			exclude,
			[callback = std::move( callback )]( XE::InvokeStack * stack )
			{
				typename XE::FunctionTraits< F >::tuple_type tuple;

				std::apply( [callback = std::move( callback ), stack]( auto &&... args )
				{
					callback( stack->Pop<decltype( args )>()... );
				}, std::move( tuple ) );
			} );
	}

public:
	void AddCommand( const XE::Delegate< void() > & callback ) const;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // LEVEL_H__0AF6B270_6AC9_4303_AFB5_F2A9EF50E227

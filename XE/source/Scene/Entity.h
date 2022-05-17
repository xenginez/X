/*!
 * \file   Entity.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0
#define ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Entity
{
public:
	XE::EntityHandle Handle;
};
DECL_XE_CLASS( Entity );

class XE_API EntityResult
{
public:
	XE::EntityComponent * WriteComponent( const XE::MetaClassCPtr & val );

	const XE::EntityComponent * ReadComponent( const XE::MetaClassCPtr & val );

	template< typename T > T * WriteComponent()
	{
		static_assert( std::is_base_of_v< XE::EntityComponent, T >, "" );

		return static_cast<T *>( WriteComponent( ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() ) );
	}

	template< typename T > const T * ReadComponent()
	{
		static_assert( std::is_base_of_v< XE::EntityComponent, T >, "" );

		return static_cast<const T *>( WriteComponent( ::ClassID< typename XE::TypeTraits< T >::raw_t >::Get() ) );
	}
};
DECL_XE_CLASS( EntityResult );

class XE_API EntityComponent
{

};
DECL_XE_CLASS( EntityComponent );

class XE_API EntitySystem : public XE::Object
{
	OBJECT( EntitySystem, XE::Object );

public:
	EntitySystem() = default;

	virtual ~EntitySystem() = default;

public:
	virtual XE::TemplateType ReadComponents() const = 0;

	virtual XE::TemplateType WriteComponents() const = 0;

	virtual void Execute( EntityResult & result ) const = 0;
};

END_XE_NAMESPACE

#endif // ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0

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

class XE_API EntitySystem
{
public:
	virtual void Execute( XE::World * world, XE::float32 dt ) const = 0;
};
DECL_XE_CLASS( EntitySystem );

class XE_API EntityComponent
{
public:
	XE::EntityHandle Entity;
};
DECL_XE_CLASS( EntityComponent );

class XE_API EntitySystemGroup : public XE::EntitySystem
{
	friend class World;
	friend struct XE::MetaTypeCollector< XE::EntitySystemGroup >;

public:
	void Execute( XE::World * world, XE::float32 dt ) const override;

private:
	XE::EntitySystemGraph _Graph;
	mutable XE::EntitySystemPaths _Paths;
};
DECL_XE_CLASS( EntitySystemGroup, EntitySystem );

class XE_API NameEntityComponent : public XE::EntityComponent
{
public:
	XE::String Name;
};
DECL_XE_CLASS( NameEntityComponent, EntityComponent );

END_XE_NAMESPACE

#endif // ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0

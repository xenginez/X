/*!
 * \file   Declare.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef DECLARE_H__5CF70235_A6C2_4E42_89ED_5D453F005819
#define DECLARE_H__5CF70235_A6C2_4E42_89ED_5D453F005819

#include "Math/Declare.h"
#include "Utils/Declare.h"

BEG_XE_NAMESPACE

class World;
class GameObject;
class EntitySystem;
class EntityComponent;
class GameObjectComponent;
using EntitySystemGraph = XE::Graph< XE::SharedPtr< XE::EntitySystem > >;
using EntitySystemPaths = XE::List< XE::Array< XE::EntitySystemGraph::vertex_const_iterator > >;

END_XE_NAMESPACE

#endif // DECLARE_H__5CF70235_A6C2_4E42_89ED_5D453F005819

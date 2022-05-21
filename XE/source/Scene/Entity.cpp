#include "Entity.h"

#include "Utils/Logger.h"
#include "Core/CoreFramework.h"
#include "Core/ThreadService.h"

#include "World.h"

BEG_META( XE::EntitySystem )
END_META()

BEG_META( XE::EntityComponent )
END_META()

BEG_META( XE::EntitySystemGroup )
type->Property( "Graph", &XE::EntitySystemGroup::_Graph );
END_META()

BEG_META( XE::NameEntityComponent )
type->Property( "Name", &XE::NameEntityComponent::Name );
END_META()

namespace
{
	XE::EntitySystemPaths SystemGraphSortPath( const XE::EntitySystemGraph & graph )
	{
		XE::EntitySystemPaths paths;
		XE::List< XE::EntitySystemGraph::vertex_const_iterator > tmp;
		XE::Array< XE::EntitySystemGraph::vertex_const_iterator > layer;
		
		auto all_of = []( const XE::EntitySystemPaths & paths, XE::EntitySystemGraph::out_edge_const_iterator beg, XE::EntitySystemGraph::out_edge_const_iterator end ) -> bool
		{
			for ( auto it = beg; it != end; ++it )
			{
				if ( std::all_of( paths.begin(), paths.end(), [it]( const XE::EntitySystemPaths::value_type & layer )
					{
						return std::find( layer.begin(), layer.end(), *it ) != layer.end();
					} ) == false )
				{
					return false;
				}
			}

			return true;
		};

		{
			auto edges = graph.out_edges( {} );
			for ( auto it = edges.first; it != edges.second; ++it ) tmp.push_back( *it );
		}

		while ( !tmp.empty() )
		{
			for ( auto it = tmp.begin(); it != tmp.end(); ++it )
			{
				auto in_edges = graph.in_edges( *it );
				if ( in_edges.first == in_edges.second || all_of( paths, in_edges.first, in_edges.second ) )
				{
					layer.push_back( *it );
					it = tmp.erase( it );
				}
			}
			for ( const auto & it : layer )
			{
				auto out_edges = graph.out_edges( it );
				for ( auto it = out_edges.first; it != out_edges.second; ++it ) tmp.push_back( *it );
			}

			paths.emplace_back( std::move( layer ) ); layer.clear();
		}

		return paths;
	}
}

void XE::EntitySystemGroup::Execute( XE::World * world, XE::float32 dt ) const
{
	if ( _Paths.empty() )
	{
		_Paths = SystemGraphSortPath( _Graph );
	}

	if ( auto thread = XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >() )
	{
		for ( auto layer : _Paths )
		{
			thread->ParallelTask( layer.begin(), layer.end(), [world, dt]( XE::EntitySystemGraph::vertex_const_iterator it )
				{
					( *it )->Execute( world, dt );
				} ).get();
		}
	}
}

/*!
 * \file	Graph.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/05/17
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GRAPH_HPP__0D72231B_C9AC_4413_B06B_EA4C9966C4BD
#define GRAPH_HPP__0D72231B_C9AC_4413_B06B_EA4C9966C4BD

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T, typename A = std::pmr::polymorphic_allocator< T > > class Graph
{
public:
	using size_type = XE::uint64;
	using value_type = T;
	using vertex_type = T;
	using allocator_type = A;
	using reference_type = T &;
	using const_reference_type = const T &;
	using difference_type = std::ptrdiff_t;
	using allocator_traits = std::allocator_traits< allocator_type >;

	using vertex_container_type = XE::List< vertex_type >;
	using vertex_iterator = typename vertex_container_type::iterator;
	using vertex_const_iterator = typename vertex_container_type::const_iterator;

	using adjacent_container_type = XE::List< vertex_const_iterator >;
	using in_edge_iterator = typename adjacent_container_type::iterator;
	using in_edge_const_iterator = typename adjacent_container_type::const_iterator;
	using out_edge_iterator = typename adjacent_container_type::iterator;
	using out_edge_const_iterator = typename adjacent_container_type::const_iterator;

	struct less {  bool operator()( const vertex_const_iterator & l, const vertex_const_iterator & r ) const volatile { return *l < *r; } };
	using edge_container_type = XE::Map < vertex_const_iterator, XE::Pair< adjacent_container_type, adjacent_container_type >, less >;

public:
	Graph( const allocator_type & alloc = allocator_type() )
		:_Edges( alloc.resource() ), _Vertices( alloc.resource() )
	{

	}

	Graph( Graph && val )
	{
		swap( std::forward< Graph &>( val ) );
	}

	Graph( const Graph & val )
		:_Edges( val._Edges ), _Vertices( val._Vertices )
	{

	}

public:
	Graph & operator= ( Graph && val )
	{
		swap( std::forward< Graph &>( val ) );

		return *this;
	}

	Graph & operator= ( const Graph & val )
	{
		_Edges = val._Edges;
		_Vertices = val._Vertices;

		return *this;
	}

public:
	void swap( Graph & val )
	{
		std::swap( _Edges, val._Edges );
		std::swap( _Vertices, val._Vertices );
	}

public:
	vertex_iterator add_vertex( vertex_type && val )
	{
		_Vertices.emplace_back( val );

		return --_Vertices.end();
	}

	vertex_iterator add_vertex( const vertex_type & val )
	{
		_Vertices.push_back( val );

		return --_Vertices.end();
	}

	void add_edge( vertex_const_iterator src, vertex_const_iterator dst )
	{
		_Edges[dst].first.push_back( src );
		_Edges[src].second.push_back( dst );
	}

public:
	void remove_vertex( const vertex_type & val )
	{
		auto it = std::find( _Vertices.begin(), _Vertices.end(), val );
		if ( it != _Vertices.end() )
		{
			remove_vertex( it );
		}
	}

	void remove_vertex( vertex_const_iterator val )
	{
		auto it = _Edges.find( val );
		if ( it != _Edges.end() )
		{
			for ( const auto & in : it->second.first )
			{
				_Edges[in].second.remove( val );
			}
			for ( const auto & out : it->second.second )
			{
				_Edges[out].first.remove( val );
			}
			_Edges.erase( it );
		}
	}

	void remove_edge( vertex_const_iterator src, vertex_const_iterator dst )
	{
		auto d_it = _Edges.find( dst );
		if ( d_it != _Edges.end() )
		{
			d_it->second.first.remove( src );
		}
		auto s_it = _Edges.find( src );
		if ( s_it != _Edges.end() )
		{
			s_it->second.first.remove( dst );
		}
	}

	void clear()
	{
		_Edges.clear();
		_Vertices.clear();
	}

public:
	vertex_iterator find_vertex( const vertex_type & val )
	{
		auto it = std::find( _Vertices.begin(), _Vertices.end(), val );

		return ( it != _Vertices.end() ) ? it : _Vertices.end();
	}

	vertex_const_iterator find_vertex( const vertex_type & val ) const
	{
		auto it = std::find( _Vertices.begin(), _Vertices.end(), val );

		return ( it != _Vertices.end() ) ? it : _Vertices.end();
	}

public:
	XE::Pair< vertex_iterator, vertex_iterator > vertices()
	{
		return { _Vertices.begin(), _Vertices.end() };
	}

	XE::Pair< vertex_const_iterator, vertex_const_iterator > vertices() const
	{
		return { _Vertices.begin(), _Vertices.end() };
	}

	XE::Pair< in_edge_iterator, in_edge_iterator > in_edges( vertex_const_iterator val )
	{
		auto it = _Edges.find( val );
		if ( it != _Edges.end() )
		{
			return { it->second.first.begin(),it->second.first.end() };
		}

		return {};
	}

	XE::Pair< in_edge_const_iterator, in_edge_const_iterator > in_edges( vertex_const_iterator val ) const
	{
		auto it = _Edges.find( val );
		if ( it != _Edges.end() )
		{
			return { it->second.first.begin(),it->second.first.end() };
		}

		return {};
	}

	XE::Pair< out_edge_iterator, out_edge_iterator > out_edges( vertex_const_iterator val )
	{
		auto it = _Edges.find( val );
		if ( it != _Edges.end() )
		{
			return { it->second.second.begin(),it->second.second.end() };
		}

		return {};
	}

	XE::Pair< out_edge_const_iterator, out_edge_const_iterator > out_edges( vertex_const_iterator val ) const
	{
		auto it = _Edges.find( val );
		if ( it != _Edges.end() )
		{
			return { it->second.second.begin(), it->second.second.end() };
		}

		return {};
	}

private:
	edge_container_type _Edges;
	vertex_container_type _Vertices;
};

END_XE_NAMESPACE

#endif//GRAPH_HPP__0D72231B_C9AC_4413_B06B_EA4C9966C4BD

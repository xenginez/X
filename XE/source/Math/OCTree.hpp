/*!
 * \file	OCTree.hpp
 *
 * \author	ZhengYuanQing
 * \date	2019/07/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef OCTREE_HPP__A57E4FF7_6104_48B7_A4FD_0114648F4E85
#define OCTREE_HPP__A57E4FF7_6104_48B7_A4FD_0114648F4E85

#include "AABB.h"

BEG_XE_NAMESPACE

template< typename T, XE::uint64 D = 8, typename A = std::pmr::polymorphic_allocator< T > > class OCTree : public NonCopyable
{
public:
	static constexpr XE::uint64 MAX_DEPTH_COUNT = D;

public:
	using ValueType = T;
	using BoxType = XE::AABB;
	using AllocatorType = A;

private:
	struct Node
	{
		Node( const AllocatorType & val = {} )
			: Values( val )
		{

		}

		BoxType Bounding;
		XE::uint64 Depth = 0;
		XE::uint64 Children = 0;
		XE::Array< ValueType > Values;
	};

public:
	OCTree( const AllocatorType & val = {} )
		:_Nodes( val.resource() )
	{

	}

public:
	void Insert( const ValueType & val )
	{
		if ( _Nodes.empty() )
		{
			_Nodes.resize( 1 );

			_Nodes[0].Bounding = _Make( val );
			_Nodes[0].Depth = 0;
			_Nodes[0].Children = 0;
			_Nodes[0].Values.push_back( val );
		}
		else
		{
			Insert( 0, val );
		}
	}

	void Remove( const ValueType & val )
	{
		if ( _Nodes.empty() )
		{
			return;
		}

		Remove( 0, val );
	}

	void Clear()
	{
		_Nodes.clear();
	}

public:
	template< typename G > void Intersect( const G & geometry, XE::Array< ValueType > & outs )
	{
		Intersect( geometry, {}, outs );
	}

	template< typename G > void Intersect( const G & geometry, XE::Span< const ValueType > exclude, XE::Array< ValueType > & outs )
	{
		if ( _Nodes.empty() )
		{
			return;
		}

		if ( _Nodes[0].Bounding.Intersect( geometry ) )
		{
			for ( const auto & it : _Nodes[0].Values )
			{
				if (_Make(it).Intersect( geometry ) )
				{
					if ( std::find( exclude.begin(), exclude.end(), it ) == exclude.end() )
					{
						outs.push_back( it );
					}
				}
			}

			if ( _Nodes[0].Children != 0 )
			{
				for ( size_t i = _Nodes[0].Children; i < _Nodes[0].Children + 8; i++ )
				{
					Intersect( i, geometry, exclude, outs );
				}
			}
		}
	}

private:
	void Rebuild( XE::uint64 parent )
	{
		if ( _Nodes[parent].Children != 0 )
		{
			for ( size_t i = _Nodes[parent].Children; i < _Nodes[parent].Children + 8; i++ )
			{
				switch ( i - parent )
				{
				case 0: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetFarLeftTop(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 1: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetFarRightTop(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 2: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetFarLeftBottom(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 3: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetFarRightBottom(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 4: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetNearLeftTop(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 5: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetNearRightTop(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 6: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetNearLeftBottom(), _Nodes[parent].Bounding.GetCenter() }; break;
				case 7: _Nodes[i].Bounding = { _Nodes[parent].Bounding.GetNearRightBottom(), _Nodes[parent].Bounding.GetCenter() }; break;
				}

				Rebuild( i );
			}
		}
	}

	void Insert( XE::uint64 parent, const ValueType & val )
	{
		auto aabb = _Make( val );

		if ( ( _Nodes[parent].Children == 0 && _Nodes[parent].Values.size() < 10 ) || _Nodes[parent].Depth == MAX_DEPTH_COUNT )
		{
			_Nodes[parent].Bounding.Merge( aabb );

			_Nodes[parent].Values.push_back( val );
		}
		else if( _Nodes[parent].Children == 0 )
		{
			auto child = _Nodes.size();
			_Nodes.resize( _Nodes.size() + 8 );

			_Nodes[parent].Children = child;
			_Nodes[parent].Bounding.Merge( aabb );
			_Nodes[parent].Values.push_back( val );

			for ( size_t i = child; i < child + 8; i++ )
			{
				_Nodes[child].Depth = _Nodes[parent].Depth + 1;
				_Nodes[child].Children = 0;
				Rebuild( parent );
			}

			for ( auto it = _Nodes[parent].Values.begin(); it != _Nodes[parent].Values.end(); )
			{
				XE::uint8 flag = 0;

				for ( size_t i = child; i <= child + 8; i++ )
				{
					if ( _Nodes[i].Bounding.Intersect( _Make( *it ) ) )
					{
						flag |= 1 << ( i - child );
					}
				}

				switch ( flag )
				{
				case 0x01: _Nodes[child + 0].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x02: _Nodes[child + 1].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x04: _Nodes[child + 2].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x08: _Nodes[child + 3].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x10: _Nodes[child + 4].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x20: _Nodes[child + 5].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x40: _Nodes[child + 6].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				case 0x80: _Nodes[child + 7].Values.push_back( *it ); it = _Nodes[parent].Values.erase( it ); break;
				default:
					++it;
					break;
				}
			}
		}
		else
		{
			if ( !_Nodes[parent].Bounding.Contains( _Make( val ) ) )
			{
				_Nodes[parent].Bounding.Merge( aabb );

				Rebuild( parent );
			}

			for ( size_t i = _Nodes[parent].Children; i < _Nodes[parent].Children + 8; i++ )
			{
				if (_Nodes[i].Bounding.Intersect( aabb ) )
				{
					Insert( i, val );
				}
			}
		}
	}

	void Remove( XE::uint64 parent, const ValueType & val )
	{
		auto aabb = _Make( val );

		if ( _Nodes[parent].Bounding.Intersect( aabb ) )
		{
			auto it = std::find( _Nodes[parent].Values.begin(), _Nodes[parent].Values.end(), val );
			if ( it != _Nodes[parent].Values.end() )
			{
				_Nodes[parent].Values.erase( it );
			}
			else if ( _Nodes[parent].Children != 0 )
			{
				for ( size_t i = _Nodes[parent].Children; i < _Nodes[parent].Children + 8; i++ )
				{
					Remove( i, val );
				}
			}
		}
	}

	template< typename G > void Intersect( XE::uint64 node_idx, const G & geometry, XE::Span< const ValueType > exclude, XE::Array< ValueType > & outs )
	{
		if ( node_idx < _Nodes.size() )
		{
			if ( _Nodes[node_idx].Bounding.Intersect( geometry ) )
			{
				for ( const auto & it : _Nodes[node_idx].Values )
				{
					if ( _Make( it ).Intersect( geometry ) && std::find( exclude.begin(), exclude.end(), it ) == exclude.end() )
					{
						outs.push_back( it );
					}
				}

				if ( _Nodes[node_idx].Children != 0 )
				{
					for ( size_t i = _Nodes[node_idx].Children; i < _Nodes[node_idx].Children + 8; i++ )
					{
						Intersect( i, geometry, exclude, outs );
					}
				}
			}
		}
	}

private:
	XE::Array< Node > _Nodes;
	XE::MakeAABB< ValueType > _Make;
};

END_XE_NAMESPACE

#endif // OCTREE_HPP__A57E4FF7_6104_48B7_A4FD_0114648F4E85

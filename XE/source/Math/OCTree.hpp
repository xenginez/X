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
	struct Box
	{
		Box( const AllocatorType & val = {} )
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
		:_Boxs( val.resource() )
	{

	}

public:
	void Insert( const ValueType & val )
	{
		if ( _Boxs.empty() )
		{
			_Boxs.resize( 1 );

			_Boxs[0].Bounding = _Make( val );
			_Boxs[0].Depth = 0;
			_Boxs[0].Children = 0;
			_Boxs[0].Values.push_back( val );
		}
		else
		{
			Insert( 0, val );
		}
	}

	void Remove( const ValueType & val )
	{
		if ( _Boxs.empty() )
		{
			return;
		}

		Remove( 0, val );
	}

	void Clear()
	{
		_Boxs.clear();
	}

public:
	template< typename G > void Intersect( const G & geometry, XE::Array< ValueType > & outs )
	{
		Intersect( geometry, {}, outs );
	}

	template< typename G > void Intersect( const G & geometry, XE::Span< const ValueType > exclude, XE::Array< ValueType > & outs )
	{
		if ( _Boxs.empty() )
		{
			return;
		}

		if ( _Boxs[0].Bounding.Intersect( geometry ) )
		{
			for ( const auto & it : _Boxs[0].Values )
			{
				if (_Make(it).Intersect( geometry ) )
				{
					if ( std::find( exclude.begin(), exclude.end(), it ) == exclude.end() )
					{
						outs.push_back( it );
					}
				}
			}

			if ( _Boxs[0].Children != 0 )
			{
				for ( size_t i = _Boxs[0].Children; i < _Boxs[0].Children + 8; i++ )
				{
					Intersect( i, geometry, exclude, outs );
				}
			}
		}
	}

private:
	void Rebuild( XE::uint64 parent )
	{
		if ( _Boxs[parent].Children != 0 )
		{
			for ( size_t i = _Boxs[parent].Children; i < _Boxs[parent].Children + 8; i++ )
			{
				switch ( i - parent )
				{
				case 0: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetFarLeftTop(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 1: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetFarRightTop(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 2: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetFarLeftBottom(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 3: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetFarRightBottom(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 4: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetNearLeftTop(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 5: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetNearRightTop(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 6: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetNearLeftBottom(), _Boxs[parent].Bounding.GetCenter() }; break;
				case 7: _Boxs[i].Bounding = { _Boxs[parent].Bounding.GetNearRightBottom(), _Boxs[parent].Bounding.GetCenter() }; break;
				}

				Rebuild( i );
			}
		}
	}

	void Insert( XE::uint64 parent, const ValueType & val )
	{
		auto aabb = _Make( val );

		if ( ( _Boxs[parent].Children == 0 && _Boxs[parent].Values.size() < 10 ) || _Boxs[parent].Depth == MAX_DEPTH_COUNT )
		{
			_Boxs[parent].Bounding.Merge( aabb );

			_Boxs[parent].Values.push_back( val );
		}
		else if( _Boxs[parent].Children == 0 )
		{
			auto child = _Boxs.size();
			_Boxs.resize( _Boxs.size() + 8 );
			_Boxs[parent].Children = child;
			_Boxs[parent].Bounding.Merge( aabb );

			for ( size_t i = child; i < child + 8; i++ )
			{
				_Boxs[child].Depth = _Boxs[parent].Depth + 1;
				_Boxs[child].Children = 0;
				Rebuild( parent );
			}

			for ( size_t i = child; i <= child + 8; i++ )
			{
				if ( _Boxs[i].Bounding.Intersect( aabb ) )
				{
					_Boxs[i].Values.push_back( val );
				}
			}

			for ( const auto & it : _Boxs[parent].Values )
			{
				for ( size_t i = child; i <= child + 8; i++ )
				{
					if ( _Boxs[i].Bounding.Intersect( _Make( it ) ) )
					{
						_Boxs[i].Values.push_back( it );
					}
				}
			}

			_Boxs[parent].Values.clear();
			_Boxs[parent].Values.shrink_to_fit();
		}
		else
		{
			if ( !_Boxs[parent].Bounding.Contains( _Make( val ) ) )
			{
				_Boxs[parent].Bounding.Merge( aabb );

				Rebuild( parent );
			}

			for ( size_t i = _Boxs[parent].Children; i < _Boxs[parent].Children + 8; i++ )
			{
				if (_Boxs[i].Bounding.Intersect( aabb ) )
				{
					Insert( i, val );
				}
			}
		}
	}

	void Remove( XE::uint64 parent, const ValueType & val )
	{
		auto aabb = _Make( val );

		if ( _Boxs[parent].Bounding.Intersect( aabb ) )
		{
			if ( _Boxs[parent].Children == 0 )
			{
				auto it = std::find( _Boxs[parent].Values.begin(), _Boxs[parent].Values.end(), val );
				if (it != _Boxs[parent].Values.end() )
				{
					_Boxs[parent].Values.erase( it );
				}
			}
			else
			{
				for ( size_t i = _Boxs[parent].Children; i < _Boxs[parent].Children + 8; i++ )
				{
					Remove( i, val );
				}
			}
		}
	}

	template< typename G > void Intersect( XE::uint64 node_idx, const G & geometry, XE::Span< const ValueType > exclude, XE::Array< ValueType > & outs )
	{
		if ( node_idx < _Boxs.size() )
		{
			if ( _Boxs[node_idx].Bounding.Intersect( geometry ) )
			{
				for ( const auto & it : _Boxs[node_idx].Values )
				{
					if ( _Make( it ).Intersect( geometry ) )
					{
						if ( std::find( exclude.begin(), exclude.end(), it ) == exclude.end() )
						{
							outs.push_back( it );
						}
					}
				}

				if ( _Boxs[node_idx].Children != 0 )
				{
					for ( size_t i = _Boxs[node_idx].Children; i < _Boxs[node_idx].Children + 8; i++ )
					{
						Intersect( i, geometry, exclude, outs );
					}
				}
			}
		}
	}

private:
	XE::Array< Box > _Boxs;
	XE::MakeAABB< ValueType > _Make;
};

/*
template< typename _Ty, typename _By = MakeAABB< _Ty > > class OCTree : public NonCopyable
{
public:
	static constexpr XE::uint32 MaxDepthCount = 8;

public:
	using ValueType = typename _Ty;
	using BoundBoxType = typename _By;

public:
	struct Node
	{
		XE::AABB Box;
		XE::uint32 Depth = 0;
		XE::uint32 Children = 0;
		Array< ValueType > Objects;
	};

public:
	OCTree()
		:_Depth( 4 ), _Nodes( 1 )
	{
	}

	OCTree( XE::uint32 val )
		:_Depth( std::clamp( val, 1, MaxDepthCount ) ), _Nodes( 1 )
	{
		
	}

	~OCTree() = default;

public:
	XE::uint32 DepthCount() const
	{
		return _Depth;
	}

public:
	template< typename _Iter >
	void Rebuild( _Iter first, _Iter last )
	{
		_Nodes.clear();

		_Nodes.resize( 1 );

		std::for_each( first, last, [&]( const ValueType & val )
					   {
						   _Nodes[0].Objects.push_back( val );
						   _Nodes[0].Box.Merge( _BoundBox( val ) );
					   } );

		Build( 0, 0 );
	}

	void Clear()
	{
		_Nodes.clear();
		_Nodes.shrink_to_fit();
	}

public:
	template< typename _Other >
	ValueType Intersect( const _Other & val, const Array< ValueType > & exclude ) const
	{
		return Intersect( 0, exclude, val );
	}

public:
	template< typename _Other >
	Array< ValueType > Intersects( const _Other & val ) const
	{
		Set< ValueType > ret;

		Intersects( 0, ret, val );

		return { ret.begin(), ret.end() };
	}

private:
	void Build( XE::uint64 node, XE::uint32 depth )
	{
		if( _Nodes[node].Objects.size() > 1 )
		{
			_Nodes[node].Depth = depth;

			if( depth < _Depth )
			{
				Vec3f center = _Nodes[node].Box.GetCenter();

				XE::uint64 children = _Nodes.size();

				_Nodes[node].Children = static_cast< XE::uint32 >( children );

				_Nodes.resize( children + 8 );

				for( const auto & child : _Nodes[node].Objects )
				{
					XE::AABB aabb = _BoundBox( child );
					int mark[6];
					mark[0] = aabb.min.x >= center.x ? 1 : 0;
					mark[1] = aabb.min.y >= center.y ? 2 : 0;
					mark[2] = aabb.min.z >= center.z ? 4 : 0;
					mark[3] = aabb.max.x >= center.x ? 1 : 0;
					mark[4] = aabb.max.y >= center.y ? 2 : 0;
					mark[5] = aabb.max.z >= center.z ? 4 : 0;
					for( int i = 0; i < 8; ++i )
					{
						if( i == ( ( i & 1 ) ? mark[3] : mark[0] )
							+ ( ( i & 2 ) ? mark[4] : mark[1] )
							+ ( ( i & 4 ) ? mark[5] : mark[2] ) )
						{
							_Nodes[children + i].Objects.push_back( child );
						}
					}
				}

				for( int i = 0; i < 8; ++i )
				{
					_Nodes[children + i].Box = AABB(
						Vec3f(
							( i & 1 ) ? center.x : _Nodes[node].Box.min.x,
							( i & 2 ) ? center.y : _Nodes[node].Box.min.y,
							( i & 4 ) ? center.z : _Nodes[node].Box.min.z
						),
						Vec3f(
							( i & 1 ) ? _Nodes[node].Box.max.x : center.x,
							( i & 2 ) ? _Nodes[node].Box.max.y : center.y,
							( i & 4 ) ? _Nodes[node].Box.max.z : center.z
						)
					);

					Build( children + i, depth + 1 );
				}
			}
		}

		_Nodes[node].Objects.clear();
		_Nodes[node].Objects.shrink_to_fit();
	}

	template< typename _Other >
	ValueType Intersect( XE::uint64 node, const Array< ValueType > & exclude, const _Other & val ) const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val ) )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val ) )
					{
						for( const auto & ex : exclude )
						{
							if( ex == obj )
							{
								continue;
							}
						}

						return obj;
					}
				}
			}
			else
			{
				for( int i = 0; i < 8; ++i )
				{
					if( ValueType obj = Intersect( node + i, exclude, val ) )
					{
						for( const auto & ex : exclude )
						{
							if( ex == obj )
							{
								continue;
							}
						}

						return obj;
					}
				}
			}
		}

		return nullptr;
	}

	ValueType Intersect( XE::uint64 node, const Array< ValueType > & exclude, const Ray & val ) const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val ).first )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val ).first )
					{
						for( const auto & ex : exclude )
						{
							if( ex == obj )
							{
								continue;
							}
						}

						return obj;
					}
				}
			}
			else
			{
				for( int i = 0; i < 8; ++i )
				{
					if( ValueType obj = Intersect( node + i, exclude, val ) )
					{
						for( const auto & ex : exclude )
						{
							if( ex == obj )
							{
								continue;
							}
						}

						return obj;
					}
				}
			}
		}

		return nullptr;
	}

	template< typename _Other >
	void Intersects( XE::uint64 node, Set< ValueType > & out, const _Other & val ) const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val ) )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val ) )
					{
						out.insert( obj );
					}
				}
			}
			else
			{
				for( int i = 0; i < 8; ++i )
				{
					Intersects( node + i, out, val );
				}
			}
		}
	}

	void Intersects( XE::uint64 node, Set< ValueType > & out, const Ray & val ) const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val ).first )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val ).first )
					{
						out.insert( obj );
					}
				}
			}
			else
			{
				for( int i = 0; i < 8; ++i )
				{
					Intersects( node + i, out, val );
				}
			}
		}
	}

private:
	XE::uint32 _Depth;
	Array< Node > _Nodes;
	BoundBoxType _BoundBox;
};
*/
END_XE_NAMESPACE

#endif // OCTREE_HPP__A57E4FF7_6104_48B7_A4FD_0114648F4E85

/*!
 * \file	QTree.hpp
 *
 * \author	ZhengYuanQing
 * \date	2019/07/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef QTREE_HPP__29CC31D8_FEF9_4084_8E7E_531966B9692A
#define QTREE_HPP__29CC31D8_FEF9_4084_8E7E_531966B9692A

#include "Type.h"

BEG_XE_NAMESPACE

template< typename _Ty, typename _By = MakeRect< _Ty > > class QTree : public NonCopyable
{
public:
	static constexpr XE::uint32 MaxDepthCount = 8;

public:
	using ValueType = typename _Ty;
	using BoundBoxType = typename _By;

public:
	struct Node
	{
		XE::Rectf Box;
		XE::uint32 Depth = 0;
		XE::uint32 Children = 0;
		Array< ValueType > _Objects;
	};

public:
	QTree()
		:_Depth( 4 ), _Nodes( 1 )
	{
	}

	QTree( XE::uint32 val )
		:_Depth( std::clamp( val, 1, MaxDepthCount ) ), _Nodes( 1 )
	{

	}

	~QTree() = default;

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
						   _Nodes[0]._Objects.push_back( val );
						   _Nodes[0].Box.Encapsulate( _BoundBox( val ) );
					   } );

		Build( 0, 0 );
	}

	template< typename ... _Args >
	ValueType Intersect( const Array< ValueType > & exclude, const _Args & ... val )const
	{
		return Intersect( 0, exclude, val... );
	}

	template< typename ... _Args >
	Array< ValueType > Intersects( const _Args & ... val )const
	{
		Set< ValueType > out;

		Intersects( 0, out, val... );

		return { out.begin(), out.end() };
	}

private:
	void Build( XE::uint64 node, XE::uint32 depth )
	{
		if( _Nodes[node]._Objects.size() > 1 )
		{
			_Nodes[node]._Depth = depth;

			if( depth < _Depth )
			{

				Vec2f center = _Nodes[node].Box.GetCenter();

				XE::uint64 children = _Nodes.size();

				_Nodes[node]._Children = static_cast< XE::uint32 >( children );

				_Nodes.resize( children + 4 );

				for( const auto & child : _Nodes[node]._Objects )
				{
					XE::AABB aabb = _BoundBox( child );
					int mark[4];
					mark[0] = aabb.GetMin().x >= center.x ? 1 : 0;
					mark[1] = aabb.GetMin().y >= center.y ? 2 : 0;
					mark[3] = aabb.GetMax().x >= center.x ? 1 : 0;
					mark[4] = aabb.GetMax().y >= center.y ? 2 : 0;
					for( int i = 0; i < 4; ++i )
					{
						if( i == ( ( i & 1 ) ? mark[3] : mark[0] )
							+ ( ( i & 2 ) ? mark[4] : mark[1] ) )
						{
							_Nodes[children + i]._Objects.push_back( child );
						}
					}
				}

				for( int i = 0; i < 4; ++i )
				{
					_Nodes[children + i].Box = Rectf(
						Vec2f(
							( i & 1 ) ? center.x : _Nodes[node].Box.GetMin().x,
							( i & 2 ) ? center.y : _Nodes[node].Box.GetMin().y
						),
						Vec2f(
							( i & 1 ) ? _Nodes[node].Box.GetMax().x : center.x,
							( i & 2 ) ? _Nodes[node].Box.GetMax().y : center.y
						)
					);

					Build( children + i, depth + 1 );
				}
			}
		}

		_Nodes[node]._Objects.clear();
		_Nodes[node]._Objects.shrink_to_fit();
	}

	template< typename ... _Args >
	ValueType Intersect( XE::uint64 node, const Array< ValueType > & exclude, const _Args &... val )const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val... ) )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val... ) )
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
					if( ValueType obj = Intersect( node + i, exclude, val... ) )
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

	template< typename ... _Args >
	void Intersects( XE::uint64 node, Set< ValueType > & out, const _Args & ... val )const
	{
		const Node & n = _Nodes[node];

		if( n.Box.Intersect( val... ) )
		{
			if( n.Children == 0 )
			{
				for( const auto & obj : n.Objects )
				{
					if( _BoundBox( obj ).Intersect( val... ) )
					{
						out.insert( obj );
					}
				}
			}
			else
			{
				for( int i = 0; i < 8; ++i )
				{
					Intersects( node + i, out, val... );
				}
			}
		}
	}

private:
	XE::uint32 _Depth;
	Array< Node > _Nodes;
	BoundBoxType _BoundBox;
};

END_XE_NAMESPACE

#endif // QTREE_HPP__29CC31D8_FEF9_4084_8E7E_531966B9692A

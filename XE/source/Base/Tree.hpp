/*!
 * \file	Tree.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/03/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TREE_HPP__0988C9B3_974D_45EC_9BB3_ABD1F0EDE45E
#define TREE_HPP__0988C9B3_974D_45EC_9BB3_ABD1F0EDE45E

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T > struct TreeNodeInput;
template< typename T > struct TreeNodeOutput;

template< typename T, typename _Alloc = std::pmr::polymorphic_allocator< T > > class Tree
{
public:
	using size_type = XE::uint64;
	using value_type = T;
	using input_type = TreeNodeInput< T >;
	using output_type = TreeNodeOutput< T >;

	using allocator_type = _Alloc;
	using allocator_traits = std::allocator_traits< _Alloc >;

	using tier_container_type = XE::List< value_type >;
	using tree_container_type = XE::List< tier_container_type >;

	using iterator = typename tree_container_type::iterator;
	using const_iterator = typename tree_container_type::iterator;
	using reverse_iterator = typename tree_container_type::reverse_iterator;
	using const_reverse_iterator = typename tree_container_type::const_reverse_iterator;

public:
	Tree( Tree && ) = delete;

	Tree( const Tree & ) = delete;

	Tree & operator= ( Tree && ) = delete;

	Tree & operator= ( const Tree & ) = delete;

	Tree( const input_type & input = input_type(), const output_type & output = output_type(), const allocator_type & alloc = allocator_type() )
		: _Input( input ), _Output( output ), _Tree( alloc )
	{

	}

	~Tree() = default;

public:
	tier_container_type & operator[]( size_type val )
	{
		return _Tree[val];
	}

	const tier_container_type & operator[]( size_type val ) const
	{
		return _Tree[val];
	}

public:
	iterator begin()
	{
		return _Tree.begin();
	}

	iterator end()
	{
		return _Tree.end();
	}

	const_iterator begin() const
	{
		return _Tree.begin();
	}

	const_iterator end() const
	{
		return _Tree.end();
	}

	const_iterator cbegin() const
	{
		return _Tree.cbegin();
	}

	const_iterator cend() const
	{
		return _Tree.cend();
	}

	reverse_iterator rbegin()
	{
		return _Tree.rbegin();
	}

	reverse_iterator rend()
	{
		return _Tree.rend();
	}

	const_reverse_iterator rbegin() const
	{
		return _Tree.rbegin();
	}

	const_reverse_iterator rend() const
	{
		return _Tree.rend();
	}

	const_reverse_iterator crbegin() const
	{
		return _Tree.crbegin();
	}

	const_reverse_iterator crend() const
	{
		return _Tree.crend();
	}

public:
	bool empty() const
	{
		return _Tree.empty();
	}

	size_type size() const
	{
		size_type size = 0;
		for ( const auto & it : _Tree ) size += it.size();
		return size;
	}

	size_type depth() const
	{
		return _Tree.size();
	}

public:
	void clear()
	{
		_Tree.clear();
	}

public:
	void insert( const value_type & vertex )
	{
		size_t tier = 0;
		auto inputs = _Input( vertex );
		auto outputs = _Output( vertex );

		if ( std::tuple_size_v< decltype( inputs )> == 0 )
		{
			if ( !_Tree.empty() )
			{
				_Tree.resize( 1 );
			}

			_Tree[0].push_back( vertex );
		}
		else
		{
			for ( size_t tier_i = 0; tier_i < _Tree.size(); tier_i++ )
			{
				for ( const auto & it : _Tree[tier_i] )
				{
					if ( compare( inputs, _Output( it ) ) )
					{
						tier = tier_i + 1;
						break;
					}
				}
			}

			if ( _Tree.size() >= tier )
			{
				_Tree.resize( tier + 1 );
			}

			_Tree[tier].push_back( vertex );
		}

		for ( size_t i = 0; i <= tier; i++ )
		{
			for ( auto it = _Tree[i].begin(); it != _Tree[i].end();  )
			{
				if ( compare( _Input( *it ), outputs ) )
				{
					if ( _Tree.size() <= tier + 1 )
					{
						_Tree.resize( tier + 2 );
					}

					_Tree[tier + 1].push_back( *it );

					it = _Tree[i].erase( it );
				}
				else
				{
					++it;
				}
			}
		}
	}

	void insert( std::initializer_list< value_type > && vertexs )
	{
		insert( vertexs.begin(), vertexs.end() );
	}

	template< typename Iterator > void insert( Iterator first, Iterator last )
	{
		for ( ; first != last; ++first )
		{
			insert( *first );
		}
	}

private:
	template< typename ... Ins, typename ... Outs > bool compare( const std::tuple< Ins... > & input, const std::tuple< Outs... > & output )
	{
		bool is_exist = false;
		XE::ApplyTuple( [&is_exist, &output]( auto val )
			{
				if constexpr ( XE::TupleHasType< decltype( val ) >( output ) )
				{
					if ( is_exist == false )
					{
						is_exist = std::equal_to< decltype( val ) >()( std::get< decltype( val ) >( output ), val );
					}
				}
			}, input );
		return is_exist;
	}

private:
	input_type _Input;
	output_type _Output;
	tree_container_type _Tree;
};

END_XE_NAMESPACE

#endif//TREE_HPP__0988C9B3_974D_45EC_9BB3_ABD1F0EDE45E

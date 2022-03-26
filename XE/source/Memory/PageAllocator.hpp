/*!
 * \file	PageAllocator.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/12/23
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PAGEALLOCATOR_HPP__E049C317_D368_47DC_83CB_299724483E3B
#define PAGEALLOCATOR_HPP__E049C317_D368_47DC_83CB_299724483E3B

#include <bitset>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T > class ObjectPage
{
public:
	using value_type = T;
	using allocate_type = T *;

public:
	ObjectPage()
	{

	}

	ObjectPage( ObjectPage && val )
	{
		std::swap( _Pools, val._Pools );
	}

	ObjectPage( const ObjectPage & ) = delete;

	~ObjectPage()
	{
		for( auto & it : _Pools )
		{
			delete[] static_cast<allocate_type *>( it );
		}
		_Pools.clear();
	}

public:
	bool IsEmpty() const
	{
		return _Pools.size() == 0;
	}

	bool HasAllocate( XE::uint64 val ) const
	{
		return _Pools < 64;
	}

	bool HasDeallocate( const allocate_type & val ) const
	{
		return std::find( _Pools.begin(), _Pools.end(), val ) != _Pools.end();
	}

public:
	allocate_type Allocate( XE::uint64 val )
	{
		return static_cast<allocate_type>( new value_type[val] );
	}

	void Deallocate( const allocate_type & val )
	{
		auto it = std::find( _Pools.begin(), _Pools.end(), val );
		if( it != _Pools.end() )
		{
			delete[] static_cast<allocate_type *>( *it );

			_Pools.erase( it );
		}
	}

private:
	XE::Array< allocate_type > _Pools;
};

template< typename Pt, typename At = std::pmr::polymorphic_allocator< Pt > > class PageAllocator
{
public:
	using page_type = Pt;
	using alloc_type = At;
	using value_type = typename Pt::value_type;
	using allocate_page_type = XE::Delegate< Pt() >;
	using deallocate_page_type = XE::Delegate< void( Pt & ) >;

public:
	PageAllocator( allocate_page_type allocate_page = nullptr, deallocate_page_type deallocate_page = nullptr, const alloc_type & alloc = alloc_type() )
		:_AllocatePage( allocate_page ), _DeallocatePage( deallocate_page ), _Pages( alloc )
	{

	}

	PageAllocator( PageAllocator && val )
	{
		std::swap( _Pages, val._Pages );
		std::swap( _AllocatePage, val._AllocatePage );
		std::swap( _DeallocatePage, val._DeallocatePage );
	}

	PageAllocator( const PageAllocator & ) = delete;

	~PageAllocator()
	{
		for( auto & it : _Pages )
		{
			_DeallocatePage( it );
		}
		_Pages.clear();
	}

public:
	PageAllocator & operator=( PageAllocator && val )
	{
		std::swap( _Pages, val._Pages );
		std::swap( _AllocatePage, val._AllocatePage );
		std::swap( _DeallocatePage, val._DeallocatePage );

		return *this;
	}

public:
	value_type Allocate( XE::uint64 val = 1 )
	{
		for( auto it = _Pages.begin(); it != _Pages.end(); ++it )
		{
			if( std::invoke( &Pt::HasAllocate, std::ref( *it ), val ) )
			{
				return std::invoke( &Pt::Allocate, std::ref( _Pages.back() ), val );
			}
		}

		if( _AllocatePage )
		{
			_Pages.push_back( _AllocatePage() );
		}
		else
		{
			_Pages.push_back( Pt() );
		}

		return std::invoke( &Pt::Allocate, std::ref( _Pages.back() ), val );
	}

	void Deallocate( const value_type & val )
	{
		for( auto it = _Pages.begin(); it != _Pages.end(); ++it )
		{
			if( std::invoke( &Pt::HasDeallocate, std::ref( *it ), val ) )
			{
				std::invoke( &Pt::Deallocate, std::ref( *it ), val );

				if( std::invoke( &Pt::IsEmpty, std::ref( *it ) ) )
				{
					if( _DeallocatePage )
					{
						_DeallocatePage( *it );
					}

					_Pages.erase( it );
				}

				return;
			}
		}
	}

private:
	XE::List< page_type > _Pages;
	allocate_page_type _AllocatePage;
	deallocate_page_type _DeallocatePage;
};

END_XE_NAMESPACE

#endif//PAGEALLOCATOR_HPP__E049C317_D368_47DC_83CB_299724483E3B

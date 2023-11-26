#include "GCMemoryResource.h"

#include <array>

#include "MMapFile.h"
#include "GCObject.h"
#include "StackMemoryResource.hpp"

#include "Base/Bitmap.h"
#include "Base/ConcurrentList.hpp"
#include "Base/ConcurrentQueue.hpp"

namespace
{
	static constexpr XE::uint64 MIN_ALIGN_SIZE = 16;
	static constexpr XE::uint64 MNI_PAGE_SIZE = MBYTE( 4 );
	static constexpr XE::int64 INIT_THRESHOLD_SIZE = MBYTE( 400 );
	static constexpr std::array<XE::uint64, 64> GROUP_SIZE_ARRAY =
	{
		  16,  32,  48,  64,  80,  96, 112, 128,
		 144, 160, 176, 192, 208, 224, 240, 256,
		 272, 288, 304, 320, 336, 352, 368, 384,
		 400, 416, 432, 448, 464, 480, 496, 512,
		 528, 544, 560, 576, 592, 608, 624, 640,
		 656, 672, 688, 704, 720, 736, 752, 768,
		 784, 800, 816, 832, 848, 864, 880, 896,
		 912, 928, 944, 960, 976, 992, 1008, 1024
	};

	enum class PhaseType : XE::uint8
	{
		NONE,
		INITIAL,
		MARKER,
		MARKER_THREAD,
		AGAIN,
		RECYCL,
		RECYCL_THREAD,
	};

	struct MemoryPage
	{
	public:
		MemoryPage( XE::uint64 size, XE::uint64 align )
			: _MMap( ALIGNED( std::max( size, MNI_PAGE_SIZE ), align ) ), _Align( align )
		{
			_Bits.Reset( size / align );
		}

	public:
		XE::uint64 Size() const
		{
			return _MMap.GetSize();
		}

		XE::uint64 Align() const
		{
			return _Align;
		}

		XE::uint8 * Begin() const
		{
			return _MMap.GetAddress();
		}

		XE::uint8 * End() const
		{
			return _MMap.GetAddress() + _MMap.GetSize();
		}

		XE::uint8 * FirstUsePtr()
		{
			auto pos = _Bits.FindFirstTrue();

			if ( pos == XE::Bitmap::npos )
			{
				return nullptr;
			}

			return Begin() + pos;
		}

		XE::uint8 * NextUsePtr( XE::uint8 * ptr )
		{
			auto pos = _Bits.FindSeriesTrue( ( ptr - Begin() ) / _Align );

			if ( pos == XE::Bitmap::npos )
			{
				return nullptr;
			}

			return Begin() + pos;
		}

		XE::uint64 UseSize() const
		{
			return _Bits.TrueCount() * _Align;
		}

		bool IsOwner( XE::uint8 * ptr ) const
		{
			return ptr >= Begin() && ptr < End();
		}

		XE::uint8 * Alloc( XE::uint64 size )
		{
			std::unique_lock< std::mutex > lock( _Mutex );

			size = ALIGNED( size, _Align );

			XE::uint64 i = _Bits.FindSeriesFalse( size / _Align );
			if (i != XE::Bitmap::npos )
			{
				_Bits.Set( i, size / _Align, true );

				return Begin() + ( i * _Align );
			}

			return nullptr;
		}

		void Free( XE::uint8 * ptr, XE::uint64 size )
		{
			std::unique_lock< std::mutex > lock( _Mutex );

			size = ALIGNED( size, _Align );

			_Bits.Set( ( ptr - Begin() ) / _Align, size / _Align, false );
		}

	private:
		XE::Bitmap _Bits;
		XE::uint64 _Align;
		std::mutex _Mutex;
		XE::MMapFile _MMap;
	};
	using MemoryPagePtr = std::unique_ptr< MemoryPage >;
}

struct XE::GCMemoryResource::Private
{
	std::mutex _Mutex;
	XE::ConcurrentList< MemoryPagePtr > _FreeLists;
	XE::ConcurrentList< MemoryPagePtr > _BigMemLists;
	std::array< XE::ConcurrentList< MemoryPagePtr >, 32 > _GroupLists;

	std::atomic< PhaseType > _Phase = PhaseType::NONE;
	XE::Set< const XE::GCRootObject * > _Roots;
	XE::ConcurrentQueue< const XE::GCObject * > _Marks;
	XE::ConcurrentQueue< const XE::GCObject * > _Remarks;

	XE::int64 _Threshold = INIT_THRESHOLD_SIZE;
	XE::uint64 _TotalUsed = 0;
	static XE::GCMemoryResource * _Instance;
};
XE::GCMemoryResource * XE::GCMemoryResource::Private::_Instance = nullptr;

XE::GCMemoryResource::GCMemoryResource()
	:_p( new Private )
{
	Private::_Instance = this;
}

XE::GCMemoryResource::~GCMemoryResource() noexcept
{
	Private::_Instance = nullptr;

	delete _p;
}

void * XE::GCMemoryResource::do_allocate( size_t _Bytes, size_t _Align )
{
	_Align = std::max( _Align, MIN_ALIGN_SIZE );
	_Bytes = ALIGNED( _Bytes, _Align );

	void * p = nullptr;
	XE::ConcurrentList< MemoryPagePtr > * cur_list = nullptr;

	auto it = std::upper_bound( GROUP_SIZE_ARRAY.begin(), GROUP_SIZE_ARRAY.end(), _Bytes );
	if ( it != GROUP_SIZE_ARRAY.end() )
	{
		_Align = *it;
		cur_list = &_p->_GroupLists[std::distance( GROUP_SIZE_ARRAY.begin(), it )];
	}
	else
	{
		cur_list = &_p->_BigMemLists;
	}

	for ( auto & it : *cur_list )
	{
		p = it->Alloc( _Bytes );
		if ( p != nullptr )
		{
			break;
		}
	}

	if ( p == nullptr )
	{
		MemoryPagePtr page;

		for ( auto it = _p->_FreeLists.begin(); it != _p->_FreeLists.end(); ++it )
		{
			if ( ( *it )->Size() > _Bytes )
			{
				page = std::move( *it );
				_p->_FreeLists.erase( it );
				break;
			}
		}

		if ( page == nullptr )
		{
			page = std::make_unique< MemoryPage >( _Bytes, _Align );
		}

		p = page->Alloc( _Bytes );

		cur_list->emplace_back( std::move( page ) );
	}

	_p->_Threshold -= _Bytes;
	_p->_TotalUsed += _Bytes;

	return p;
}

void XE::GCMemoryResource::do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align )
{

}

bool XE::GCMemoryResource::do_is_equal( const std::pmr::memory_resource & _That ) const noexcept
{
	return &_That == this;
}

void XE::GCMemoryResource::GC()
{
	auto _p = Private::_Instance->_p;

	switch ( _p->_Phase )
	{
	case PhaseType::NONE:
	{
		if ( _p->_Threshold <= 0 )
		{
			_p->_Phase = PhaseType::INITIAL;
		}
	}
	break;
	case PhaseType::INITIAL:
	{
		std::unique_lock< std::mutex > lock( _p->_Mutex );

		for ( const auto & it : _p->_Roots )
		{
			it->Mark();
			const_cast< XE::GCRootObject *>( it )->_Status = XE::GCStatus::BLACK;
		}

		_p->_Phase = PhaseType::MARKER;
	}
	break;
	case PhaseType::MARKER:
	{
		_p->_Phase = PhaseType::MARKER_THREAD;
		std::thread( [_p]()
		{
			const XE::GCObject * ptr = nullptr;
			while ( _p->_Marks.try_pop( ptr ) )
			{
				Mark( ptr );
			}
			_p->_Phase = PhaseType::AGAIN;
		} ).detach();
	}
	break;
	case PhaseType::AGAIN:
	{
		std::unique_lock< std::mutex > lock( _p->_Mutex );

		const XE::GCObject * ptr = nullptr;
		while ( _p->_Remarks.try_pop( ptr ) )
		{
			Mark( ptr );
		}

		_p->_Phase = PhaseType::RECYCL;
	}
	break;
	case PhaseType::RECYCL:
	{
		_p->_Phase = PhaseType::RECYCL_THREAD;
		std::thread( []()
		{
			Private::_Instance->Recycl();
		} ).detach();
	}
	break;
	default:
		break;
	}
}

void XE::GCMemoryResource::Mark( const XE::GCObject * _Ptr )
{
	auto _p = Private::_Instance->_p;

	if ( _Ptr != nullptr )
	{
		switch ( _Ptr->_Status )
		{
		case XE::GCStatus::WHITE:
		{
			const_cast< XE::GCObject * >( _Ptr )->_Status = XE::GCStatus::GREY;
			_p->_Marks.push( _Ptr );
		}
		break;
		case XE::GCStatus::GREY:
		{
			_Ptr->Mark();
			const_cast<XE::GCObject *>( _Ptr )->_Status = XE::GCStatus::BLACK;
		}
		break;
		case XE::GCStatus::BLACK:
			break;
		}
	}
}

void XE::GCMemoryResource::Barrier( const XE::GCObject * _Ptr )
{
	auto _p = Private::_Instance->_p;

	std::unique_lock< std::mutex > lock( _p->_Mutex );

	if ( _Ptr != nullptr && ( _p->_Phase == PhaseType::MARKER || _p->_Phase == PhaseType::MARKER_THREAD ) && _Ptr->_Status == XE::GCStatus::WHITE )
	{
		_p->_Remarks.push( _Ptr );
		const_cast<XE::GCObject *>( _Ptr )->_Status = XE::GCStatus::GREY;
	}
}

void XE::GCMemoryResource::Register( const GCRootObject * root )
{
	auto _p = Private::_Instance->_p;

	std::unique_lock< std::mutex > lock( _p->_Mutex );

	_p->_Roots.insert( root );
}

void XE::GCMemoryResource::Unregister( const GCRootObject * root )
{
	auto _p = Private::_Instance->_p;

	std::unique_lock< std::mutex > lock( _p->_Mutex );

	_p->_Roots.erase( root );
}

void XE::GCMemoryResource::Recycl()
{
	XE::StackMemoryResource<MBYTE( 1 )> res;
	{
		XE::Array< XE::ConcurrentList< MemoryPagePtr > * > lists( &res );

		for ( auto & list : _p->_GroupLists )
		{
			lists.push_back( &list );
		}
		lists.push_back( &_p->_BigMemLists );

		for ( auto list : lists )
		{
			for ( auto it = list->begin(); it != list->end(); ++it )
			{
				XE::GCObject * ptr = reinterpret_cast<XE::GCObject *>( ( *it )->FirstUsePtr() );
				do
				{
					if ( ptr != nullptr )
					{
						if ( ptr->_Status == XE::GCStatus::WHITE )
						{
							auto size = ptr->Size();

							ptr->~GCObject();

							( *it )->Free( reinterpret_cast<XE::uint8 *>( ptr ), size );

							_p->_TotalUsed -= size;
						}
						else
						{
							ptr->_Status = XE::GCStatus::WHITE;
						}
					}
				} while ( ptr = reinterpret_cast<XE::GCObject *>( ( *it )->NextUsePtr( reinterpret_cast<XE::uint8 *>( ptr ) ) ) );
				if ( ( *it )->UseSize() == 0 )
				{
					MemoryPagePtr page = std::move( *it );
					list->erase( it );
					_p->_FreeLists.emplace_back( std::move( page ) );
				}
			}
		}

		while ( _p->_FreeLists.size() > 2 )
		{
			_p->_FreeLists.pop_back();
		}

		_p->_Threshold += static_cast<XE::int64>( _p->_TotalUsed * 1.5 );
		_p->_Phase = PhaseType::NONE;
	}
}

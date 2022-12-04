#include "GCMemoryResource.h"

#include "MMapFile.h"
#include "GCObject.h"

namespace
{
	static constexpr XE::uint64 MAX_MEMORY_SIZE = GBYTE( 4 );
	static constexpr XE::int64 INIT_THRESHOLD_SIZE = MBYTE( 400 );

	enum class Status : XE::uint8
	{
		WHITE = 0,
		BLACK,
		GREY,
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

	XE_INLINE Status GetStatus( void * ptr )
	{
		XE::uint8 * p = reinterpret_cast<XE::uint8 *>( ptr );

		p -= sizeof( XE::uint8 );

		return Status( *p & 0b00000111 );
	}

	XE_INLINE void SetStatus( void * ptr, Status status )
	{
		XE::uint8 * p = reinterpret_cast<XE::uint8 *>( ptr );

		p -= sizeof( XE::uint8 );

		*p = static_cast<XE::uint8>( status ) | ( *p & 0b11111000 );
	}

	struct Page
	{
		XE::MMapFile MMap;

		bool IsOwner( void * ptr ) const
		{
			return reinterpret_cast<XE::uint8 *>( ptr ) >= MMap.GetAddress() && reinterpret_cast<XE::uint8 *>( ptr ) < ( MMap.GetAddress() + MMap.GetSize() );
		}

		void * Alloc( XE::uint64 size )
		{

		}

		void Free( void * ptr )
		{

		}
	};

	struct Group
	{
		XE::uint64 Size;
		XE::ConcurrentList< Page * > Pages;
	};
}

struct XE::GCMemoryResource::Private
{
	std::shared_mutex _Mutex;
	
	XE::ConcurrentList< Page * > _Pages;

	std::atomic< PhaseType > _Phase = PhaseType::NONE;
	XE::Set< XE::GCRootObject * > _Roots;
	XE::ConcurrentQueue< XE::GCObject * > _Marks;
	XE::ConcurrentQueue< XE::GCObject * > _Remarks;

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
	void * p = nullptr;

	std::shared_lock< std::shared_mutex > lock( _p->_Mutex );
	{

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
		std::unique_lock< std::shared_mutex > lock( _p->_Mutex );

		for ( const auto & it : _p->_Roots )
		{
			it->Mark();
			SetStatus( it, Status::BLACK );
		}

		_p->_Phase = PhaseType::MARKER;
	}
	break;
	case PhaseType::MARKER:
	{
		_p->_Phase = PhaseType::MARKER_THREAD;
		std::thread( [_p]()
		{
			XE::GCObject * ptr = nullptr;
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
		std::unique_lock< std::shared_mutex > lock( _p->_Mutex );

		XE::GCObject * ptr = nullptr;
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
		std::thread( [_p]()
		{
		// TODO: 

			_p->_Threshold += static_cast<XE::int64>( _p->_TotalUsed * 1.5 );
			_p->_Phase = PhaseType::NONE;
		} ).detach();
	}
	break;
	default:
		break;
	}
}

void XE::GCMemoryResource::Mark( XE::GCObject * _Ptr )
{
	auto _p = Private::_Instance->_p;

	if ( _Ptr != nullptr )
	{
		switch ( GetStatus( _Ptr ) )
		{
		case Status::WHITE:
		{
			std::shared_lock< std::shared_mutex > lock( _p->_Mutex );
			SetStatus( _Ptr, Status::GREY );
			_p->_Marks.push( _Ptr );
		}
		break;
		case Status::GREY:
		{
			std::shared_lock< std::shared_mutex > lock( _p->_Mutex );
			_Ptr->Mark();
			SetStatus( _Ptr, Status::BLACK );
		}
		break;
		case Status::BLACK:
			break;
		}
	}
}

void XE::GCMemoryResource::Barrier( XE::GCObject * _Ptr )
{
	auto _p = Private::_Instance->_p;

	if ( _Ptr != nullptr && ( _p->_Phase == PhaseType::MARKER || _p->_Phase == PhaseType::MARKER_THREAD ) && GetStatus( _Ptr ) == Status::WHITE )
	{
		_p->_Remarks.push( _Ptr );
		SetStatus( _Ptr, Status::GREY );
	}
}

void XE::GCMemoryResource::Register( GCRootObject * root )
{
	auto _p = Private::_Instance->_p;

	std::shared_lock< std::shared_mutex > lock( _p->_Mutex );

	_p->_Roots.insert( root );
}

void XE::GCMemoryResource::Unregister( GCRootObject * root )
{
	auto _p = Private::_Instance->_p;

	std::shared_lock< std::shared_mutex > lock( _p->_Mutex );

	_p->_Roots.erase( root );
}

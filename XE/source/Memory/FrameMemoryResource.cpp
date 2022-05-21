#include "FrameMemoryResource.h"

#include "MemoryResource.h"

XE::FrameMemoryResource::FrameMemoryResource()
	:_Data( XE::MemoryResource::GetDefaultMemoryResource()->allocate( MBYTE( 2 ) ) ), _Size( MBYTE( 2 ) )
{
	_Current = new( _Data )Node;

	_Current->prev = nullptr;
	_Current->next = _Current;
	_Current->size = _Size;
}

XE::FrameMemoryResource::~FrameMemoryResource() noexcept
{
	XE::MemoryResource::GetDefaultMemoryResource()->deallocate( _Data, _Size );
}

void * XE::FrameMemoryResource::do_allocate( size_t _Bytes, size_t _Align )
{
	Node * current = _Current;
	auto min_size = std::min( _Bytes, sizeof( Node ) );

	while ( current != nullptr && current->size < min_size )
	{
		current = current->next;
	}

	if ( current != nullptr )
	{
		auto size = current->size;
		auto prev = current->prev;
		auto next = current->next;

		void * result = static_cast<void *>( current );

		if ( size - min_size > sizeof( Node ) )
		{
			_Current = new ( current + min_size )Node;

			_Current->prev = prev;
			_Current->next = next;
			_Current->size = min_size - size;
		}
		else
		{
			_Current = next;
			_Current->prev = nullptr;
		}

		return result;
	}

	return XE::MemoryResource::GetDefaultMemoryResource()->allocate( _Bytes, _Align );
}

void XE::FrameMemoryResource::do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align )
{
	auto min_size = std::min( _Bytes, sizeof( Node ) );

	if ( _Ptr >= _Data && _Ptr < ( static_cast<XE::uint8 *>( _Data ) + min_size ) )
	{
		auto node = _Current;

		if ( _Ptr < node )
		{
			while ( _Ptr < node && node->prev != nullptr )
			{
				node = node->prev;
			}

			_Current = new ( _Ptr ) Node;
			_Current->next = node;
			_Current->prev = node->prev;
			_Current->size = min_size;
			node->prev = _Current;

			node = _Current;
		}
		else
		{
			while ( _Ptr > node && node->next != nullptr )
			{
				node = node->next;
			}

			if ( node + ( node->size + min_size ) == _Ptr )
			{
				node->size += min_size;
			}
			else
			{
				auto n = new ( _Ptr )Node;
				n->prev = node;
				n->next = node->next;
				n->size = min_size;
				node->next = n;

				node = n;
			}
		}

		if ( node + node->size == node->next )
		{
			node->size += node->next->size;
			node->next = node->next->next;
			node->next->prev = node;
		}
	}
	else
	{
		XE::MemoryResource::GetDefaultMemoryResource()->deallocate( _Ptr, _Bytes, _Align );
	}
}

bool XE::FrameMemoryResource::do_is_equal( const std::pmr::memory_resource & _That ) const noexcept
{
	return &_That == this;
}

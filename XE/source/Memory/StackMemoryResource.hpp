/*!
 * \file	StackMemoryResource.hpp
 *
 * \author	ZhengYuanQing
 * \date	2022/05/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef STACKMEMORYRESOURCE_HPP__81B98080_8CEE_4E0F_9FF6_4239DB46F11B
#define STACKMEMORYRESOURCE_HPP__81B98080_8CEE_4E0F_9FF6_4239DB46F11B

#include <array>

#include "MemoryResource.h"
#include "DefaultMemoryResource.h"

BEG_XE_NAMESPACE

template< XE::uint64 S > class StackMemoryResource : public std::pmr::memory_resource
{
	static constexpr XE::uint64 _Size = S;

public:
	StackMemoryResource() = default;

	~StackMemoryResource() noexcept override = default;

private:
	void * do_allocate( size_t _Bytes, size_t _Align ) override
	{
		auto size = ALIGNED( _Bytes, _Align );

		if ( _Size - _Pos >= size )
		{
			auto p = _Pos;
			_Pos += size;

			return p + _Data.data();
		}

		return XE::MemoryResource::GetDefaultMemoryResource()->allocate( size );
	}

	void do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align ) override
	{
		auto size = ALIGNED( _Bytes, _Align );

		if ( _Ptr < _Data.data() && _Ptr >= _Data.data() + _Size )
		{
			XE::MemoryResource::GetDefaultMemoryResource()->deallocate( _Ptr, _Bytes, _Align );
		}
	}

	bool do_is_equal( const std::pmr::memory_resource & _That ) const noexcept override
	{
		return &_That == this;
	}

private:
	XE::uint64 _Pos = 0;
	std::array< XE::uint8, S > _Data;
};

END_XE_NAMESPACE

#endif//STACKMEMORYRESOURCE_HPP__81B98080_8CEE_4E0F_9FF6_4239DB46F11B

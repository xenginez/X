/*!
 * \file	concurrent.hpp
 *
 * \author	ZhengYuanQing
 * \date	2021/07/14
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CONCURRENT_H__FBC498D1_BC14_4F2C_8264_9266C236C648
#define CONCURRENT_H__FBC498D1_BC14_4F2C_8264_9266C236C648

#include <shared_mutex>

#include "Type.h"

BEG_XE_NAMESPACE

template< typename T > class Concurrent< T, std::mutex >
{
public:
	using value_type = T;
	using reference_type = T &;
	using const_reference_type = const T &;

	using mutex_type = std::mutex;
	using unique_lock_type = std::unique_lock< std::mutex >;

	using call_type = XE::Delegate<void( T & )>;
	using const_call_type = XE::Delegate<void( const T & )>;

public:
	void operator()( const call_type & call )
	{
		unique( call );
	}

	void operator()( const const_call_type & call ) const
	{
		unique( call );
	}

public:
	void unique( const call_type & call )
	{
		unique_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

	void unique( const call_type & call ) const
	{
		unique_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

public:
	reference_type unsafe()
	{
		return _value;
	}

	const_reference_type unsafe() const
	{
		return _value;
	}

private:
	mutex_type _mutex;
	value_type _value;
};

template< typename T > class Concurrent< T, std::shared_mutex >
{
public:
	using value_type = T;
	using reference_type = T &;
	using const_reference_type = const T &;

	using mutex_type = std::shared_mutex;
	using unique_lock_type = std::unique_lock< std::shared_mutex >;
	using shared_lock_type = std::shared_lock< std::shared_mutex >;

	using call_type = XE::Delegate<void( T & )>;
	using const_call_type = XE::Delegate<void( const T & )>;

public:
	void operator()( const call_type & call )
	{
		unique( call );
	}

	void operator()( const const_call_type & call ) const
	{
		unique( call );
	}

	void operator[]( const call_type & call )
	{
		shared( call );
	}

	void operator[]( const const_call_type & call ) const
	{
		shared( call );
	}

public:
	void unique( const call_type & call )
	{
		unique_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

	void unique( const call_type & call ) const
	{
		unique_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

	void shared( const call_type & call )
	{
		shared_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

	void shared( const call_type & call ) const
	{
		shared_lock_type lock( _mutex );

		call( std::ref( _value ) );
	}

public:
	reference_type unsafe()
	{
		return _value;
	}

	const_reference_type unsafe() const
	{
		return _value;
	}

private:
	mutex_type _mutex;
	value_type _value;
};

END_XE_NAMESPACE

#endif // CONCURRENT_H__FBC498D1_BC14_4F2C_8264_9266C236C648

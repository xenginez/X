#include "TimerService.h"

#include "Utils/Concurrent.hpp"

BEG_META( XE::TimerService )
END_META()

#define T_INTERVAR (10)

#define TF_BITS (8)
#define TF_SIZE (1ULL << TF_BITS)
#define TF_MASK (TF_SIZE - 1)
#define TF_INDEX(tick) (tick & TF_MASK)

#define TN_BITS (6)
#define TN_COUNT (4)
#define TN_SIZE (1ULL << TN_BITS)
#define TN_MASK (TN_SIZE - 1)
#define TN_INDEX(tick, n) (((tick) >> (TF_BITS + ((n) * TN_BITS))) & TN_MASK)

struct TimerWheel
{
	using CallbackListType = XE::ConcurrentList< XE::Delegate< void() > >;

	struct Item
	{
		XE::uint32 _Tick = 0;
		std::chrono::milliseconds _Expire;
		CallbackListType::const_iterator _Idx;
	};

	XE::float32 _DeltaTime = 0;
	XE::uint32 _CurrentTick = 0;
	std::array< std::list< Item >, TF_SIZE > _First;
	XE::MultiArray< std::list< Item >, TN_COUNT, TN_SIZE > _Second;

	CallbackListType _Callbacks;

	void insert( Item && item )
	{
		XE::uint32 count = ( ( item._Expire.count() + 1 ) / T_INTERVAR ) + ( ( item._Tick + 1 ) / T_INTERVAR );
		XE::uint32 index = count - _CurrentTick;
		if( index < TF_SIZE )
		{
			_First[TF_INDEX( count )].push_back( item );
		}
		else
		{
			for( size_t i = 0; i < _Second.size(); i++ )
			{
				if( index < ( 1LL << ( TN_BITS + ( i + 1 ) * TN_BITS ) ) )
				{
					_Second[i][TN_INDEX( count, i )].push_back( item );
					break;
				}
			}
		}
	}
	void update( XE::float32 dt )
	{
		std::list< Item > lists1, lists2;

		_DeltaTime += dt;

		for( auto tick = XE::int64( (XE::float64)_DeltaTime * ( 1000 / T_INTERVAR ) ); tick > 0; tick-- )
		{
			lists1.insert( lists1.end(), _First[TF_INDEX( _CurrentTick )].begin(), _First[TF_INDEX( _CurrentTick )].end() );
			_First[TF_INDEX( _CurrentTick )].clear();

			for( XE::uint64 i = 0; i < _Second.size(); i++ )
			{
				lists2.insert( lists2.end(), _Second[i][TN_INDEX( _CurrentTick, i )].begin(), _Second[i][TN_INDEX( _CurrentTick, i )].end() );
				_Second[i][TN_INDEX( _CurrentTick, i )].clear();
			}

			_CurrentTick++;
			_DeltaTime -= 0.01f;
		}

		for( auto & it : lists1 )
		{
			if( *it._Idx != nullptr )
			{
				( *it._Idx )();

				it._Tick = _CurrentTick;

				insert( std::move( it ) );
			}
		}

		for( auto & it : lists2 )
		{
			insert( std::move( it ) );
		}
	}
};

struct XE::TimerService::Private
{
	bool _IsPause = false;
	XE::uint64 _FrameCount = 0;
	XE::float32 _TimeScale = 1.0f;
	XE::float32 _DetlaTime = 0.0f;
	XE::float32 _MinDetlaTime = 0.0f;
	XE::float32 _MaxDetlaTime = 1.0f / 30.0f;
	XE::float32 _FixedDetlaTime = 1.0f / 30.0f;
	std::chrono::high_resolution_clock::time_point _StartTime;
	std::chrono::high_resolution_clock::time_point _FrameTime;

	std::mutex _TimerWheelMutex;
	std::mutex _UnscaleTimerWheelMutex;
	std::unique_ptr< TimerWheel > _TimerWheel;
	std::unique_ptr< TimerWheel > _UnscaleTimerWheel;
};

XE::TimerService::TimerService()
	:_p( XE::New< Private >() )
{

}

XE::TimerService::~TimerService()
{
	XE::Delete( _p );
}

void XE::TimerService::Prepare()
{

}

void XE::TimerService::Startup()
{
	auto now_time = std::chrono::high_resolution_clock::now();
	
	_p->_StartTime = now_time;
	_p->_FrameTime = now_time;

	_p->_TimerWheel = std::make_unique<TimerWheel>();
	_p->_UnscaleTimerWheel = std::make_unique<TimerWheel>();
}

void XE::TimerService::Update()
{
	_p->_FrameCount++;

	auto now_time = std::chrono::high_resolution_clock::now();
	_p->_DetlaTime = ( std::chrono::duration_cast< std::chrono::milliseconds >( now_time - _p->_FrameTime ).count() ) / 1000.0f;
	_p->_DetlaTime = std::clamp( _p->_DetlaTime, _p->_MinDetlaTime, _p->_MaxDetlaTime );
	_p->_FrameTime = now_time;

	_p->_TimerWheel->update( GetDeltaTime() );
	_p->_UnscaleTimerWheel->update( GetUnscaleDeltaTime() );
}

void XE::TimerService::Clearup()
{
	_p->_TimeScale = 1.0f;
	_p->_DetlaTime = 0.0f;
	_p->_MinDetlaTime = 1.0f / 30.0f;
	_p->_FixedDetlaTime = 1.0f / 30.0f;

	_p->_TimerWheel = nullptr;
	_p->_UnscaleTimerWheel = nullptr;
}

void XE::TimerService::Pause()
{
	_p->_IsPause = true;
}

void XE::TimerService::Continue()
{
	_p->_IsPause = false;
}

bool XE::TimerService::IsPause() const
{
	return _p->_IsPause;
}

XE::uint64 XE::TimerService::GetFrameCount() const
{
	return _p->_FrameCount;
}

XE::float32 XE::TimerService::GetTimeScale() const
{
	return _p->_TimeScale;
}

void XE::TimerService::SetTimeScale( XE::float32 val )
{
	_p->_TimeScale = val;
}

XE::float32 XE::TimerService::GetMinDeltaTime() const
{
	return _p->_MinDetlaTime;
}

void XE::TimerService::SetMinDeltaTime( XE::float32 val )
{
	_p->_MinDetlaTime = val;
}

XE::float32 XE::TimerService::GetMaxDeltaTime() const
{
	return _p->_MaxDetlaTime;
}

void XE::TimerService::SetMaxDeltaTime( XE::float32 val )
{
	_p->_MaxDetlaTime = val;
}

XE::float32 XE::TimerService::GetTime() const
{
	return ( std::chrono::duration_cast< std::chrono::milliseconds >( _p->_FrameTime - _p->_StartTime ).count() ) / 1000.0f;
}

XE::float32 XE::TimerService::GetDeltaTime() const
{
	return _p->_DetlaTime * _p->_TimeScale;
}

XE::float32 XE::TimerService::GetFixedDeltaTime() const
{
	return _p->_FixedDetlaTime * _p->_TimeScale;
}

XE::float32 XE::TimerService::GetUnscaleDeltaTime() const
{
	return _p->_DetlaTime;
}

XE::float32 XE::TimerService::GetUnscaleFixedDeltaTime() const
{
	return _p->_FixedDetlaTime;
}

XE::Disposable XE::TimerService::StartTimer( const std::chrono::high_resolution_clock::duration & duration, const XE::Delegate<bool()> & callback )
{
	TimerWheel::Item item;

	auto it = _p->_TimerWheel->_Callbacks.emplace_back( nullptr );

	*it = [this, it, callback]()
	{
		if( !callback() )
		{
			_p->_TimerWheel->_Callbacks.erase( it );
		}
	};

	item._Idx = it;
	item._Tick = _p->_TimerWheel->_CurrentTick;
	item._Expire = std::chrono::duration_cast< std::chrono::milliseconds >( duration );

	{
		std::unique_lock< std::mutex > lock( _p->_TimerWheelMutex );

		_p->_TimerWheel->insert( std::move( item ) );
	}

	return { [this, it]() { _p->_TimerWheel->_Callbacks.erase( it ); } };
}

XE::Disposable XE::TimerService::StartUnscaleTimer( const std::chrono::high_resolution_clock::duration & duration, const XE::Delegate<bool()> & callback )
{
	TimerWheel::Item item;

	auto it = _p->_UnscaleTimerWheel->_Callbacks.emplace_back( nullptr );

	*it = [this, it, callback]()
	{
		if( !callback() )
		{
			_p->_TimerWheel->_Callbacks.erase( it );
		}
	};

	item._Idx = it;
	item._Tick = _p->_UnscaleTimerWheel->_CurrentTick;
	item._Expire = std::chrono::duration_cast< std::chrono::milliseconds >( duration );

	{
		std::unique_lock< std::mutex > lock( _p->_UnscaleTimerWheelMutex );

		_p->_UnscaleTimerWheel->insert( std::move( item ) );
	}

	return { [this, it]() { _p->_TimerWheel->_Callbacks.erase( it ); } };
}

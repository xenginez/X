#include "ThreadService.h"
#undef OS_WINDOWS

#include <tbb/concurrent_queue.h>

#ifdef XE_PROFILING
#define MICROPROFILE_ENABLED 1
#else
#define MICROPROFILE_ENABLED 0
#endif // XE_PROFILING


#define MICROPROFILE_IMPL
#include <microprofile/microprofile.h>
#include <microprofile/microprofile.cpp>

BEG_META( XE::ThreadService )
END_META()

struct XEPThread
{
	XEPThread() = default;

	~XEPThread() = default;

	virtual void Handler() = 0;

	virtual void PushTask( const XE::Delegate< void() > & val ) = 0;

	virtual bool HasThreadID( const std::thread::id & val ) = 0;

};

struct XEPMainThread : public XEPThread
{
	XEPMainThread()
		:_CurrentTasks( 0 ), _ID( std::this_thread::get_id() )
	{

	}

	void Handler() override
	{
		tbb::concurrent_queue< XE::Delegate< void() > > * Tasks = _CurrentTasks == 0 ? &_FrontTasks : &_BackTasks;
		_CurrentTasks = ( _CurrentTasks + 1 ) % 2;

		XE::Delegate< void() >  task;
		while( Tasks->try_pop( task ) )
		{
			if( task )
			{
				task();
			}
		}
	}

	void PushTask( const XE::Delegate< void() > & val ) override
	{
		_CurrentTasks == 0 ? _FrontTasks.push( val ) : _BackTasks.push( val );
	}

	bool HasThreadID( const std::thread::id & val )  override
	{
		return _ID == val;
	}

	std::thread::id _ID;
	std::atomic< XE::uint64 > _CurrentTasks;
	tbb::concurrent_queue< XE::Delegate< void() > > _FrontTasks;
	tbb::concurrent_queue< XE::Delegate< void() > > _BackTasks;
};

struct XEPSpecialThread : public XEPThread
{
	XEPSpecialThread( const char * name )
		:_Thread( [this, name]()
				  {
					  MicroProfileOnThreadCreate( name );

					  Handler();

					  MicroProfileOnThreadExit();
				  } )
	{

	}

	~XEPSpecialThread()
	{
		_Exit = true;

		_Variable.notify_all();

		if( _Thread.joinable() )
		{
			_Thread.join();
		}
	}

	void Handler() override
	{
		while( !_Exit )
		{
			std::unique_lock< std::mutex > Lock( _Lock );

			_Variable.wait( Lock, [this]() { return !_Tasks.empty(); } );

			XE::Delegate< void() >  task;
			while( _Tasks.try_pop( task ) )
			{
				if( task )
				{
					task();
				}
			}
		}
	}

	void PushTask( const XE::Delegate< void() > & val ) override
	{
		_Tasks.push( val );

		_Variable.notify_one();
	}

	bool HasThreadID( const std::thread::id & val )  override
	{
		return _Thread.get_id() == val;
	}

	bool _Exit = false;
	std::mutex _Lock;
	std::thread _Thread;
	std::condition_variable _Variable;
	tbb::concurrent_queue< XE::Delegate< void() > > _Tasks;
};

struct XEPWorkThread : public XEPThread
{
	XEPWorkThread()
	{
		for( XE::uint64 i = 0; i < std::thread::hardware_concurrency(); ++i )
		{
			_Threads.emplace_back( [this, i]()
								   {
									   std::string name = "Work" + std::to_string( i );
									   
									   MicroProfileOnThreadCreate( name.c_str() );

									   Handler();

									   MicroProfileOnThreadExit();
								   } );
		}
	}

	~XEPWorkThread()
	{
		_Exit = true;

		_Variable.notify_all();

		for( int i = 0; i < _Threads.size(); ++i )
		{
			if( _Threads[i].joinable() )
			{
				_Threads[i].join();
			}
		}
	}

	void Handler()
	{
		while( !_Exit )
		{
			std::unique_lock< std::mutex > Lock( _Lock );

			_Variable.wait( Lock, [this]() { return !_Tasks.empty(); } );

			XE::Delegate< void() >  task;
			while( _Tasks.try_pop( task ) )
			{
				if( task )
				{
					task();
				}
			}
		}
	}

	void PushTask( const XE::Delegate< void() > & val ) override
	{
		_Tasks.push( val );

		_Variable.notify_one();
	}

	bool HasThreadID( const std::thread::id & val )  override
	{
		for( const auto & it : _Threads )
		{
			if( it.get_id() == val )
			{
				return true;
			}
		}

		return false;
	}

	bool _Exit = false;
	std::mutex _Lock;
	XE::Array< std::thread > _Threads;
	std::condition_variable _Variable;
	tbb::concurrent_queue< XE::Delegate< void() > > _Tasks;
};

struct XE::ThreadService::Private
{
	XE::Array< XEPThread * > _Threads;
};

XE::ThreadService::ThreadService()
	:_p( XE::New< Private >() )
{

}

XE::ThreadService::~ThreadService()
{
	XE::Delete( _p );
}

void XE::ThreadService::Prepare()
{

}

void XE::ThreadService::Startup()
{
	MicroProfileOnThreadCreate( "Game" );

	_p->_Threads.resize( (XE::uint64)XE::ThreadType::UNKNOWN );

	_p->_Threads[(XE::uint64)XE::ThreadType::GAME] = new XEPMainThread();
	_p->_Threads[(XE::uint64)XE::ThreadType::WORKS] = new XEPWorkThread();
	_p->_Threads[(XE::uint64)XE::ThreadType::IO] = new XEPSpecialThread( "IO" );
	_p->_Threads[(XE::uint64)XE::ThreadType::AUDIO] = new XEPSpecialThread( "Audio" );
	_p->_Threads[(XE::uint64)XE::ThreadType::RENDER] = new XEPSpecialThread( "Render" );
	_p->_Threads[(XE::uint64)XE::ThreadType::PHYSICS] = new XEPSpecialThread( "Physics" );
}

void XE::ThreadService::Update()
{
	_p->_Threads[( XE::uint64 )XE::ThreadType::GAME]->Handler();
}

void XE::ThreadService::Clearup()
{
	_p->_Threads[( XE::uint64 )XE::ThreadType::GAME]->Handler();
	_p->_Threads[( XE::uint64 )XE::ThreadType::GAME]->Handler();

	for( auto p : _p->_Threads )
	{
		delete p;
	}

	_p->_Threads.clear();
}

XE::ThreadType XE::ThreadService::GetCurrentThreadType() const
{
	auto id = std::this_thread::get_id();

	for( XE::uint64 i = 0; i < _p->_Threads.size(); ++i )
	{
		if( _p->_Threads[i]->HasThreadID( id ) )
		{
			return ( XE::ThreadType )( i );
		}
	}

	return XE::ThreadType::UNKNOWN;
}
 
void XE::ThreadService::_PostTask( ThreadType type, const XE::Delegate< void() > & task )
{
	if( task )
	{
		_p->_Threads[( XE::uint64 )type]->PushTask( task );
	}
}

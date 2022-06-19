/*!
 * \file   ThreadService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/04
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __THREADSERVICE_H__5E718656_36CC_4C4A_9077_F6887B508EC8
#define __THREADSERVICE_H__5E718656_36CC_4C4A_9077_F6887B508EC8

#include <future>

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API ThreadService : public XE::Service
{
	OBJECT( ThreadService, Service )

private:
	struct Private;

	struct RefCountFuture : public XE::EnableSharedFromThis< RefCountFuture >
	{
		RefCountFuture()
		{
			future = promise.get_future().share();
		}

		~RefCountFuture()
		{
			promise.set_value();
		}

		std::promise< void > promise;
		std::shared_future< void > future;
	};

public:
	ThreadService();

	~ThreadService() override;

public:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::ThreadType GetCurrentThreadType() const;

public:
	template< typename ... Fs > std::shared_future< void > PostTasks( Fs && ... tasks )
	{
		XE::SharedPtr< RefCountFuture > _ref = XE::MakeShared< RefCountFuture >();

		_PostTasks( _ref, tasks... );

		return _ref->future;
	}

public:
	std::shared_future< void > PostTask( ThreadType type, const XE::Delegate< void() > & task )
	{
		XE::SharedPtr< std::promise< void > > promise = XE::MakeShared< std::promise< void > >();

		std::shared_future< void > future = promise->get_future().share();

		_PostTask( type, [task, future, promise]()
				   {
					   task();
					   ( *promise ).set_value();
				   } );

		return future;
	}

	template< typename R > std::shared_future< R > PostTask( ThreadType type, const XE::Delegate< R() > & task )
	{
		XE::SharedPtr< std::promise< R > > promise = XE::MakeShared< std::promise< R > >();

		std::shared_future< R > future = promise->get_future().share();

		_PostTask( type, [task, future, promise]()
				  {
					   ( *promise ).set_value( task() );
				  } );

		return future;
	}

public:
	template< typename Iterator, typename F > std::shared_future< void > ParallelTask( Iterator begin, Iterator end, F && task )
	{
		XE::SharedPtr< RefCountFuture > _ref = XE::MakeShared< RefCountFuture >();

		for ( ; begin != end; ++begin )
		{
			_PostTask( ThreadType::WORKS, [begin, task = std::move( task ), _ref]() mutable
				{
					task( begin );
				} );
		}

		return _ref->future;
	}

	template< typename Iterator, typename F > std::shared_future< void > ParallelStepTask( Iterator begin, Iterator end, XE::uint64 step, F && task )
	{
		XE::SharedPtr< RefCountFuture > _ref = XE::MakeShared< RefCountFuture >();

		Iterator current = begin;

		while( current != end )
		{
			for( XE::uint64 i = 0; i < step && current != end; ++i, ++current );

			_PostTask( ThreadType::WORKS, [begin, current, task = std::move( task ), _ref]() mutable
					  {
						  task( begin, current );
					  } );

			begin = current;
		}

		return _ref->future;
	}

private:
	void _PostTasks( const XE::SharedPtr< RefCountFuture > & _ref )
	{

	}

	template< typename F > void _PostTasks( const XE::SharedPtr< RefCountFuture > & _ref, F && task )
	{
		_PostTask( ThreadType::WORKS, [task, _ref]() { task(); } );
	}

	template< typename F, typename ... Fs > void _PostTasks( const XE::SharedPtr< RefCountFuture > & _ref, F && task, Fs && ... tasks )
	{
		_PostTasks( _ref, task );

		_PostTasks( _ref, tasks... );
	}

private:
	void _PostTask( ThreadType type, const XE::Delegate< void() > & task );

private:
	Private * _p;
};

END_XE_NAMESPACE

#define ASSERT_THREAD( TYPE, MSG ) \
XE_ASSERT( (XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >()->GetCurrentThreadType() != TYPE ) && MSG )

#define CHECK_THREAD( TYPE ) \
(XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >()->GetCurrentThreadType() == TYPE)

#endif // __THREADSERVICE_H__5E718656_36CC_4C4A_9077_F6887B508EC8

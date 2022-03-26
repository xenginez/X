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

	struct _RefCountFuture : public XE::EnableSharedFromThis< _RefCountFuture >
	{
		_RefCountFuture()
		{
			future = promise.get_future().share();
		}

		~_RefCountFuture()
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

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::ThreadType GetCurrentThreadType() const;

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

	template< typename Iterator > std::shared_future< void > ParallelTask( Iterator begin, Iterator end, const XE::Delegate< void( Iterator ) > & task )
	{
		XE::SharedPtr< _RefCountFuture > _ref = XE::MakeShared< _RefCountFuture >();

		for( ; begin != end; ++begin )
		{
			_PostTask( ThreadType::WORKS, [begin, task, _ref]()
					  {
						  task( begin );
					  } );
		}

		return _ref->future;
	}

	template< typename Iterator > std::shared_future< void > ParallelTask( Iterator begin, Iterator end, XE::uint64 step, const XE::Delegate< void( Iterator, Iterator ) > & task )
	{
		XE::SharedPtr< _RefCountFuture > _ref = XE::MakeShared< _RefCountFuture >();

		Iterator current = begin;

		while( current != end )
		{
			for( XE::uint64 i = 0; i < step && current != end; ++i, ++current );

			_PostTask( ThreadType::WORKS, [begin, current, task, _ref]()
					  {
						  task( begin, current );
					  } );

			begin = current;
		}

		return _ref->future;
	}

private:
	void _PostTask( ThreadType type, const XE::Delegate< void() > & task );

private:
	Private * _p;
};

END_XE_NAMESPACE

#define ASSERT_THREAD( TYPE ) \
XE_ASSERT( XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >()->GetCurrentThreadType() != TYPE )

#define CHECK_THREAD( TYPE ) \
XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >()->GetCurrentThreadType() == TYPE

#endif // __THREADSERVICE_H__5E718656_36CC_4C4A_9077_F6887B508EC8

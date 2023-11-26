/*!
 * \file	GCMemoryResource.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GCMEMORYRESOURCE_H__E5216A17_0AB8_4D07_8E39_30D9ABF934CA
#define GCMEMORYRESOURCE_H__E5216A17_0AB8_4D07_8E39_30D9ABF934CA

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API GCMemoryResource : public std::pmr::memory_resource
{
private:
	friend class GCObject;
	friend class GCRootObject;
	friend class MemoryResource;
	template< typename T > friend class GCPtr;

private:
	struct Private;

private:
	GCMemoryResource();

	~GCMemoryResource() noexcept override;

private:
	void * do_allocate( size_t _Bytes, size_t _Align ) override;

	void do_deallocate( void * _Ptr, size_t _Bytes, size_t _Align ) override;

	bool do_is_equal( const std::pmr::memory_resource & _That ) const noexcept override;

public:
	static void GC();

public:
	static void Mark( const XE::GCObject * _Ptr );

	static void Barrier( const XE::GCObject * _Ptr );

private:
	static void Register( const XE::GCRootObject * _Root );

	static void Unregister( const XE::GCRootObject * _Root );

private:
	void Recycl();

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//GCMEMORYRESOURCE_H__E5216A17_0AB8_4D07_8E39_30D9ABF934CA

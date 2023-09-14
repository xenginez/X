/*!
 * \file   Object.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/04
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef OBJECT_H__3E588A66_5DD3_46BE_B734_9218C812D828
#define OBJECT_H__3E588A66_5DD3_46BE_B734_9218C812D828

#include "Variant.h"
#include "InvokeStack.h"
#include "MetaTypeList.hpp"

BEG_XE_NAMESPACE

class XE_API Object : public XE::EnableSharedFromThis< Object >
{
	template< typename T > friend struct XE::MetaTypeCollector;

public:
	static XE::MetaClassCPtr GetMetaClassStatic();

	virtual XE::MetaClassCPtr GetMetaClass() const;

public:
	Object();

	virtual ~Object();

public:
	virtual XE::Variant Get( const XE::String & name );

	virtual void Set( const XE::String & name, const XE::Variant & val );

	virtual XE::Variant Invoke( const XE::String & name, XE::InvokeStack * params );

	template< typename ... Types > XE::Variant Invoke( const XE::String & name, Types && ...args )
	{
		XE::InvokeStack stack;

		stack.Push( this, std::forward< Types >( args )... );

		return Invoke( name, &stack );
	}

public:
	virtual void Serialize( XE::OArchive & archive ) const;

	virtual void Deserialize( XE::IArchive & archive );

};

END_XE_NAMESPACE

#endif // OBJECT_H__3E588A66_5DD3_46BE_B734_9218C812D828

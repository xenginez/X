/*!
 * \file	Object.h
 *
 * \author	ZhengYuanQing
 * \date	2019/09/03
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef OBJECT_H__21535455_358C_46D2_9AF9_6A10E5431797
#define OBJECT_H__21535455_358C_46D2_9AF9_6A10E5431797

#include "Utils/Event.h"
#include "Utils/Disposable.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Object : public XE::ReflectObject
{
	OBJECT( Object, ReflectObject )

public:
	using EventFilterCallback = XE::Delegate< void( const XE::Object *, const XE::EventPtr & ) >;

public:
	Object();

	~Object() override;

public:
	virtual void AssetLoad();

public:
	virtual void ProcessEvent( const XE::EventPtr & val );
};

END_XE_NAMESPACE

#endif // OBJECT_H__21535455_358C_46D2_9AF9_6A10E5431797

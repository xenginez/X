/*!
 * \file   Disposable.h
 *
 * \author ZhengYuanQing
 * \date   2021/02/11
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef DISPOSABLE_H__770E7900_32CB_49A3_B8BC_4D4CC99B89C1
#define DISPOSABLE_H__770E7900_32CB_49A3_B8BC_4D4CC99B89C1

#include <functional>

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Disposable
{
public:
    using CallbackType = XE::Delegate<void()>;

public:
    Disposable() = default;

    Disposable( Disposable && val );

    Disposable( const CallbackType & val );

    Disposable( const Disposable & val ) = delete;

    ~Disposable();

public:
    Disposable & operator=( Disposable && val );

    Disposable & operator=( const Disposable & val ) = delete;

public:
	void Detach();

    void Dispose();

private:
    CallbackType _Callback;
};

END_XE_NAMESPACE

#endif // DISPOSABLE_H__770E7900_32CB_49A3_B8BC_4D4CC99B89C1

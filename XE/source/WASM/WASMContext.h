/*!
 * \file	WASMContext.h
 *
 * \author	ZhengYuanQing
 * \date	2022/12/25
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASMCONTEXT_H__4580B9DC_AB9A_4CE3_B37D_62AFE43ED657
#define WASMCONTEXT_H__4580B9DC_AB9A_4CE3_B37D_62AFE43ED657

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASMContext
{
public:
	WASMContext() = default;

	~WASMContext() = default;

public:
	void Push( const XE::WASMValue & val );

	XE::WASMValue & Top();

	XE::WASMValue Pop();

public:
	void Pushed();

	void Poped();

public:
	XE::WASMFrame & GetFrame();

private:
	XE::Array< XE::WASMFrame > _FrameStack;
	XE::Array< XE::WASMValue > _ValueStack;
};

END_XE_NAMESPACE

#endif//WASMCONTEXT_H__4580B9DC_AB9A_4CE3_B37D_62AFE43ED657

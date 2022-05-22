/*!
 * \file	WASMInterpreter.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASMINTERPRETER_H__B03F10EA_D41B_4A2E_9F17_5C0F7C4D036F
#define WASMINTERPRETER_H__B03F10EA_D41B_4A2E_9F17_5C0F7C4D036F

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASMInterpreter : public XE::EnableSharedFromThis< WASMInterpreter >
{
public:
	WASMInterpreter();

	~WASMInterpreter();

public:
	XE::int32 Invoke( const XE::WASMModulePtr & mod, XE::uint32 func );

public:
	void Push( const XE::WASMValue & val );

	XE::WASMValue Top();

	XE::WASMValue Pop();

private:
	void Pushed( const XE::WASMFrame & frame );

	XE::WASMFrame Poped();

private:
	XE::int32 Exec( XE::MemoryView view );

private:
	XE::WASMFrame _CurFrame;
	XE::Array< XE::WASMFrame > _FrameStack;
	XE::Array< XE::WASMValue > _ValueStack;
};

END_XE_NAMESPACE

#endif//WASMINTERPRETER_H__B03F10EA_D41B_4A2E_9F17_5C0F7C4D036F

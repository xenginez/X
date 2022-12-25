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

class XE_API WASMInterpreter
{
public:
	WASMInterpreter( XE::WASMContext * ctx, XE::WASMService * service );

	~WASMInterpreter() = default;

public:
	XE::int32 Exec( XE::MemoryView view );

private:
	XE::WASMContext * _Context;
	XE::WASMService * _Service;
};

END_XE_NAMESPACE

#endif//WASMINTERPRETER_H__B03F10EA_D41B_4A2E_9F17_5C0F7C4D036F

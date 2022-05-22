/*!
* \file   WASMInstance.h
*
* \author ZhengYuanQing
* \date   2022/05/22
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef WASMINSTANCE_H__B5D50373_759A_40BC_9A19_B603CE56DEE4
#define WASMINSTANCE_H__B5D50373_759A_40BC_9A19_B603CE56DEE4

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASMInstance : public XE::EnableSharedFromThis< WASMInstance >
{
public:
	WASMInstance();

	~WASMInstance();

public:

private:
	XE::Array< XE::FuncInstance > Funcs;
	XE::Array< XE::TableInstance > Tables;
	XE::Array< XE::MemoryInstance > Memorys;
	XE::Array< XE::GlobalInstance > Globals;
};

END_XE_NAMESPACE

#endif//WASMINSTANCE_H__B5D50373_759A_40BC_9A19_B603CE56DEE4
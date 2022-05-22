/*!
 * \file	WASMModule.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WASMMODULE_H__26705626_E5FC_4767_82C4_ECA4803F6DBF
#define WASMMODULE_H__26705626_E5FC_4767_82C4_ECA4803F6DBF

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API WASMModule : public XE::EnableSharedFromThis< WASMModule >
{
public:
	WASMModule();

	~WASMModule();

public:
	void Load( XE::MemoryView view );

private:
	void LoadCustomSecions( XE::MemoryView view );

	void LoadTypeSecions( XE::MemoryView view );

	void LoadImportSecions( XE::MemoryView view );

	void LoadFuncSecions( XE::MemoryView view );

	void LoadTableSecions( XE::MemoryView view );

	void LoadMemorySecions( XE::MemoryView view );

	void LoadGlobalSecions( XE::MemoryView view );

	void LoadExportSecions( XE::MemoryView view );

	void LoadStartSecions( XE::MemoryView view );

	void LoadElemSecions( XE::MemoryView view );

	void LoadCodeSecions( XE::MemoryView view );

	void LoadDataSecions( XE::MemoryView view );

private:
	XE::uint32 _Start = 0;
	XE::Array< XE::TypeSection > _Types;
	XE::Array< XE::ImportSection > _Imports;
	XE::Array< XE::FuncSection > _Funcs;
	XE::Array< XE::TableSection > _Tables;
	XE::Array< XE::MemorySection > _Memorys;
	XE::Array< XE::GlobalSection > _Globals;
	XE::Array< XE::ExportSection > _Exports;
	XE::Array< XE::ElemSection > _Elems;
	XE::Array< XE::DataSection > _Datas;

private:
	XE::uint32 _ImportFuncSize = 0;
	XE::uint32 _ImportTableSize = 0;
	XE::uint32 _ImportMemorySize = 0;
	XE::uint32 _ImportGlobalSize = 0;
};

END_XE_NAMESPACE

#endif//WASMMODULE_H__26705626_E5FC_4767_82C4_ECA4803F6DBF

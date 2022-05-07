/*!
 * \file   Module.h
 *
 * \author ZhengYuanQing
 * \date   2022/05/07
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef MODULE_H__231D35BF_2935_441C_81E0_D3083C897627
#define MODULE_H__231D35BF_2935_441C_81E0_D3083C897627

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Module : public std::enable_shared_from_this< Module >
{
public:
	Module();

	~Module();

public:
	void Load( XE::MemoryView view );

	InstancePtr NewInstance() const;

private:
	XE::Array< XE::TypeSection > Types;
	XE::Array< XE::ImportSection > Imports;
	XE::Array< XE::FuncSection > Funcs;
	XE::Array< XE::TableSection > Tables;
	XE::Array< XE::MemorySection > Memorys;
	XE::Array< XE::GlobalSection > Globals;
	XE::Array< XE::ExportSection > Exports;
	XE::uint32 Start;
	XE::Array< XE::ElemSection > Elems;
	XE::Array< XE::DataSection > Datas;
};

END_XE_NAMESPACE

#endif // MODULE_H__231D35BF_2935_441C_81E0_D3083C897627

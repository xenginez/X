/*!
 * \file	MetaEnumerator.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef METAENUMERATOR_H__C5504C28_6B27_4387_9ADE_8B9064DACA3B
#define METAENUMERATOR_H__C5504C28_6B27_4387_9ADE_8B9064DACA3B

#include "Variant.h"
#include "MetaType.h"

BEG_XE_NAMESPACE

class XE_API MetaEnumerator : public XE::EnableSharedFromThis< XE::MetaEnumerator >
{
public:
	virtual ~MetaEnumerator() = default;

public:
	virtual XE::MetaTypeCPtr GetValueType() = 0;

	virtual XE::Variant GetCurrent() = 0;

	virtual void SetCurrent( const XE::Variant & val ) = 0;

public:
	virtual void MoveNext() = 0;

	virtual void Reset() = 0;

	virtual bool IsEnd() = 0;

public:
	virtual XE::uint64 GetSize() = 0;

	virtual void SetIndex( XE::uint64 val ) = 0;

public:
	virtual void EraseCurrent() = 0;

	virtual void InsertToCurrent( const XE::Variant & val ) = 0;

	virtual bool Swap( XE::uint64 first, XE::uint64 second ) = 0;
};

END_XE_NAMESPACE

#endif//METAENUMERATOR_H__C5504C28_6B27_4387_9ADE_8B9064DACA3B

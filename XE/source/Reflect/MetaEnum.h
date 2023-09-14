/*!
 * \file   MetaEnum.h
 *
 * \author ZhengYuanQing
 * \date   2019/1/10
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETAENUM_H__08B9D60C_CE48_4913_BE61_A217F8BFF289
#define __IMETAENUM_H__08B9D60C_CE48_4913_BE61_A217F8BFF289

#include "Variant.h"
#include "MetaType.h"

BEG_XE_NAMESPACE

class XE_API MetaEnum : public XE::MetaType
{
public:
	MetaEnum( const XE::String & Name, XE::uint64 Size, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module );

	~MetaEnum();

public:
	XE::uint64 GetFlags() const;

	XE::uint64 GetEnumCount() const;

public:
	XE::String GetDefaultName() const;

	XE::Variant GetDefaultValue() const;

	String FindName( XE::Variant val ) const;

	XE::Variant FindValue( const String & val ) const;

	const Array< Pair< String, XE::Variant > > & GetValues() const;

public:
	void Visit( const XE::Delegate< void( const XE::String &, const XE::Variant & ) > & val ) const;

protected:
	void _RegisterValue( const XE::String & Name, const XE::Variant & Val );

private:
	XE::Array< XE::Pair< XE::String, XE::Variant > > _Values;
};

END_XE_NAMESPACE

#endif // __IMETAENUM_H__08B9D60C_CE48_4913_BE61_A217F8BFF289

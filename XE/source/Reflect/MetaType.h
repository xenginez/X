/*!
 * \file   MetaType.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/12
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6
#define __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6

#include "MetaInfo.h"

BEG_XE_NAMESPACE

class XE_API MetaType : public XE::MetaInfo
{
public:
	MetaType( const XE::String & Name, XE::MetaInfoType Type, XE::uint64 Size, XE::MetaInfoCPtr Owner, XE::MetaModuleCPtr Module );

	~MetaType();

public:
	XE::uint64 GetSize() const;

public:
	virtual void Serialize( XE::OArchive & arc, const XE::Variant & obj ) const = 0;

	virtual void Deserialize( XE::IArchive & arc, XE::Variant & obj ) const = 0;

private:
	XE::uint64 _Size;
};

END_XE_NAMESPACE

#endif // __IMETATYPE_H__8929D439_3C4B_4410_85AD_5D3F570075E6

/*!
 * \file   CRC32.h
 *
 * \author ZhengYuanQing
 * \date   2019/01/23
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __CRC32_H__77E17FE6_6F43_4C9A_9985_D9E1478BD18B
#define __CRC32_H__77E17FE6_6F43_4C9A_9985_D9E1478BD18B

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API CRC32
{
public:
	static constexpr XE::uint64 BLOCK_SIZE = 16;

public:
	CRC32();

	CRC32( XE::MemoryView val );

	CRC32( const XE::String & val );

public:
	bool operator ==( const CRC32 & val ) const;

	bool operator !=( const CRC32 & val ) const;

public:
	CRC32 & Add( XE::MemoryView val );

	CRC32 & Add( const XE::String & val );

	CRC32 & Finalize();

public:
	void Reset();

	void Reset( XE::MemoryView val );

	void Reset( const XE::String & val );

public:
	XE::uint32 ToUint32() const;

public:
	XE::uint32 _Hash = ~0;
};

END_XE_NAMESPACE

#endif // __CRC32_H__77E17FE6_6F43_4C9A_9985_D9E1478BD18B

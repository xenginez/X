/*!
 * \file	MD5.h
 *
 * \author	ZhengYuanQing
 * \date	2019/12/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MD5_H__EF0EE087_0F95_41D9_956F_532BA1F954FA
#define MD5_H__EF0EE087_0F95_41D9_956F_532BA1F954FA

#include <array>

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API MD5
{
public:
	static constexpr XE::uint64 HASH_SIZE = 16;
	static constexpr XE::uint64 BUFFER_SIZE = 128;

public:
	MD5();

	MD5( XE::MemoryView val );

	MD5( const XE::String & val );

public:
	bool operator ==( const MD5 & val ) const;

	bool operator !=( const MD5 & val ) const;

public:
	MD5 & Add( XE::MemoryView val );

	MD5 & Add( const XE::String & val );

	MD5 & Finalize();

public:
	void Reset();

	void Reset( XE::MemoryView val );

	void Reset( const XE::String & val );

public:
	XE::MemoryView ToView() const;

	XE::String ToString( bool up = true ) const;

	std::array<XE::uint8, HASH_SIZE> ToArray() const;

public:
	XE::uint64 _Index = 0;
	XE::uint64 _Count = 0;
	std::array< XE::uint32, 4 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

XE_INLINE XE::String ToString(const XE::MD5 & _Val )
{
	return _Val.ToString();
}

END_XE_NAMESPACE

#endif // MD5_H__EF0EE087_0F95_41D9_956F_532BA1F954FA

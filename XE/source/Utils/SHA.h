/*!
 * \file   SHA.h
 *
 * \author ZhengYuanQing
 * \date   2022/02/07
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef SHA_H__CDAA54BE_793D_415D_ABD8_D31F5770791F
#define SHA_H__CDAA54BE_793D_415D_ABD8_D31F5770791F

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API SHA1
{
public:
	static constexpr XE::uint64 HASH_SIZE = 20;
	static constexpr XE::uint64 BUFFER_SIZE = 128;

public:
	SHA1();

	SHA1( XE::MemoryView val );

	SHA1( const XE::String & val );

public:
	bool operator ==( const SHA1 & val ) const;

	bool operator !=( const SHA1 & val ) const;

public:
	SHA1 & Add( XE::MemoryView val );

	SHA1 & Add( const XE::String & val );

	SHA1 & Finalize();

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
	std::array< XE::uint32, 5 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA2_224
{
public:
	static constexpr XE::uint64 HASH_SIZE = 28;
	static constexpr XE::uint64 BUFFER_SIZE = 128;

public:
	SHA2_224();

	SHA2_224( XE::MemoryView val );

	SHA2_224( const XE::String & val );

public:
	bool operator ==( const SHA2_224 & val ) const;

	bool operator !=( const SHA2_224 & val ) const;

public:
	SHA2_224 & Add( XE::MemoryView val );

	SHA2_224 & Add( const XE::String & val );

	SHA2_224 & Finalize();

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
	std::array< XE::uint32, 8 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA2_256
{
public:
	static constexpr XE::uint64 HASH_SIZE = 32;
	static constexpr XE::uint64 BUFFER_SIZE = 128;

public:
	SHA2_256();

	SHA2_256( XE::MemoryView val );

	SHA2_256( const XE::String & val );

public:
	bool operator ==( const SHA2_256 & val ) const;

	bool operator !=( const SHA2_256 & val ) const;

public:
	SHA2_256 & Add( XE::MemoryView val );

	SHA2_256 & Add( const XE::String & val );

	SHA2_256 & Finalize();

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
	std::array< XE::uint32, 8 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA2_384
{
public:
	static constexpr XE::uint64 HASH_SIZE = 48;
	static constexpr XE::uint64 BUFFER_SIZE = 256;

public:
	SHA2_384();

	SHA2_384( XE::MemoryView val );

	SHA2_384( const XE::String & val );

public:
	bool operator ==( const SHA2_384 & val ) const;

	bool operator !=( const SHA2_384 & val ) const;

public:
	SHA2_384 & Add( XE::MemoryView val );

	SHA2_384 & Add( const XE::String & val );

	SHA2_384 & Finalize();

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
	std::array< XE::uint64, 8 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA2_512
{
public:
	static constexpr XE::uint64 HASH_SIZE = 64;
	static constexpr XE::uint64 BUFFER_SIZE = 256;

public:
	SHA2_512();

	SHA2_512( XE::MemoryView val );

	SHA2_512( const XE::String & val );

public:
	bool operator ==( const SHA2_512 & val ) const;

	bool operator !=( const SHA2_512 & val ) const;

public:
	SHA2_512 & Add( XE::MemoryView val );

	SHA2_512 & Add( const XE::String & val );

	SHA2_512 & Finalize();

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
	std::array< XE::uint64, 8 > _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA3_224
{
public:
	static constexpr XE::uint8 FLAG = 0x06;
	static constexpr XE::uint64 HASH_SIZE = 28;
	static constexpr XE::uint64 BUFFER_SIZE = 144;

public:
	SHA3_224();

	SHA3_224( XE::MemoryView val );

	SHA3_224( const XE::String & val );

public:
	bool operator ==( const SHA3_224 & val ) const;

	bool operator !=( const SHA3_224 & val ) const;

public:
	SHA3_224 & Add( XE::MemoryView val );

	SHA3_224 & Add( const XE::String & val );

	SHA3_224 & Finalize();

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
	std::array<XE::uint64, 5 * 5> _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA3_256
{
public:
	static constexpr XE::uint8 FLAG = 0x06;
	static constexpr XE::uint64 HASH_SIZE = 32;
	static constexpr XE::uint64 BUFFER_SIZE = 136;

public:
	SHA3_256();

	SHA3_256( XE::MemoryView val );

	SHA3_256( const XE::String & val );

public:
	bool operator ==( const SHA3_256 & val ) const;

	bool operator !=( const SHA3_256 & val ) const;

public:
	SHA3_256 & Add( XE::MemoryView val );

	SHA3_256 & Add( const XE::String & val );

	SHA3_256 & Finalize();

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
	std::array<XE::uint64, 5 * 5> _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA3_384
{
public:
	static constexpr XE::uint8 FLAG = 0x06;
	static constexpr XE::uint64 HASH_SIZE = 48;
	static constexpr XE::uint64 BUFFER_SIZE = 104;

public:
	SHA3_384();

	SHA3_384( XE::MemoryView val );

	SHA3_384( const XE::String & val );

public:
	bool operator ==( const SHA3_384 & val ) const;

	bool operator !=( const SHA3_384 & val ) const;

public:
	SHA3_384 & Add( XE::MemoryView val );

	SHA3_384 & Add( const XE::String & val );

	SHA3_384 & Finalize();

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
	std::array<XE::uint64, 5 * 5> _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};

class XE_API SHA3_512
{
public:
	static constexpr XE::uint8 FLAG = 0x06;
	static constexpr XE::uint64 HASH_SIZE = 64;
	static constexpr XE::uint64 BUFFER_SIZE = 72;

public:
	SHA3_512();

	SHA3_512( XE::MemoryView val );

	SHA3_512( const XE::String & val );

public:
	bool operator ==( const SHA3_512 & val ) const;

	bool operator !=( const SHA3_512 & val ) const;

public:
	SHA3_512 & Add( XE::MemoryView val );

	SHA3_512 & Add( const XE::String & val );

	SHA3_512 & Finalize();

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
	std::array<XE::uint64, 5 * 5> _Hash;
	std::array< XE::uint8, BUFFER_SIZE > _Buffer;
};


XE_INLINE XE::String ToString( const XE::SHA1 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA2_224 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA2_256 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA2_384 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA2_512 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA3_224 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA3_256 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA3_384 & _Val )
{
	return _Val.ToString();
}

XE_INLINE XE::String ToString( const XE::SHA3_512 & _Val )
{
	return _Val.ToString();
}

END_XE_NAMESPACE

#endif // SHA_H__CDAA54BE_793D_415D_ABD8_D31F5770791F

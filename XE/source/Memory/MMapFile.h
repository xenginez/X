/*!
 * \file	MMapFile.h
 *
 * \author	ZhengYuanQing
 * \date	2021/04/08
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef MMAPFILE_H__886D355F_2A43_4F62_B820_BC9B8CE19917
#define MMAPFILE_H__886D355F_2A43_4F62_B820_BC9B8CE19917

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API MMapFile : public XE::NonCopyable
{
private:
	struct Private;

public:
	MMapFile();

	MMapFile( XE::uint64 size );

	MMapFile( MMapFile && val );

	MMapFile( const std::filesystem::path & path );

	~MMapFile();

public:
	XE::uint8 & operator[]( XE::uint64 val );

	XE::uint8 operator[]( XE::uint64 val ) const;

public:
	void Swap( MMapFile & val );

public:
	bool IsOpen() const;

	XE::uint64 GetSize() const;

	XE::uint8 * GetAddress() const;

	const std::filesystem::path & GetFilePath() const;

public:
	void Open( XE::uint64 size );

	void Open( const std::filesystem::path & path );

	void Close();

	void Reset();

public:
	void Write( XE::uint64 index, const XE::uint8 * data, XE::uint64 size );

	XE::uint64 Read( XE::uint64 index, XE::uint8 * data, XE::uint64 size ) const;

private:
	Private * _p;
};

END_XE_NAMESPACE

namespace std
{
	XE_INLINE void swap( XE::MMapFile & _Left, XE::MMapFile & _Right ) noexcept
	{
		_Left.Swap( _Right );
	}
}

#endif // MMAPFILE_H__886D355F_2A43_4F62_B820_BC9B8CE19917

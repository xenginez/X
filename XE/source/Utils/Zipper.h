/*!
 * \file	Zipper.h
 *
 * \author	ZhengYuanQing
 * \date	2020/06/19
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ZIPPER_H__AEBA8259_8E43_4DEF_B112_6C8ABB412AFC
#define ZIPPER_H__AEBA8259_8E43_4DEF_B112_6C8ABB412AFC

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Zipper
{
private:
    struct Private;

public:
	Zipper();

	Zipper( const std::filesystem::path & path );

    ~Zipper();

public:
	bool ExtractEntry( const XE::String & name ) const;

	void GetEntryNames( XE::Array< XE::String > & names ) const;

	void GetEntryNamesFromType( const XE::String & type, XE::Array< XE::String > & names ) const;

public:
	bool AddEntry( const XE::String & name, const XE::String & type, std::istream & stream, XE::ZipType zip = XE::ZipType::LZ4 );

	bool GetEntry( const XE::String & name, std::ostream & stream );

	bool GetEntryStream( const XE::String & name, std::ostream & stream, XE::uint64 offset, XE::uint64 size );

public:
    bool Open( const std::filesystem::path & path, const XE::String & password = "" );

	void Close();

	bool IsOpen() const;

private:
    Private * _p;
};

END_XE_NAMESPACE

#endif // ZIPPER_H__AEBA8259_8E43_4DEF_B112_6C8ABB412AFC

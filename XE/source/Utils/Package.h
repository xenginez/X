/*!
 * \file	Package.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PACKAGE_H__968340C1_1221_47C0_9FC0_D6A1FB32EE5B
#define PACKAGE_H__968340C1_1221_47C0_9FC0_D6A1FB32EE5B

#include "UUID.h"

BEG_XE_NAMESPACE

class XE_API Package : public XE::NonCopyable
{
private:
	struct Private;

public:
	Package();

	~Package();

public:
	XE::uint64 GetVersion() const;

	void SetVersion( XE::uint64 val );

public:
	bool ExistsEntry( const XE::UUID & val ) const;

	bool ExistsEntry( const XE::String & val ) const;

public:
	void GetEntryUUIDs( XE::Array< XE::UUID > & val ) const;

	void GetEntryNames( XE::Array< XE::String > & val ) const;

public:
	bool GetEntry( const XE::UUID & uuid, std::ostream & stream ) const;

	bool GetEntry( const XE::String & name, std::ostream & stream ) const;

public:
	bool Open( const std::filesystem::path & path, const XE::String & password = "" );

	void Close();

	bool IsOpen() const;

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//PACKAGE_H__968340C1_1221_47C0_9FC0_D6A1FB32EE5B

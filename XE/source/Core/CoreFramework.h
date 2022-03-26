/*!
 * \file   CoreFramework.h
 *
 * \author ZhengYuanQing
 * \date   2019/02/26
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __COREFRAMEWORK_H__3160BD89_FCC4_4C30_B3D4_D540CB0A51E0
#define __COREFRAMEWORK_H__3160BD89_FCC4_4C30_B3D4_D540CB0A51E0

#include "Utils/Window.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API CoreFramework : public XE::NonCopyable
{
	OBJECT( CoreFramework )

private:
	struct Private;

public:
	CoreFramework();

	~CoreFramework();

public:
	static CoreFrameworkPtr GetCurrentFramework();

public:
	virtual XE::ServicePtr GetService( const XE::MetaClassPtr & val ) const;

	template< typename T > XE::SharedPtr< T > GetServiceT() const
	{
		return SP_CAST< T >( GetService( ClassID< T >::Get() ) );
	}

public:
	virtual bool RegisterService( const XE::MetaClassPtr & val );

	virtual void UnregisterService( const XE::MetaClassPtr & val );

public:
	virtual void Exit();

	virtual void WaitExit();

	virtual bool IsExit() const;

public:
	virtual XE::Language GetSystemLanguage() const;

	virtual std::filesystem::path GetModulePath() const;

	virtual std::filesystem::path GetAssetsPath() const;

	virtual std::filesystem::path GetCachesPath() const;

	virtual std::filesystem::path GetUserDataPath() const;

	virtual std::filesystem::path GetApplicationPath() const;

public:
	virtual XE::WindowPtr GetMainWindow() const;

public:
	virtual void Save();

	virtual void Reload();

public:
	int Exec( XE::WindowPtr window );

protected:
	virtual void Prepare();

	virtual void Startup();

	virtual void Update();

	virtual void Clearup();

protected:
	void LoadModules();

	void LoadServices();

protected:
	void Save( const std::filesystem::path & path, const Map < XE::String, XE::String > & values ) const;

	void Reload( const std::filesystem::path & path, Map < XE::String, XE::String > & values ) const;

public:
	void SetBool( const XE::String & key, bool val );

	bool GetBool( const XE::String & key, bool def = false );

	void SetInt8( const XE::String & key, XE::int8 val );

	XE::int8 GetInt8( const XE::String & key, XE::int8 def = 0 );

	void SetInt16( const XE::String & key, XE::int16 val );

	XE::int16 GetInt16( const XE::String & key, XE::int16 def = 0 );

	void SetInt32( const XE::String & key, XE::int32 val );

	XE::int32 GetInt32( const XE::String & key, XE::int32 def = 0 );

	void SetInt64( const XE::String & key, XE::int64 val );

	XE::int64 GetInt64( const XE::String & key, XE::int64 def = 0 );

	void  SetUInt8( const XE::String & key, XE::uint8 val );

	XE::uint8  GetUInt8( const XE::String & key, XE::uint8 def = 0 );

	void SetUInt16( const XE::String & key, XE::uint16 val );

	XE::uint16 GetUInt16( const XE::String & key, XE::uint16 def = 0 );

	void SetUInt32( const XE::String & key, XE::uint32 val );

	XE::uint32 GetUInt32( const XE::String & key, XE::uint32 def = 0 );

	void SetUInt64( const XE::String & key, XE::uint64 val );

	XE::uint64 GetUInt64( const XE::String & key, XE::uint64 def = 0 );

	void SetFloat32( const XE::String & key, XE::float32 val );

	XE::float32 GetFloat32( const XE::String & key, XE::float32 def = 0.0f );

	void SetFloat64( const XE::String & key, XE::float64 val );

	XE::float64 GetFloat64( const XE::String & key, XE::float64 def = 0.0 );

	void SetString( const XE::String & key, const XE::String & val );

	XE::String GetString( const XE::String & key, const XE::String & def = "" );

	void SetStringArray( const XE::String & key, const XE::Array< XE::String > & val );

	XE::Array< XE::String > GetStringArray( const XE::String & key, const XE::Array< XE::String > & def = XE::Array< XE::String >() );

protected:
	virtual XE::String GetValue( const XE::String & key );

	virtual void SetValue( const XE::String & key, const XE::String & val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __COREFRAMEWORK_H__3160BD89_FCC4_4C30_B3D4_D540CB0A51E0

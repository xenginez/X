/*!
 * \file	CoreFramework.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/30
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef COREFRAMEWORK_H__941035B0_93C2_49A9_BEB1_FC3442BCA3F4
#define COREFRAMEWORK_H__941035B0_93C2_49A9_BEB1_FC3442BCA3F4

#include "Global.h"
#include "AssetDatabase.h"

BEG_XS_NAMESPACE

class XS_API CoreFramework : public XE::CoreFramework
{
	OBJECT( CoreFramework, XE::CoreFramework )

private:
	struct Private;

public:
	CoreFramework();

	~CoreFramework() override;

	static XS::CoreFramework * GetCurrentFramework();

public:
	bool RegisterService( const XE::MetaClassPtr & val ) override;

	void UnregisterService( const XE::MetaClassPtr & val ) override;

public:
	void Exit() override;

	void WaitExit() override;

	bool IsExit() const override;

public:
	XE::Language GetSystemLanguage() const override;

	std::filesystem::path GetModulePath() const override;

	std::filesystem::path GetAssetsPath() const override;

	std::filesystem::path GetCachesPath() const override;

	std::filesystem::path GetProjectPath() const;

	std::filesystem::path GetUserDataPath() const override;

	std::filesystem::path GetApplicationPath() const override;

public:
	XE::WindowPtr GetMainWindow() const override;

	XS::AssetDatabase * GetAssetDatabase() const;

public:
	void Save() override;

	void Reload() override;

public:
	int Exec( XE::WindowPtr window ) override;

	void Exec( XE::WindowPtr window, const XE::String & project_path );

protected:
	void Prepare() override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

protected:
	XE::String GetValue( const XE::String & key ) override;

	void SetValue( const XE::String & key, const XE::String & val ) override;

private:
	Private * _p;
};
DECL_PTR( CoreFramework );

END_XS_NAMESPACE

#endif//COREFRAMEWORK_H__941035B0_93C2_49A9_BEB1_FC3442BCA3F4

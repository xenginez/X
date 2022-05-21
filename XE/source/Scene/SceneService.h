/*!
 * \file	SceneService.h
 *
 * \author	ZhengYuanQing
 * \date	2022/05/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef SCENESERVICE_H__BC7E3E13_EE62_4C4F_92B5_B3A2275E717D
#define SCENESERVICE_H__BC7E3E13_EE62_4C4F_92B5_B3A2275E717D

#include "Type.h"

#include "Core/Service.h"

BEG_XE_NAMESPACE

class XE_API SceneService : public XE::Service
{
	OBJECT( SceneService, XE::Service )

private:
	struct Private;

public:
	SceneService();

	~SceneService() override;

public:
	void Prepare()override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	void LoadWorld( const XE::String & val );

	void UnloadWorld( const XE::String & val );

	void AsyncLoadWorld( const XE::String & val );

public:
	bool GetWorldEnable( const XE::String & val ) const;

	void SetWorldEnable( const XE::String & name, bool enable );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//SCENESERVICE_H__BC7E3E13_EE62_4C4F_92B5_B3A2275E717D

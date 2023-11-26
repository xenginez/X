/*!
 * \file	GUIService.h
 *
 * \author	ZhengYuanQing
 * \date	2022/06/11
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GUISERVICE_H__59B6C5BA_686C_4552_889E_194404FDDE5A
#define GUISERVICE_H__59B6C5BA_686C_4552_889E_194404FDDE5A

#include "Core/Service.h"

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API GUIService : public XE::Service
{
	OBJECT( GUIService, XE::Service )

private:
	struct Private;

public:
	GUIService();

	~GUIService() override;

public:
	void Prepare()override;

	void Startup() override;

	void Update() override;

	void Clearup() override;

public:
	XE::Vec2i GetScreenSize() const;

public:
	XE::CanvasPtr FindCanvas( const XE::String & val );

	XE::Disposable RegisterCanvas( const XE::CanvasPtr & val );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif//GUISERVICE_H__59B6C5BA_686C_4552_889E_194404FDDE5A

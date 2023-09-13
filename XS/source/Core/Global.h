/*!
 * \file	Global.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GLOBAL_H__3384BD3C_6A3A_46A7_829C_38D1EB4DD8FE
#define GLOBAL_H__3384BD3C_6A3A_46A7_829C_38D1EB4DD8FE

#include <XE/XE.h>

#include <QDir>
#include <QIcon>
#include <QDebug>
#include <QWidget>
#include <QDialog>
#include <QMetaType>
#include <QShortcut>
#include <QApplication>

#ifdef XS_EXPORT
#define XS_API Q_DECL_EXPORT
#else
#define XS_API Q_DECL_IMPORT
#endif // XS_EXPORT

#define BEG_XS_NAMESPACE namespace XS{
#define END_XS_NAMESPACE }

#define UI_DECL( TYPE ) namespace Ui { class TYPE; }

BEG_XS_NAMESPACE

static constexpr const char * FOLLOW_DIRECTORY = "Follow";
static constexpr const char * EDITORS_DIRECTORY = "Editors";
static constexpr const char * PACKAGES_DIRECTORY = "Packages";
static constexpr const char * PROJECT_FILE_NAME = "Project.json";
static constexpr const char * ASSET_DATABASE_URL = "Project/AssetDatabaseUrl";

using InspectorEventInfo = std::pair<XE::String, XE::Variant>;

DECL_EVENT( INSPECTOR, "inspector editor view", XS::InspectorEventInfo );

END_XS_NAMESPACE

#endif//GLOBAL_H__3384BD3C_6A3A_46A7_829C_38D1EB4DD8FE

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

#include <QWidget>
#include <QDialog>
#include <QMetaType>

#ifdef XS_EXPORT
#define XS_API Q_DECL_EXPORT
#else
#define XS_API Q_DECL_IMPORT
#endif // XS_EXPORT

#define BEG_XS_NAMESPACE namespace XS{
#define END_XS_NAMESPACE }

#define UI_DECL( TYPE ) namespace Ui { class TYPE; }

#endif//GLOBAL_H__3384BD3C_6A3A_46A7_829C_38D1EB4DD8FE

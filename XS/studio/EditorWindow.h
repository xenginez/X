/*!
 * \file	EditorWindow.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef EDITORWINDOW_H__1616AEDE_9EAD_4888_8E9F_06BCB0CE8AB9
#define EDITORWINDOW_H__1616AEDE_9EAD_4888_8E9F_06BCB0CE8AB9

#include "Global.h"

namespace XStudio
{
	class EditorWindow : public XS::FramelessWindow
	{
		Q_OBJECT

	public:
		EditorWindow( const QString & project, QWidget * parent = nullptr );

		~EditorWindow();

	protected:
		void closeEvent( QCloseEvent * e ) override;
	};
}

#endif//EDITORWINDOW_H__1616AEDE_9EAD_4888_8E9F_06BCB0CE8AB9

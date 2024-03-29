/*!
 * \file	InspectorEditor.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/13
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef INSPECTOREDITOR_H__FAF8AC9B_77BB_4F16_867A_56CF43E88042
#define INSPECTOREDITOR_H__FAF8AC9B_77BB_4F16_867A_56CF43E88042

#include "Widgets/InspectorWidget.h"
#include "Widgets/ToolEditorWindow.h"

UI_DECL( InspectorEditor );

BEG_XS_NAMESPACE

class XS_API InspectorEditor : public XS::ToolEditorWindow
{
	Q_OBJECT

public:
	InspectorEditor( QWidget * parent = nullptr );

	~InspectorEditor();

public:
	Q_INVOKABLE static QIcon icon();

	Q_INVOKABLE static QString name();

private slots:
	void OnInspector( const QString & name, const XE::Variant & val );

private:
	Ui::InspectorEditor * ui;
	XS::InspectorWidget * _Inspector = nullptr;
	XE::Disposable _InspectorEvent;
};

END_XS_NAMESPACE

#endif//INSPECTOREDITOR_H__FAF8AC9B_77BB_4F16_867A_56CF43E88042
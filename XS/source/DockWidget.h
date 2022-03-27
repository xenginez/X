/*!
 * \file	DockWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef DOCKWIDGET_H__FEBF931E_25BB_4A58_ABCB_285B779A01F9
#define DOCKWIDGET_H__FEBF931E_25BB_4A58_ABCB_285B779A01F9

#include <QLabel>
#include <QSettings>
#include <QUndoStack>
#include <QDockWidget>
#include <QUndoCommand>

#include "Registry.h"

class QToolButton;

BEG_XS_NAMESPACE

class UndoCommand : public QUndoCommand
{
public:
	UndoCommand( const QString & text, const std::function<void()> & undo, const std::function<void()> & redo )
		:QUndoCommand( text ), _Undo( undo ), _Redo( redo )
	{}

	void undo() override
	{
		_Undo();
	}

	void redo() override
	{
		_Redo();
	}

private:
	std::function<void()> _Undo;
	std::function<void()> _Redo;
};

class XS_API DockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockWidget( QWidget * parent = nullptr );

	~DockWidget();

public:
	virtual void Save( QSettings & settings );

	virtual void Load( QSettings & settings );

public:
	void PushUndoCommand( QUndoCommand * command );

	template< typename REDO, typename UNDO > void PushUndoCommand( const QString & text, UNDO && undo, REDO && redo )
	{
		PushUndoCommand( new UndoCommand( text, undo, redo ) );
	}

protected:
	void setTitleBar( QWidget * title_bar );

	template< typename T > void setupUi( T * ui )
	{
		QWidget * widget = new QWidget( this );

		ui->setupUi( widget );

		QDockWidget::setWidget( widget );
		QDockWidget::setObjectName( widget->objectName() );
		QDockWidget::setWindowTitle( widget->windowTitle() );
	}

protected:
	bool eventFilter( QObject * watched, QEvent * event ) override;

private slots:
	void OnFixedClicked( bool checked );

	void OnCloseClicked( bool checked );

	void OnFloatClicked( bool checked );

	void OnTopLevelChanged( bool topLevel );

private:
	QLabel * _IconLabel = nullptr;
	QUndoStack * _UndoStack = nullptr;
	QToolButton * _FloatButton = nullptr;
	QToolButton * _FixedButton = nullptr;
};

END_XS_NAMESPACE

#endif//DOCKWIDGET_H__FEBF931E_25BB_4A58_ABCB_285B779A01F9

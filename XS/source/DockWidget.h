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
	UndoCommand( const QString & text, const std::function<void()> & redo, const std::function<void()> & undo )
		:QUndoCommand( text ), _Redo( redo ), _Undo( undo )
	{}

	void redo() override
	{
		_Redo();
	}

	void undo() override
	{
		_Undo();
	}

private:
	std::function<void()> _Redo;
	std::function<void()> _Undo;
};

class XS_API DockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockWidget( QWidget * parent = nullptr );

	~DockWidget();

public:
	virtual void SaveLayout( QSettings & settings );

	virtual void LoadLayout( QSettings & settings );

public:
	template< typename T > T * GetParent() const
	{
		QWidget * parent = parentWidget();
		while ( parent != nullptr )
		{
			if ( parent->metaObject()->inherits( &T::staticMetaObject ) )
			{
				return dynamic_cast<T *>( parent );
			}
			else
			{
				parent = parent->parentWidget();
			}
		}
		return nullptr;
	}

	void PushUndoCommand( QUndoCommand * command );

	template< typename REDO, typename UNDO > void PushUndoCommand( const QString & text, REDO && redo, UNDO && undo )
	{
		PushUndoCommand( new UndoCommand( text, std::forward<REDO>( redo ), std::forward<UNDO>( undo ) ) );
	}

	QShortcut * AddShortcuts( const QString & name, const QKeySequence & key );

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

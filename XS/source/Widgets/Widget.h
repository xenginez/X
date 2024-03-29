/*!
 * \file	Widget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef WIDGET_H__C939D859_7363_4F66_A583_460EB05AAC48
#define WIDGET_H__C939D859_7363_4F66_A583_460EB05AAC48

#include "DockWidget.h"

BEG_XS_NAMESPACE

class XS_API Widget : public QWidget
{
	Q_OBJECT

private:
	friend class DockWidget;

public:
	Widget( QWidget * parent = nullptr );

	~Widget() override;

public:
	static XS::CoreFramework * GetFramework();

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
		PushUndoCommand( new XS::UndoCommand( text, redo, undo ) );
	}

public:
	QShortcut * AddShortcuts( const QString & name, const QKeySequence & key );

public:
	virtual void SaveLayout( QSettings & settings );

	virtual void LoadLayout( QSettings & settings );

public:
	void OnCommandRedo();

	void OnCommandUndo();

	void OnCommandSave();

protected:
	virtual void OnRedo();

	virtual void OnUndo();

	virtual void OnSave();

private:
	XS::Widget * _Parent = nullptr;
	QList< XS::Widget * > _Children;
};

END_XS_NAMESPACE

#endif//WIDGET_H__C939D859_7363_4F66_A583_460EB05AAC48

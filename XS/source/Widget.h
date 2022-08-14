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

	template< typename REDO, typename UNDO > void PushUndoCommand( const QString & text, REDO && redo, UNDO && undo )
	{
		QWidget * parent = parentWidget();
		while ( parent != nullptr )
		{
			if ( parent->metaObject()->inherits( &XS::DockWidget::staticMetaObject ) )
			{
				dynamic_cast<XS::DockWidget *>( parent )->PushUndoCommand( text, redo, undo );
				break;
			}
			else
			{
				parent = parent->parentWidget();
			}
		}
	}

public:
	QShortcut * AddShortcuts( const QString & name, const QKeySequence & key );

public:
	virtual void SaveLayout( QSettings & settings );

	virtual void LoadLayout( QSettings & settings );

};

END_XS_NAMESPACE

#endif//WIDGET_H__C939D859_7363_4F66_A583_460EB05AAC48

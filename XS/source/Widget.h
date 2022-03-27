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

#include "Registry.h"

BEG_XS_NAMESPACE

class XS_API Widget : public QWidget
{
	Q_OBJECT

public:
	Widget( QWidget * parent = nullptr );

	~Widget() override;

public:
	template< typename REDO, typename UNDO > void PushUndoCommand( const QString & text, UNDO && undo, REDO && redo )
	{
		QString dock_name( "XS::DockWidget" );
		QWidget * parent = parentWidget();
		while ( parent != nullptr )
		{
			if ( parent->metaObject()->className() == dock_name )
			{
				dynamic_cast<XS::DockWidget *>( parent )->PushUndoCommand( text, undo, redo );
				break;
			}
			else
			{
				parent = parent->parentWidget();
			}
		}
	}
};

END_XS_NAMESPACE

#endif//WIDGET_H__C939D859_7363_4F66_A583_460EB05AAC48

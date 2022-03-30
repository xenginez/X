#include "ClassInspector.h"

#include <QHeaderView>

REG_WIDGET( XS::ClassInspector );

Q_DECLARE_METATYPE( XS::ObjectProxyPtr );

XS::ClassInspector::ClassInspector( QWidget * parent /* = nullptr */ )
	: Inspector( parent )
{
	_QTreeWidget = new QTreeWidget( this );
	SetContentWidget( _QTreeWidget );
	_QTreeWidget->setHeaderLabels( { tr( "name" ), tr( "value" ) } );
	_QTreeWidget->setEditTriggers( QAbstractItemView::EditTrigger::NoEditTriggers );
	_QTreeWidget->setSelectionMode( QAbstractItemView::SelectionMode::NoSelection );
}

XS::ClassInspector::~ClassInspector()
{

}

void XS::ClassInspector::Refresh()
{
	_QTreeWidget->clear();

	if ( auto cls = SP_CAST< const XE::MetaClass >( GetObjectProxy()->GetType() ) )
	{
		cls->VisitProperty( [this]( const XE::MetaPropertyCPtr & prop )
			{
				auto proxy = XE::MakeShared< XS::ObjectProxy >( GetObjectProxy()->GetValue(), prop );

				if ( proxy->FindAttributeT< XE::NonEditorAttribute >() )
				{
					return;
				}

				QTreeWidgetItem * item = new QTreeWidgetItem();
				{
					item->setText( 0, tr( prop->GetName().c_str() ) );
					item->setData( 0, Qt::UserRole + 1, QVariant::fromValue( proxy ) );
					item->setToolTip( 0, tr( prop->GetValueType()->GetFullName().c_str() ) );
				}
				_QTreeWidget->addTopLevelItem( item );

				auto inspector = Inspector::Create( proxy, this );

				if ( inspector->metaObject()->className() == metaObject()->className() )
				{
					QTreeWidget * child = dynamic_cast<XS::ClassInspector *>( inspector )->_QTreeWidget;
					for ( size_t i = 0; i < child->topLevelItemCount(); i++ )
					{
						Merge( _QTreeWidget, item, child, child->topLevelItem( i ) );

						child->deleteLater();
					}
				}
				else
				{
					_QTreeWidget->setItemWidget( item, 1, inspector );
				}
			} );
	}
}

void XS::ClassInspector::Merge( QTreeWidget * parent, QTreeWidgetItem * parent_item, QTreeWidget * child, QTreeWidgetItem * child_item )
{
	QTreeWidgetItem * item = new QTreeWidgetItem( QStringList() << child_item->text( 0 ) );
	{
		item->setText( 0, child_item->text( 0 ) );
		item->setToolTip( 0, child_item->toolTip( 0 ) );
		item->setData( 0, Qt::UserRole + 1, child_item->data( 0, Qt::UserRole + 1 ) );
	}
	parent_item->addChild( item );

	QWidget * widget = child->itemWidget( child_item, 1 );
	{
		widget->setParent( this );
	}
	parent->setItemWidget( item, 1, widget );

	for ( size_t i = 0; i < child_item->childCount(); i++ )
	{
		Merge( parent, item, child, child_item->child( i ) );
	}
}

void XS::ClassInspector::Expand()
{
	_QTreeWidget->expandAll();
}

void XS::ClassInspector::Collapse()
{
	_QTreeWidget->collapseAll();
}

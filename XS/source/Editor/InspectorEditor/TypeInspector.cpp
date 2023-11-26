#include "TypeInspector.h"

#include <QHBoxLayout>

#include "Widgets/TypeSelectorDialog.h"

REG_INSPECTOR( XE::MetaID, XS::TypeInspector );

XS::TypeInspector::TypeInspector( QWidget * parent )
	:InspectorWidget( parent )
{
	QWidget * widget = new QWidget( this );
	{
		QHBoxLayout * layout = new QHBoxLayout( widget );
		{
			layout->setSpacing( 0 );
			layout->setObjectName( QString::fromUtf8( "layout" ) );
			layout->setContentsMargins( 0, 0, 0, 0 );
		}

		_Edit = new QLineEdit( this );
		{
			_Edit->setReadOnly( true );
		}
		layout->addWidget( _Edit );

		_Button = new QToolButton( this );
		{
			_Button->setFixedWidth( 15 );
			_Button->setText( "" );
			_Button->setIcon( QIcon( "" ) );

			connect( _Button, &QToolButton::clicked, this, &TypeInspector::OnToolButtonClicked );
		}
		layout->addWidget( _Button );
	}
	SetContentWidget( widget );
}

XS::TypeInspector::~TypeInspector()
{

}

void XS::TypeInspector::Refresh()
{
	auto id = GetObjectProxy()->GetValue().Value<XE::MetaID>();

	_Edit->setProperty( "MetaID", id.GetHashCode() );
	_Edit->setText( QString::fromUtf8( XE::Reflection::FindType( id.GetHashCode() )->GetFullName().c_str() ) );
}

void XS::TypeInspector::OnToolButtonClicked()
{
	TypeSelectorDialog dialog( XS::TypeSelectorDialog::ALL, this );

	if ( dialog.exec() == QDialog::Accepted )
	{
		XE::MetaID old_id = _Edit->property( "MetaID" ).toULongLong();
		XE::MetaID new_id = dialog.GetSelectTypeID();

		PushUndoCommand
		( tr( "TypeInspector" ),
		  [this, new_id]()
		{
			_Edit->setProperty( "MetaID", new_id.GetHashCode() );

			GetObjectProxy()->SetValue( new_id );
		},
		  [this, old_id]()
		{
			_Edit->setProperty( "MetaID", old_id.GetHashCode() );

			GetObjectProxy()->SetValue( old_id );
		} );
	}
}

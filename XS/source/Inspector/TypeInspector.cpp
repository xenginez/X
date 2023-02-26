#include "TypeInspector.h"

#include <QHBoxLayout>

#include "Core/TypeSelectorDialog.h"

REG_INSPECTOR( XE::ASTInfoType, XS::TypeInspector );

XS::TypeInspector::TypeInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
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
	_Edit->setText( QString::fromUtf8( GetObjectProxy()->GetValue().Value<XE::ASTInfoTypePtr>()->Name.c_str() ) );
}

void XS::TypeInspector::OnToolButtonClicked()
{
	TypeSelectorDialog dialog( XS::TypeSelectorDialog::ALL, this );

	if ( dialog.exec() == QDialog::Accepted )
	{
		auto old_type = _Edit->text();
		auto new_type = dialog.GetSelectType();
		PushUndoCommand
		( tr( "TypeInspector" ),
		  [this, new_type]()
		{
			GetObjectProxy()->GetValue().Value<XE::ASTInfoTypePtr>()->Name = new_type->GetFullName();
		},
		  [this, old_type]()
		{
			GetObjectProxy()->GetValue().Value<XE::ASTInfoTypePtr>()->Name = old_type.toStdString();
		} );
	}
}

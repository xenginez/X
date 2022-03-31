#include "StringInspector.h"

#include <QDialog>
#include <QToolButton>
#include <QHBoxLayout>

#include "ui_TextDialog.h"

REG_WIDGET( XS::StringInspector );

REG_INSPECTOR( XE::String, XS::StringInspector );

XS::StringInspector::StringInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent )
{
	QWidget * widget = new QWidget( this );

	QHBoxLayout * verticalLayout = new QHBoxLayout( this );

	verticalLayout->setSpacing( 0 );
	verticalLayout->setObjectName( QString::fromUtf8( "layout" ) );
	verticalLayout->setContentsMargins( 0, 0, 0, 0 );

	_QLineEdit = new QLineEdit( this );
	QToolButton * button = new QToolButton( this );
	button->setMaximumWidth( 15 );
	button->setIcon( QIcon( "SkinIcons:/images/common/icon_radiobutton_checked.png" ) );

	verticalLayout->addWidget( _QLineEdit );
	verticalLayout->addWidget( button );

	SetContentWidget( widget );

	connect( button, &QToolButton::clicked, [this]()
		{
			QDialog * dialog = new QDialog( this );
			{
				Ui::TextDialog ui;
				ui.setupUi( dialog );

				dialog->setWindowTitle( GetObjectProxy()->GetName().c_str() );
				dialog->setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );

				ui.textEdit->setText( _QLineEdit->text() );
				auto cursor = ui.textEdit->textCursor();
				cursor.movePosition( QTextCursor::End );
				ui.textEdit->setTextCursor( cursor );

				auto callback = [this, dialog, text = ui.textEdit]( bool accept )
				{
					if ( accept )
					{
						_QLineEdit->setText( text->toPlainText() );
					}

					dialog->deleteLater();
				};

				connect( dialog, &QDialog::rejected, [callback]() { callback( false ); } );
				connect( ui.buttonBox, &QDialogButtonBox::accepted, [callback]() { callback( true ); } );
				connect( ui.buttonBox, &QDialogButtonBox::rejected, [callback]() { callback( false ); } );
			}
			dialog->show();
		} );
}

XS::StringInspector::~StringInspector()
{

}

void XS::StringInspector::Refresh()
{
	disconnect( _QLineEdit, nullptr );

	_QLineEdit->setText( GetObjectProxy()->GetValue().Value<XE::String>().c_str() );

	connect( _QLineEdit, &QLineEdit::textChanged, [this]( const QString & text )
		{
			XE::String old = GetObjectProxy()->GetValue().Value<XE::String>();
			if ( text != old.c_str() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = XE::String( text.toStdString() )]()
				{
					proxy->SetValue( value );
					_QLineEdit->setText( value.c_str() );
				},
					[this, proxy = GetObjectProxy(), value = old]()
				{
					proxy->SetValue( value );
					_QLineEdit->setText( value.c_str() );
				} );
			}
		} );
}

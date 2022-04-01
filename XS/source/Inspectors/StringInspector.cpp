#include "StringInspector.h"

#include <QDialog>
#include <QTextEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>

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
				auto desktop_rect = QApplication::desktop()->screenGeometry( 0 );

				dialog->resize( desktop_rect.width() * 0.3f, desktop_rect.height() * 0.6f );

				auto verticalLayout = new QVBoxLayout( dialog );
				auto textEdit = new QTextEdit( dialog );
				textEdit->setText( _QLineEdit->text() );
				auto cursor = textEdit->textCursor();
				cursor.movePosition( QTextCursor::End );
				textEdit->setTextCursor( cursor );

				verticalLayout->addWidget( textEdit );

				auto buttonBox = new QDialogButtonBox( dialog );
				buttonBox->setOrientation( Qt::Horizontal );
				buttonBox->setStandardButtons( QDialogButtonBox::Cancel | QDialogButtonBox::Ok );

				verticalLayout->addWidget( buttonBox );

				QObject::connect( buttonBox, SIGNAL( accepted() ), dialog, SLOT( accept() ) );
				QObject::connect( buttonBox, SIGNAL( rejected() ), dialog, SLOT( reject() ) );

				dialog->setWindowTitle( GetObjectProxy()->GetName().c_str() );
				dialog->setWindowFlags( Qt::Dialog | Qt::WindowCloseButtonHint );


				auto callback = [this, dialog, text = textEdit]( bool accept )
				{
					if ( accept )
					{
						_QLineEdit->setText( text->toPlainText() );
					}

					dialog->deleteLater();
				};

				connect( dialog, &QDialog::accepted, [callback]() { callback( true ); } );
				connect( dialog, &QDialog::rejected, [callback]() { callback( false ); } );
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

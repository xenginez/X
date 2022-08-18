#include "StringInspector.h"

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>

REG_WIDGET( XS::StringInspector );

namespace Ui
{
	class StringInspector
	{
	public:
		QHBoxLayout * horizontalLayout;
		QLineEdit * lineEdit;
		QToolButton * toolButton;

		void setupUi( QWidget * StringInspector )
		{
			if ( StringInspector->objectName().isEmpty() )
				StringInspector->setObjectName( QString::fromUtf8( "StringInspector" ) );
			StringInspector->resize( 219, 20 );
			horizontalLayout = new QHBoxLayout( StringInspector );
			horizontalLayout->setSpacing( 0 );
			horizontalLayout->setObjectName( QString::fromUtf8( "horizontalLayout" ) );
			horizontalLayout->setContentsMargins( 0, 0, 0, 0 );
			lineEdit = new QLineEdit( StringInspector );
			lineEdit->setObjectName( QString::fromUtf8( "lineEdit" ) );

			horizontalLayout->addWidget( lineEdit );

			toolButton = new QToolButton( StringInspector );
			toolButton->setObjectName( QString::fromUtf8( "toolButton" ) );
			QSizePolicy sizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
			sizePolicy.setHorizontalStretch( 0 );
			sizePolicy.setVerticalStretch( 0 );
			sizePolicy.setHeightForWidth( toolButton->sizePolicy().hasHeightForWidth() );
			toolButton->setSizePolicy( sizePolicy );
			toolButton->setMinimumSize( QSize( 15, 0 ) );
			toolButton->setMaximumSize( QSize( 15, 16777215 ) );
			toolButton->setIcon( QIcon( "SkinIcons:/images/common/icon_radiobutton_checked.png" ) );

			horizontalLayout->addWidget( toolButton );
		}
	};
}

REG_INSPECTOR( XE::String, XS::StringInspector );

XS::StringInspector::StringInspector( QWidget * parent /*= nullptr */ )
	:Inspector( parent ), ui( new Ui::StringInspector )
{
	ui->setupUi( this );

	connect( ui->toolButton, &QToolButton::clicked, [this]()
		{
			QDialog * dialog = new QDialog( this );
			{
				auto desktop_rect = QApplication::desktop()->screenGeometry( 0 );

				dialog->resize( desktop_rect.width() * 0.3f, desktop_rect.height() * 0.6f );

				auto verticalLayout = new QVBoxLayout( dialog );
				auto textEdit = new QTextEdit( dialog );
				textEdit->setText( ui->lineEdit->text() );
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
						ui->lineEdit->setText( text->toPlainText() );
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
	delete ui;
}

void XS::StringInspector::Refresh()
{
	disconnect( ui->lineEdit, nullptr );

	ui->lineEdit->setText( GetObjectProxy()->GetValue().Value<XE::String>().c_str() );

	connect( ui->lineEdit, &QLineEdit::textChanged, [this]( const QString & text )
		{
			XE::String old = GetObjectProxy()->GetValue().Value<XE::String>();
			if ( text != old.c_str() )
			{
				PushUndoCommand( GetObjectProxy()->GetName().c_str(),
					[this, proxy = GetObjectProxy(), value = XE::String( text.toStdString() )]()
				{
					proxy->SetValue( value );
					ui->lineEdit->setText( value.c_str() );
				},
					[this, proxy = GetObjectProxy(), value = old]()
				{
					proxy->SetValue( value );
					ui->lineEdit->setText( value.c_str() );
				} );
			}
		} );
}

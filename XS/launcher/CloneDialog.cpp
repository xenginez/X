#include "CloneDialog.h"

#include "ui_CloneDialog.h"

#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>

Launcher::CloneDialog::CloneDialog( QWidget * parent /*= nullptr */ )
	:QDialog( parent ), ui( new Ui::CloneDialog )
{
	ui->setupUi( this );

	ui->finish_button->hide();

	connect( ui->url_button, &QToolButton::clicked, this, &CloneDialog::OnUrlButtonClicked );
	connect( ui->dir_button, &QToolButton::clicked, this, &CloneDialog::OnDirButtonClicked );
	connect( ui->cancel_button, &QToolButton::clicked, this, &CloneDialog::OnCancelButtonClicked );
	connect( ui->finish_button, &QToolButton::clicked, this, &CloneDialog::OnFinishButtonClicked );
	connect( ui->confirm_button, &QToolButton::clicked, this, &CloneDialog::OnConfirmButtonClicked );
}

Launcher::CloneDialog::~CloneDialog()
{
	delete ui;
}

QString Launcher::CloneDialog::url() const
{
	return ui->url_lineEdit->text();
}

QString Launcher::CloneDialog::dir() const
{
	return ui->dir_lineEdit->text();
}

void Launcher::CloneDialog::OnUrlButtonClicked()
{
	QString url = QFileDialog::getExistingDirectory( this, "git clone url", QDir::homePath() );

	if( url != "" )
	{
		ui->url_lineEdit->setText( url );
	}
}

void Launcher::CloneDialog::OnDirButtonClicked()
{
	QString path = ui->dir_lineEdit->text();

	QString dir = QFileDialog::getExistingDirectory( this, "git clone directory", path.isEmpty() ? QDir::homePath() : path );

	if( dir != "" )
	{
		ui->dir_lineEdit->setText( dir );
	}
}

void Launcher::CloneDialog::OnCancelButtonClicked()
{
	reject();
}

void Launcher::CloneDialog::OnFinishButtonClicked()
{
	accept();
}

void Launcher::CloneDialog::OnConfirmButtonClicked()
{
	ui->finish_button->show();
	ui->confirm_button->hide();
	ui->finish_button->setEnabled( false );

	QProcess * process = new QProcess( this );

	connect( process, QOverload<int>::of( &QProcess::finished ), [this, process]( int code )
	{
		if( code == 0 )
		{
			ui->textEdit->append( tr( "complete " ) + QDateTime::currentDateTime().toString( Qt::LocalDate ) );

			ui->finish_button->show();
			ui->finish_button->setEnabled( true );
		}
		else
		{
			ui->textEdit->append( tr( "failed " ) + QDateTime::currentDateTime().toString( Qt::LocalDate ) );

			ui->finish_button->hide();
			ui->confirm_button->show();

			process->deleteLater();
		}

		ui->cancel_button->show();
	} );
	connect( process, &QProcess::readyRead, [this, process]()
	{
		while( true )
		{
			QByteArray data = process->readLine();

			if (data.isEmpty())
			{
				break;
			}

			ui->textEdit->append( data );
			ui->textEdit->update();
		}
	} );
	process->setProcessChannelMode( QProcess::MergedChannels );

	QString cmd;
	QFileInfo file( ui->url_lineEdit->text() );

	cmd += "git clone --progress --recursive -v \"" + ui->url_lineEdit->text() + "\" \"" + QDir::toNativeSeparators( ui->dir_lineEdit->text() ) + QDir::separator() + file.baseName() + "\"";

	process->start( cmd, QProcess::ReadOnly );
}

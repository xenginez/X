#include "LoggerDockWidget.h"

#include "ui_LoggerDockWidget.h"

#include <QStandardItemModel>

REG_WIDGET( XS::LoggerDockWidget );

XS::LoggerDockWidget::LoggerDockWidget( QWidget * parent /*= nullptr */ )
	:XS::DockWidget( parent ), ui( new Ui::LoggerDockWidget )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_logger.png" ) );

	setTitleBar( ui->title_bar );

	ui->clear->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_clear.png" ) );
	ui->merge->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_merge.png" ) );
	ui->error->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_error.png" ) );
	ui->warning->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_warning.png" ) );
	ui->info->setIcon( QIcon( "SkinIcons:/images/logger/icon_logger_info.png" ) );

	ui->search->addAction( QIcon( "SkinIcons:/images/logger/icon_logger_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	connect( ui->clear, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnClearButtonClicked );
	connect( ui->merge, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnMergeButtonClicked );
	connect( ui->error, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnErrorButtonClicked );
	connect( ui->warning, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnWarningButtonClicked );
	connect( ui->info, &QToolButton::clicked, this, &XS::LoggerDockWidget::OnInfoButtonClicked );
	connect( ui->search, &QLineEdit::editingFinished, this, &XS::LoggerDockWidget::OnSearchEditingFinished );

	_Logger = XE::Logger::RegisterListener( { &XS::LoggerDockWidget::OnLoggerListener, this } );
}

XS::LoggerDockWidget::~LoggerDockWidget()
{
	delete ui;
}

void XS::LoggerDockWidget::Save( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		settings.setValue( "merge", ui->merge->isChecked() );
		settings.setValue( "info", ui->info->isChecked() );
		settings.setValue( "error", ui->error->isChecked() );
		settings.setValue( "warning", ui->warning->isChecked() );
	}
	settings.endGroup();
}

void XS::LoggerDockWidget::Load( QSettings & settings )
{
	settings.beginGroup( objectName() );
	{
		ui->merge->setChecked( settings.value( "merge", false ).toBool() );
		ui->info->setChecked( settings.value( "info", true ).toBool() );
		ui->error->setChecked( settings.value( "error", true ).toBool() );
		ui->warning->setChecked( settings.value( "warning", true ).toBool() );
	}
	settings.endGroup();
}

void XS::LoggerDockWidget::OnErrorButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnWarningButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnInfoButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnClearButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnMergeButtonClicked( bool checked )
{

}

void XS::LoggerDockWidget::OnSearchEditingFinished()
{

}

void XS::LoggerDockWidget::OnLoggerListener( std::chrono::system_clock::time_point time, const char * file, XE::uint32 line, XE::LoggerLevel level, XE::String msg )
{

}

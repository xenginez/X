#include "LoggerEditor.h"

#include "ui_LoggerEditor.h"

#include <QUrl>
#include <QDir>
#include <QTimer>
#include <QProcess>
#include <QItemDelegate>
#include <QDesktopServices>

#include "MainWindow.h"

namespace
{
	class ListViewItemDelegate : public QItemDelegate
	{
	public:
		explicit ListViewItemDelegate( QObject * parent = nullptr )
			: QItemDelegate( parent )
		{
		}

		~ListViewItemDelegate() = default;

		QSize sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const override
		{
			QSize size = QItemDelegate::sizeHint( option, index );
			size.setHeight( std::max( size.height(), 20 ) );
			return size;
		}
	};
}

Q_DECLARE_METATYPE( XE::LoggerLevel );

REG_WIDGET( XS::LoggerEditor );

XS::LoggerEditor::LoggerEditor( QWidget * parent /*= nullptr */ )
	:XS::EditorWindow( parent ), ui( new Ui::LoggerEditor )
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

	_Model = new QStandardItemModel( this );
	_Model->setColumnCount( 1 );
	ui->list->setModel( _Model );
	ui->list->setItemDelegate( new ListViewItemDelegate( this ) );

	connect( ui->clear, &QToolButton::clicked, this, &XS::LoggerEditor::OnClearButtonClicked );
	connect( ui->merge, &QToolButton::clicked, this, &XS::LoggerEditor::OnMergeButtonClicked );
	connect( ui->info, &QToolButton::clicked, this, &XS::LoggerEditor::OnInfoButtonClicked );
	connect( ui->error, &QToolButton::clicked, this, &XS::LoggerEditor::OnErrorButtonClicked );
	connect( ui->warning, &QToolButton::clicked, this, &XS::LoggerEditor::OnWarningButtonClicked );
	connect( ui->search, &QLineEdit::editingFinished, this, &XS::LoggerEditor::OnSearchEditingFinished );
	connect( ui->list, &QListView::doubleClicked, this, &XS::LoggerEditor::OnListViewItemDoubleClicked );

	_Logger = XE::Logger::RegisterListener( [this]( std::chrono::system_clock::time_point time, const char * file, XE::uint32 line, const char * func, XE::LoggerLevel level, const XE::String & msg )
	{
		OnLoggerListener( time, file, line, func, level, msg );
	} );
}

XS::LoggerEditor::~LoggerEditor()
{
	delete ui;
}

void XS::LoggerEditor::SaveLayout( QSettings & settings )
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

void XS::LoggerEditor::LoadLayout( QSettings & settings )
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

void XS::LoggerEditor::OnErrorButtonClicked( bool checked )
{
	if ( checked == false )
	{
		for ( size_t i = 0; i < _Model->rowCount(); )
		{
			if ( _Model->item( i, 0 )->data( Qt::UserRole + 4 ).value<XE::LoggerLevel>() == XE::LoggerLevel::Error )
			{
				_Model->removeRow( i );
			}
			else
			{
				i++;
			}
		}
	}
}

void XS::LoggerEditor::OnWarningButtonClicked( bool checked )
{
	if ( checked == false )
	{
		for ( size_t i = 0; i < _Model->rowCount(); )
		{
			if ( _Model->item( i, 0 )->data( Qt::UserRole + 4 ).value<XE::LoggerLevel>() == XE::LoggerLevel::Warning )
			{
				_Model->removeRow( i );
			}
			else
			{
				i++;
			}
		}
	}
}

void XS::LoggerEditor::OnInfoButtonClicked( bool checked )
{
	if ( checked == false )
	{
		for ( size_t i = 0; i < _Model->rowCount(); )
		{
			if ( _Model->item( i, 0 )->data( Qt::UserRole + 4 ).value<XE::LoggerLevel>() == XE::LoggerLevel::Message )
			{
				_Model->removeRow( i );
			}
			else
			{
				i++;
			}
		}
	}
}

void XS::LoggerEditor::OnClearButtonClicked( bool checked )
{
	_Model->clear();
}

void XS::LoggerEditor::OnMergeButtonClicked( bool checked )
{
	if ( checked )
	{
		for ( size_t i = 1; i < _Model->rowCount(); )
		{
			bool merge = false;
			QString data_1 = _Model->item( i, 0 )->data( Qt::UserRole + 1 ).toString();
			XE::uint32 data_2 = _Model->item( i, 0 )->data( Qt::UserRole + 2 ).toUInt();
			QString data_3 = _Model->item( i, 0 )->data( Qt::UserRole + 3 ).toString();
			for ( size_t j = 0; j < i; j++ )
			{
				if (
					data_1 == _Model->item( j, 0 )->data( Qt::UserRole + 1 ).toString() &&
					data_2 == _Model->item( j, 0 )->data( Qt::UserRole + 2 ).toUInt() &&
					data_3 == _Model->item( j, 0 )->data( Qt::UserRole + 3 ).toString()
					)
				{
					merge = true;

					QWidget * i_widget = ui->list->indexWidget( _Model->index( i, 0 ) );
					QWidget * j_widget = ui->list->indexWidget( _Model->index( j, 0 ) );

					QLabel * i_count = i_widget->findChild<QLabel *>( "count" );
					QLabel * j_count = j_widget->findChild<QLabel *>( "count" );
					j_count->setText( QString::number( j_count->text().toUInt() + i_count->text().toUInt() ) );

					QLabel * i_message = i_widget->findChild<QLabel *>( "message" );
					QLabel * j_message = j_widget->findChild<QLabel *>( "message" );
					j_message->setText( i_message->text() ); j_message->setToolTip( i_message->toolTip() );

					_Model->removeRow( i );
					break;
				}
			}

			i = ( merge ? i : i + 1 );
		}
	}
}

void XS::LoggerEditor::OnSearchEditingFinished()
{
	QString search = ui->search->text();
	for ( size_t i = 0; i < _Model->rowCount(); i++ )
	{
		QWidget * widget = ui->list->indexWidget( _Model->index( i, 0 ) );
		if ( widget != nullptr )
		{
			QLabel * message = widget->findChild<QLabel *>( "message" );
			if ( message != nullptr )
			{
				if ( message->text().contains( search ) )
				{
					ui->list->setCurrentIndex( _Model->index( i, 0 ) );
					return;
				}
			}
		}
	}
	ui->list->setCurrentIndex( QModelIndex() );
}

void XS::LoggerEditor::OnListViewItemDoubleClicked( const QModelIndex & index )
{
	auto item = _Model->itemFromIndex( index );

	auto file = item->data( Qt::UserRole + 1 ).toString();
	auto line = item->data( Qt::UserRole + 2 ).toUInt();

	QString vscode;

#if PLATFORM_OS == OS_WINDOWS
	QStringList list;
	list << R"(HKEY_CLASSES_ROOT\Applications\Code.exe\shell\open\command)";
	list << R"(HKEY_CURRENT_USER\SOFTWARE\Classes\Applications\Code.exe\shell\open\command)";
	for ( const auto & it : list )
	{
		QSettings * setting = new QSettings( it, QSettings::NativeFormat );

		auto value = setting->value( ".", "" ).toString();
		if ( !value.isEmpty() )
		{
			value.replace( "\"", "" );
			vscode = value.replace( " %1", "" );
			break;
		}
	}
#elif PLATFORM_OS == OS_LINUX
#elif PLATFORM_OS == OS_MAC
#endif

	if ( vscode.isEmpty() || QProcess::startDetached( vscode, QStringList() << "-r" << "-g" << QString( "%0:%1" ).arg( QDir( QApplication::applicationDirPath() ).absoluteFilePath( file ) ).arg( line ) ) == false )
	{
		QDesktopServices::openUrl( QUrl::fromLocalFile( QDir( QApplication::applicationDirPath() ).absoluteFilePath( file ) ) );
	}
}

void XS::LoggerEditor::OnLoggerListener( std::chrono::system_clock::time_point time, const char * file, XE::uint32 line, const char * func, XE::LoggerLevel level, const XE::String & msg )
{
	switch ( level )
	{
	case XE::LoggerLevel::Error:
		if ( !ui->error->isChecked() )
		{
			return;
		}
		break;
	case XE::LoggerLevel::Warning:
		if ( !ui->warning->isChecked() )
		{
			return;
		}
		break;
	case XE::LoggerLevel::Message:
		if ( !ui->info->isChecked() )
		{
			return;
		}
		break;
	default:
		return;
	}

	struct
	{
		QString thread;
		QString message;
		XE::uint32 line;
		const char * file;
		const char * func;
		XE::LoggerLevel level;
		std::chrono::system_clock::time_point time;
	} info;

	info.time = time;
	info.file = file;
	info.line = line;
	info.func = func;
	info.level = level;
	info.message = QString::fromUtf8( reinterpret_cast<const char *>( msg.c_str() ) );
	if ( GetParent< XS::MainWindow >()->GetEditorThreadId() == std::this_thread::get_id() )
	{
		info.thread = "EDITOR";
	}
	else
	{
		switch ( XE::CoreFramework::GetCurrentFramework()->GetServiceT< XE::ThreadService >()->GetCurrentThreadType() )
		{
		case XE::ThreadType::IO:
			info.thread = "IO";
			break;
		case XE::ThreadType::GAME:
			info.thread = "GAME";
			break;
		case XE::ThreadType::AUDIO:
			info.thread = "AUDIO";
			break;
		case XE::ThreadType::RENDER:
			info.thread = "RENDER";
			break;
		case XE::ThreadType::PHYSICS:
			info.thread = "PHYSICS";
			break;
		case XE::ThreadType::WORKS:
			info.thread = "WORKS";
			break;
		default:
			info.thread = "UNKNOWN";
			break;
		}
	}

	QMetaObject::invokeMethod( this, [this, info]()
		{
			if ( ui->merge->isChecked() )
			{
				for ( size_t i = 0; i < _Model->rowCount(); i++ )
				{
					auto it = _Model->item( i, 0 );
					if ( 
						_Model->item( i, 0 )->data( Qt::UserRole + 1 ).toString() == info.file &&
						_Model->item( i, 0 )->data( Qt::UserRole + 2 ).toUInt() == info.line &&
						_Model->item( i, 0 )->data( Qt::UserRole + 3 ).toString() == info.thread )
					{
						QWidget * widget = ui->list->indexWidget( _Model->index( i, 0 ) );

						auto count = widget->findChild<QLabel *>( "count" );
						count->setText( QString::number( count->text().toUInt() + 1 ) );

						auto message = widget->findChild<QLabel *>( "message" );
						message->setText( info.message );
						message->setToolTip( QString( XE::ToString( info.time ).c_str() ) + "\n" + QString( info.file ) + ":" + QString::number( info.line ) + "  " + info.func +"\n" + info.message );

						return;
					}
				}
			}

			QStandardItem * item = new QStandardItem();
			auto height = 20;
			
			QWidget * widget = new QWidget( ui->list );
			QHBoxLayout * layout = new QHBoxLayout( widget );
			QLabel * icon = new QLabel( widget );
			icon->setObjectName( "icon" );
			icon->setFixedSize( height, height );
			icon->setToolTip( "icon" );
			QLabel * thread = new QLabel( widget );
			thread->setObjectName( "thread" );
			thread->setText( info.thread );
			thread->setFixedSize( 60, height );
			auto font = thread->font(); font.setBold( true ); thread->setFont( font );
			thread->setToolTip( "thread" );
			QLabel * message = new QLabel( widget );
			message->setObjectName( "message" );
			message->setText( info.message );
			message->setFixedHeight( height );
			message->setToolTip( QString( XE::ToString( info.time ).c_str() ) + "\n" + QString( info.file ) + ":" + QString::number( info.line ) + "\n" + info.message );
			QLabel * count = new QLabel( widget );
			count->setObjectName( "count" );
			count->setText( QString::number( 1 ) );
			count->setFixedSize( 50, height );
			count->setAlignment( Qt::AlignRight );
			count->setToolTip( "count" );
			layout->setContentsMargins( 0, 0, 0, 0 );
			layout->setSpacing( 5 );
			layout->addWidget( icon ); layout->addWidget( thread ); layout->addWidget( message ); layout->addWidget( count );
			widget->setFixedHeight( height );
			widget->setLayout( layout );

			switch ( info.level )
			{
			case XE::LoggerLevel::Error:
			{
				icon->setPixmap( ui->error->icon().pixmap( height, height ) );
				message->setStyleSheet( "color: rgb(207, 55, 54);" );
			}
				break;
			case XE::LoggerLevel::Message:
			{
				icon->setPixmap( ui->info->icon().pixmap( height, height ) );
				message->setStyleSheet( "color: rgb(15, 172, 255);" );
			}
				break;
			case XE::LoggerLevel::Warning:
			{
				icon->setPixmap( ui->warning->icon().pixmap( height, height ) );
				message->setStyleSheet( "color: rgb(250, 173, 20);" );
			}
				break;
			}

			item->setData( info.file, Qt::UserRole + 1 );
			item->setData( info.line, Qt::UserRole + 2 );
			item->setData( info.thread, Qt::UserRole + 3 );
			item->setData( QVariant::fromValue( info.level ), Qt::UserRole + 4 );

			auto row = _Model->rowCount();
			_Model->setRowCount( row + 1 );
			_Model->setItem( row, 0, item );
			ui->list->setIndexWidget( _Model->index( row, 0 ), widget );

		}, Qt::QueuedConnection );
}

#include "AssetExplorerEditor.h"

#include "ui_AssetExplorerEditor.h"

#include <QMenu>
#include <QDateTime>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileSystemModel>
#include <QFileIconProvider>

#include "CoreFramework.h"
#include "QMetaStaticCall.h"

#define database() (XS::CoreFramework::GetCurrentFramework()->GetAssetDatabase())
#define PATH_ROLE (Qt::UserRole + 1)
#define UUID_ROLE (Qt::UserRole + 2)
#define EDITOR_ROLE (Qt::UserRole + 3)

REG_WIDGET( XS::AssetExplorerEditor );

class XS::AssetsItemModel : public QAbstractItemModel
{
public:
	struct AssetsItem : public QObject
	{
		AssetsItem( const QFileInfo & info, AssetsItem * parent = nullptr )
			: QObject( parent ), info( info ), parent( parent )
		{

		}

		~AssetsItem()
		{
			for ( auto it : children )
			{
				it->deleteLater();
			}
		}

		int row() const
		{
			if ( parent != nullptr )
			{
				for ( int i = 0; i < parent->children.size(); i++ )
				{
					if ( parent->children[i] == this )
					{
						return i;
					}
				}
			}

			return 0;
		}
		AssetsItem * find( const QString & path )
		{
			if ( QFileInfo( path ) == info.filePath() )
			{
				return this;
			}

			if ( info.filePath().contains( path ) ) // path.contains( info.filePath() ) )
			{
				for ( auto it : children )
				{
					auto item = it->find( path );
					if ( item != nullptr )
					{
						return item;
					}
				}
			}

			return nullptr;
		}

		QFileInfo info;
		AssetsItem * parent = nullptr;
		QList< AssetsItem * > children;
	};

public:
	static constexpr std::array<const char *, 7> TopLevelPaths =
	{
		XS::FOLLOW_DIRECTORY,
		" ",
		XE::ASSETS_DIRECTORY,
		XS::EDITORS_DIRECTORY,
		XE::CONFIGS_DIRECTORY,
		" ",
		XS::PACKAGES_DIRECTORY,
	};

public:
	AssetsItemModel( const QString & root, QObject * parent = nullptr )
		: QAbstractItemModel( parent ), _RootItem( new AssetsItem( root, nullptr ) )
	{
		for ( int i = 0; i < TopLevelPaths.size(); i++ )
		{
			_RootItem->children.push_back( new AssetsItem( _RootItem->info.filePath() + "/" + TopLevelPaths[i], _RootItem ) );
		}
	}

	~AssetsItemModel()
	{
		delete _RootItem;
	}

public:
	QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const override
	{
		if ( row < 0 || column != 0 )
		{
			return {};
		}

		AssetsItem * item = ( parent.isValid() ) ? reinterpret_cast<AssetsItem *>( parent.internalPointer() ) : _RootItem;

		if ( row < item->children.size() )
		{
			return createIndex( row, column, item->children[row] );
		}
		else if ( isRoot( parent ) )
		{
			if ( row < _RootItem->children.size() )
			{
				return createIndex( row, column, _RootItem->children[row] );
			}
		}
		else if ( isSpacing( parent ) )
		{
			return {};
		}
		else if ( isPackage( parent ) )
		{
			return {};
		}
		else if ( isFollow( parent ) )
		{
			if ( row < followItem()->children.size() )
			{
				return createIndex( row, column, followItem()->children[row] );
			}
		}
		else if ( item->info.isDir() )
		{
			auto list = QDir( item->info.filePath() ).entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
			if ( row < list.size() )
			{
				AssetsItem * child = new AssetsItem( list[row], item );

				item->children.push_back( child );

				return createIndex( row, column, child );
			}
		}

		return {};
	}
	QModelIndex parent( const QModelIndex & child ) const override
	{
		if ( !child.isValid() )
		{
			return {};
		}

		AssetsItem * item = reinterpret_cast<AssetsItem *>( child.internalPointer() );

		if ( item->parent == nullptr || item->parent == _RootItem )
		{
			return {};
		}

		return createIndex( item->parent->row(), 0, item->parent );
	}
	int rowCount( const QModelIndex & parent = QModelIndex() ) const override
	{
		AssetsItem * item = ( parent.isValid() ) ? reinterpret_cast<AssetsItem *>( parent.internalPointer() ) : _RootItem;

		if ( item == _RootItem )
		{
			return TopLevelPaths.size();
		}
		else if ( isSpacing( parent ) )
		{
			return 0;
		}
		else if ( isPackage( parent ) )
		{
			return 0;
		}
		else if ( isFollow( parent ) )
		{
			return item->children.size();
		}
		else if ( isFollow( createIndex( 0, 0, item->parent ) ) )
		{
			return 0;
		}
		else if ( item->info.isDir() )
		{
			return QDir( item->info.filePath() ).entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot ).size();
		}

		return 0;
	}
	int columnCount( const QModelIndex & parent = QModelIndex() ) const override
	{
		return 1;
	}
	Qt::ItemFlags flags( const QModelIndex & index ) const override
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		if ( isSpacing( index ) )
		{
			return Qt::NoItemFlags;
		}

		Qt::ItemFlags flags = QAbstractItemModel::flags( index ) | Qt::ItemIsEnabled;

		if ( item->info.isDir() ) flags |= Qt::ItemIsDragEnabled;

		return flags;
	}
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override
	{
		switch ( role )
		{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return fileName( index );
		case Qt::ToolTipRole:
			return filePath( index );
		case Qt::DecorationRole:
			return fileIcon( index );
		case Qt::TextAlignmentRole:
				return QVariant( Qt::AlignLeft | Qt::AlignVCenter );
		}

		return {};
	}
	bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() ) override
	{
		beginInsertRows( parent, row, row );
		{
			if ( isFollow( parent ) )
			{
				return removeFollow( createIndex( row, 0, followItem()->children[row] ) );
			}
		}
		endInsertRows();

		return false;
	}

public:
	bool isDir( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->info.isDir();
	}
	bool isRoot( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item == _RootItem;
	}
	bool isSpacing( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item->info.fileName() == " ";
	}
	bool isPackage( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item == _RootItem->children.back();
	}
	bool isFollow( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item == _RootItem->children.front();
	}
	bool isFollowChild( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == followItem();
	}

public:
	QIcon fileIcon( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		if ( isSpacing( index ) )
		{
			return {};
		}
		else if ( isPackage( index ) )
		{
			return QIcon( "SkinIcons:/images/assets/icon_assets_package.png" );
		}
		else if ( isFollow( index ) )
		{
			return QIcon( "SkinIcons:/images/assets/icon_assets_follows.png" );
		}
		else if ( isDir( index ) )
		{
			return QIcon( "SkinIcons:/images/assets/icon_assets_folder.png" );
		}
		else
		{
			return QIcon( "SkinIcons:/images/assets/icon_assets_folder.png" );
		}
	}
	QString fileName( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		if ( isSpacing( index ) )
		{
			return "";
		}

		return item->info.fileName();
	}
	QString filePath( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		if ( isSpacing( index ) )
		{
			return "";
		}

		return item->info.filePath();
	}
	QFileInfo fileInfo( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		if ( isSpacing( index ) )
		{
			return {};
		}

		return item->info;
	}

public:
	QModelIndex rootIndex() const
	{
		return createIndex( 0, 0, _RootItem );
	}
	QModelIndex index( const QString & path ) const
	{
		auto item = _RootItem->find( path );

		return createIndex( item->row(), 0, item );
	}

public:
	QList<QString> follows() const
	{
		QList<QString> result;

		for ( auto it : followItem()->children )
		{
			result.push_back( it->info.filePath() );
		}

		return result;
	}
	void setFollows(const QList<QString> & list ) const
	{
		for ( auto it : list )
		{
			followItem()->children.push_back( new AssetsItem( it, followItem() ) );
		}
	}
	void addFollow( const QModelIndex & index )
	{
		auto item = reinterpret_cast<AssetsItem *>( index.internalPointer() );
		if ( item != nullptr )
		{
			auto it = std::find_if( followItem()->children.begin(), followItem()->children.end(), [&item]( const auto & val )
				{
					return val->info == item->info;
				} );
			if ( it == followItem()->children.end() )
			{
				followItem()->children.push_back( new AssetsItem( item->info, followItem() ) );

				layoutChanged( { createIndex( 0, 0, followItem() ) } );
			}
		}
	}
	bool removeFollow( const QModelIndex & index )
	{
		auto item = reinterpret_cast<AssetsItem *>( index.internalPointer() );
		if ( item != nullptr && item->parent == followItem() )
		{
			auto it = std::find( followItem()->children.begin(), followItem()->children.end(), item );
			if ( it != followItem()->children.end() )
			{
				followItem()->children.erase( it );
			}
			layoutChanged( { createIndex( 0, 0, followItem() ) } );

			return true;
		}
		return false;
	}

private:
	AssetsItem * followItem() const
	{
		return _RootItem->children[0];
	}
	AssetsItem * packageItem() const
	{
		return _RootItem->children.back();
	}

private:
	AssetsItem * _RootItem;
};

XS::AssetExplorerEditor::AssetExplorerEditor( QWidget * parent /*= nullptr */ )
	:ToolEditorWindow( parent ), ui( new Ui::AssetExplorerEditor )
{
	setupUi( ui );
	setWindowIcon( icon() );
	setTitleBar( ui->title_bar );

	ui->add->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_add.png" ) );
	ui->type->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_type.png" ) );
	ui->label->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_label.png" ) );
	ui->invisible->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_invisible.png" ) );
	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/assets/icon_assets_folder.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/assets/icon_assets_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	ui->splitter->setSizes( { 3000, 7000 } );

	auto project_path = QDir::toNativeSeparators( QString::fromStdString( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string() ) );
	_Model = new XS::AssetsItemModel( project_path, this );
	ui->tree->setModel( _Model );
	ui->tree->setRootIndex( _Model->rootIndex() );
//	ui->tree->setCurrentIndex( _Model->index( QDir::toNativeSeparators( project_path + "/" + XE::ASSETS_DIRECTORY ) ) );

	QList< const QMetaObject * > list = XS::Registry::GetDerivedClass( &XS::AssetEditor::staticMetaObject );
	for ( const auto & it : list )
	{
		_Editors.push_back( XS::Registry::ConstructT< XS::AssetEditor >( it->className(), this ) );
	}

	connect( ui->tree, &QTreeView::clicked, this, &AssetExplorerEditor::OnTreeViewClicked );
	connect( ui->scale, &QSlider::valueChanged, this, &AssetExplorerEditor::OnScaleValueChanged );
	connect( ui->list, &QListWidget::itemChanged, this, &AssetExplorerEditor::OnListWidgetItemChanged );
	connect( ui->list, &QListWidget::itemDoubleClicked, this, &AssetExplorerEditor::OnListWidgetItemDoubleClicked );
	connect( ui->tree, &QTreeView::customContextMenuRequested, this, &AssetExplorerEditor::OnTreeViewCustomContextMenuRequested );
	connect( ui->list, &QListWidget::customContextMenuRequested, this, &AssetExplorerEditor::OnTreeViewCustomContextMenuRequested );
}

XS::AssetExplorerEditor::~AssetExplorerEditor()
{
	delete ui;
}

QIcon XS::AssetExplorerEditor::icon()
{
	return QIcon( "SkinIcons:/images/icons/icon_assets.png" );
}

QString XS::AssetExplorerEditor::name()
{
	return tr( "Assets" );
}

void XS::AssetExplorerEditor::SaveLayout( QSettings & settings )
{
	XS::ToolEditorWindow::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_geometry", ui->splitter->saveGeometry() );
		settings.setValue( "splitter_state", ui->splitter->saveState() );
		settings.setValue( "scale", ui->scale->value() );
	}
	settings.endGroup();
}

void XS::AssetExplorerEditor::LoadLayout( QSettings & settings )
{
	XS::ToolEditorWindow::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreGeometry( settings.value( "splitter_geometry" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_state" ).toByteArray() );
		ui->scale->setValue( settings.value( "scale" ).toInt() );
	}
	settings.endGroup();
}

void XS::AssetExplorerEditor::OnWatcherFileChanged( const QString & path )
{

}

void XS::AssetExplorerEditor::OnWatcherDirectoryChanged( const QString & path )
{

}

void XS::AssetExplorerEditor::OnScaleValueChanged( int value )
{
	if ( value == 0 )
	{
		ui->list->setIconSize( QSize( 20, 20 ) );
		ui->list->setViewMode( QListView::ListMode );
		ui->list->setGridSize( QSize( ui->list->viewport()->width(), 20 ) );
	}
	else
	{
		float scale = (float)ui->scale->value() / (float)ui->scale->maximum();

		ui->list->setIconSize( QSize( 50 + 50 * scale, 50 + 50 * scale ) );
		ui->list->setViewMode( QListView::IconMode );
		ui->list->setGridSize( QSize( 50 + 50 * scale, ( 50 + 50 * scale ) + ( 20 + 20 * scale ) ) );
	}
}

void XS::AssetExplorerEditor::OnTreeViewClicked( const QModelIndex & index )
{
	QFileInfo info( _Model->filePath( index ) );

	ui->path->setText( QDir( _Model->filePath( _Model->rootIndex() ) ).relativeFilePath( info.absoluteFilePath() ) );

	ui->list->clear();
	auto list = QDir( info.filePath() ).entryInfoList( QDir::Files | QDir::NoDotAndDotDot );
	for ( const auto & file : list )
	{
		QUuid uuid = database()->Query( file );

		auto it = std::find_if( _Editors.begin(), _Editors.end(), [&]( XS::AssetEditor * val ) { return val->extensionName() == file.suffix(); } );
		QIcon icon = it != _Editors.end() ? ( *it )->assetIcon( uuid ) : QIcon( "SkinIcons:/images/assets/icon_assets_file.svg" );

		QListWidgetItem * item = new QListWidgetItem( icon, file.baseName(), ui->list );
		{
			item->setData( UUID_ROLE, uuid );
			item->setData( PATH_ROLE, QVariant::fromValue( file ) );
			item->setData( EDITOR_ROLE, (XE::uint64)*it );
			item->setFlags( item->flags() | Qt::ItemIsEditable );
			item->setToolTip( QString( tr(
R"(Name: %1
Type: %2
Uuid: %3
Path: %4
Time: %5 )" ) )
							  .arg( file.baseName() )
							  .arg( file.suffix() )
							  .arg( uuid.toString() )
							  .arg( file.filePath() )
							  .arg( file.lastModified().toString( Qt::LocaleDate ) ) );
		}
		ui->list->addItem( item );
	}
}

void XS::AssetExplorerEditor::OnTreeViewCustomContextMenuRequested( const QPoint & pos )
{
	QModelIndex index = ui->tree->indexAt( ui->tree->viewport()->mapFromGlobal( QCursor::pos() ) );
	
	if ( _Model->isSpacing( index ) || _Model->isFollow( index ) || _Model->isPackage( index ) )
	{
		return;
	}

	QMenu menu;
	{
		if ( _Model->isFollowChild( index ) )
		{
			menu.addAction( tr( "Unfollow" ), [this, index]() { _Model->removeRow( index.row(), index.parent() ); } );
		}
		else
		{
			menu.addAction( tr( "Follow" ), [this, index]() { _Model->addFollow( index ); } );
		}

		auto create = menu.addMenu( tr( "Create" ) );
		{
			for ( auto editor : _Editors )
			{
				create->addAction( editor->icon(), editor->name(), [this, editor]()
				{
					QString name = editor->name();
					QString path = _Model->filePath( ui->tree->currentIndex() );

					QFileInfo dir( QString( "%1/New %2%3.%4" ).arg( path ).arg( name ).arg( "" ).arg( editor->extensionName()));
					for ( int i = 2; dir.exists(); ++i )
					{
						dir = QString( "%1/New %2 %3.%4" ).arg( path ).arg( name ).arg( i ).arg( editor->extensionName() );
					}

					auto uuid = editor->assetCreate( dir );

					OnTreeViewClicked( ui->tree->currentIndex() );

					for ( int i = 0; i < ui->list->count(); i++ )
					{
						auto item = ui->list->item( i );
						if ( item->data( UUID_ROLE ).toUuid() == uuid )
						{
							ui->list->setCurrentItem( item );

							ui->list->editItem( item );
						}
					}
				} );
			}
		}
	}
	menu.exec( QCursor::pos() );
}

void XS::AssetExplorerEditor::OnListWidgetItemChanged( QListWidgetItem * item )
{
	auto uuid = item->data( UUID_ROLE ).toUuid();
	auto path = item->data( PATH_ROLE ).value< QFileInfo >();
	auto editor = (XS::AssetEditor *)item->data( EDITOR_ROLE ).value<XE::uint64>();

	if ( editor != nullptr && path.baseName() != item->text() )
	{
		editor->assetRename( uuid, path, QFileInfo( path.dir().absoluteFilePath( item->text() + "." + editor->extensionName() ) ) );
	}
}

void XS::AssetExplorerEditor::OnListWidgetItemDoubleClicked( QListWidgetItem * item )
{
	qDebug() << "OnListWidgetItemDoubleClicked";
}

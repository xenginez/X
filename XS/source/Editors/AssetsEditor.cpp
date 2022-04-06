#include "AssetsEditor.h"

#include "ui_AssetsEditor.h"

#include <QMenu>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileSystemModel>
#include <QFileIconProvider>

#include "CoreFramework.h"

REG_WIDGET( XS::AssetsEditor );

class XS::AssetsItemModel : public QAbstractItemModel
{
public:
	struct AssetsItem
	{
		AssetsItem( int row, int col, const QFileInfo & info, AssetsItem * parent = nullptr )
			:row( row ), col( col ), info( info ), parent( parent )
		{

		}

		~AssetsItem()
		{
			for ( auto it : children )
			{
				delete it;
			}
		}

		AssetsItem * find( const QString & path )
		{
			if ( path == info.filePath() )
			{
				return this;
			}

			if ( path.contains( info.filePath() ) )
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

		int row = 0;
		int col = 0;
		QFileInfo info;
		AssetsItem * parent = nullptr;
		QList< AssetsItem * > children;
	};

public:
	static constexpr std::array<const char *, 7> TopLevelPaths =
	{
		"Favorites",
		" ",
		"Assets",
		"Editor",
		"UserDatas",
		" ",
		"Package",
	};

public:
	AssetsItemModel( const QString & root, QObject * parent = nullptr )
		: QAbstractItemModel( parent ), _RootItem( new AssetsItem( 0, 0, root, nullptr ) )
	{
		for ( int i = 0; i < TopLevelPaths.size(); i++ )
		{
			_RootItem->children.push_back( new AssetsItem( i, 0, _RootItem->info.filePath() + "/" + TopLevelPaths[i], _RootItem ) );
		}
	}

	~AssetsItemModel()
	{
		delete _RootItem;
	}

public:
	QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const override
	{
		AssetsItem * item = ( parent.isValid() ) ? reinterpret_cast<AssetsItem *>( parent.internalPointer() ) : _RootItem;

		auto it = std::find_if( item->children.begin(), item->children.end(), [row]( const auto & child ) { return child->row == row; } );
		if ( it != item->children.end() )
		{
			return createIndex( row, column, *it );
		}

		auto idx = createIndex( row, column, item );

		if ( isSpacing( idx ) )
		{
			return {};
		}
		else if ( isPackage( idx ) )
		{
			return {};
		}
		else if ( isFavorites( idx ) )
		{
			if ( row < _Favorites.size() )
			{
				return index( _Favorites[row] );
			}
		}
		else if ( item->info.isDir() )
		{
			auto list = QDir( item->info.filePath() ).entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot );
			if ( row < list.size() )
			{
				AssetsItem * child = new AssetsItem( row, column, list[row], item );

				item->children.push_back( child );

				return createIndex( row, column, child );
			}
		}

		return {};
	}
	QModelIndex parent( const QModelIndex & child ) const override
	{
		if (!child.isValid())
		{
			return {};
		}

		AssetsItem * item = reinterpret_cast<AssetsItem *>( child.internalPointer() );

		if ( item->parent == nullptr || item->parent == _RootItem )
		{
			return {};
		}

		return createIndex( item->parent->row, item->parent->col, item->parent );
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
		else if ( isFavorites( parent ) )
		{
			return _Favorites.size();
		}
		else if ( item->info.isDir() )
		{
			return QDir( item->info.filePath() ).entryInfoList( QDir::AllEntries | QDir::NoDotAndDotDot ).size();
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

public:
	bool isDir( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->info.isDir();
	}
	bool isSpacing( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item->info.fileName() == " ";
	}
	bool isPackage( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item->row == TopLevelPaths.size() - 1;
	}
	bool isFavorites( const QModelIndex & index ) const
	{
		AssetsItem * item = ( index.isValid() ) ? reinterpret_cast<AssetsItem *>( index.internalPointer() ) : _RootItem;

		return item->parent == _RootItem && item->row == 0;
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
		else if ( isFavorites( index ) )
		{
			return QIcon( "SkinIcons:/images/assets/icon_assets_favorites.png" );
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

public:
	QModelIndex rootIndex() const
	{
		return createIndex( 0, 0, _RootItem );
	}
	QModelIndex index( const QString & path ) const
	{
		auto item = _RootItem->find( path );

		return createIndex( item->row, item->col, item );
	}

public:
	const QList<QString> & favorites() const
	{
		return _Favorites;
	}
	void setFavorites( const QList<QString> & val )
	{
		_Favorites = val;

		auto idx = createIndex( 0, 0, _RootItem->children[0] );

		dataChanged( idx, idx );
	}
	void addFavorite( const QModelIndex & index )
	{
		auto item = reinterpret_cast<AssetsItem *>( index.internalPointer() );
		if ( item != nullptr && _Favorites.contains( item->info.filePath() ) == false )
		{
			_Favorites.push_back( item->info.filePath() );

			auto idx = createIndex( 0, 0, _RootItem->children[0] );

			dataChanged( idx, idx );
		}
	}
	void removeFavorite( const QString & path )
	{
		_Favorites.removeOne( path );

		auto idx = createIndex( 0, 0, _RootItem->children[0] );

		dataChanged( idx, idx );
	}

private:
	AssetsItem * _RootItem;
	QList< QString > _Favorites;
};

XS::AssetsEditor::AssetsEditor( QWidget * parent /*= nullptr */ )
	:DockWidget( parent ), ui( new Ui::AssetsEditor )
{
	setupUi( ui );

	setWindowIcon( QIcon( "SkinIcons:/images/icons/icon_assets.png" ) );

	setTitleBar( ui->title_bar );

	ui->add->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_add.png" ) );
	ui->type->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_type.png" ) );
	ui->label->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_label.png" ) );
	ui->invisible->setIcon( QIcon( "SkinIcons:/images/assets/icon_assets_invisible.png" ) );
	ui->icon->setPixmap( QPixmap( "SkinIcons:/images/assets/icon_assets_folder.png" ) );
	ui->search->addAction( QIcon( "SkinIcons:/images/assets/icon_assets_search.png" ), QLineEdit::ActionPosition::LeadingPosition );

	auto project_path = QDir::toNativeSeparators( QString::fromStdString( XS::CoreFramework::GetCurrentFramework()->GetProjectPath().string() ) );
	auto watchdb_file = QDir::toNativeSeparators( QString::fromStdString( ( XS::CoreFramework::GetCurrentFramework()->GetProjectPath() / "FileWatcher.db" ).string() ) );

	_Module = new XS::AssetsItemModel( project_path, this );
	ui->tree->setModel( _Module );
	ui->tree->setRootIndex( _Module->rootIndex() );

// 	_LocalDB = QSqlDatabase::addDatabase( "QSQLITE" );
// 	_LocalDB.setHostName( watchdb_file );
// 	_LocalDB.open();

	connect( ui->tree, &QTreeView::customContextMenuRequested, this, &AssetsEditor::OnTreeViewCustomContextMenuRequested );
}

XS::AssetsEditor::~AssetsEditor()
{
	if ( _LocalDB.isOpen() )
	{
		_LocalDB.close();
	}

	delete ui;
}

void XS::AssetsEditor::SaveLayout( QSettings & settings )
{
	DockWidget::SaveLayout( settings );

	settings.beginGroup( objectName() );
	{
		settings.setValue( "splitter_geometry", ui->splitter->saveGeometry() );
		settings.setValue( "splitter_state", ui->splitter->saveState() );
	}
	settings.endGroup();
}

void XS::AssetsEditor::LoadLayout( QSettings & settings )
{
	DockWidget::LoadLayout( settings );

	settings.beginGroup( objectName() );
	{
		ui->splitter->restoreGeometry( settings.value( "splitter_geometry" ).toByteArray() );
		ui->splitter->restoreState( settings.value( "splitter_state" ).toByteArray() );
	}
	settings.endGroup();
}

void XS::AssetsEditor::OnWatcherFileChanged( const QString & path )
{

}

void XS::AssetsEditor::OnWatcherDirectoryChanged( const QString & path )
{

}

void XS::AssetsEditor::OnTreeViewCustomContextMenuRequested( const QPoint & pos )
{
	QModelIndex index = ui->tree->indexAt( ui->tree->viewport()->mapFromGlobal( QCursor::pos() ) );
	
	if ( _Module->isSpacing( index ) || _Module->isFavorites( index ) || _Module->isPackage( index ) )
	{
		return;
	}
	QMenu menu;
	{

		QAction * favorite = new QAction( tr( "favorite" ), &menu );
		connect( favorite, &QAction::triggered, [this, index]() { _Module->addFavorite( index ); } );

		menu.addAction( favorite );
	}
	menu.exec( QCursor::pos() );
}

#include "Skin.h"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QStyleFactory>

XS::Skin::Skin()
	:QProxyStyle( styleBase() )
{
}

XS::Skin::Skin( const QString & path )
	: QProxyStyle( styleBase() )
{
	QFile file( path );
	if ( file.open( QFile::ReadOnly ) )
	{
		QString line;

		while ( !file.atEnd() )
		{
			line = file.readLine();

			if ( line.startsWith( "SkinIcons" ) )
			{
				line.remove( 0, (int)std::strlen( "SkinIcons" ) );
				while ( !line.isEmpty() && ( line[0] == ' ' || line[0] == '\t' ) ) { line.remove( 0, 1 ); }
				if ( line[0] != '=' ) continue; line.remove( 0, 1 );
				while ( !line.isEmpty() && ( line[0] == ' ' || line[0] == '\t' ) ) { line.remove( 0, 1 ); }
				line.remove( line.indexOf( ';' ), line.size() - line.indexOf( ';' ) );

				_ImagePath = line;
				QDir app_dir( QApplication::applicationDirPath() );
				QDir::addSearchPath( "SkinIcons", app_dir.absoluteFilePath( _ImagePath ) );
			}
			else if ( line[0] == '$' )
			{
				line.remove( 0, 1 );

#define CASE( NAME ) \
				if ( line.startsWith( #NAME ) ) \
				{ \
					QPalette::ColorRole role = QPalette::##NAME; \
					QPalette::ColorGroup group = QPalette::All; \
					line.remove( 0, (int)std::strlen( #NAME ) ); \
					if ( line.startsWith( "." ) ) \
					{ \
						line.remove( 0, 1 ); \
						if ( line.startsWith( "Active" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "Active" ) ); \
							group = QPalette::Active; \
						} \
						else if ( line.startsWith( "Disabled" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "Disabled" ) ); \
							group = QPalette::Disabled; \
						} \
						else if ( line.startsWith( "Inactive" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "Inactive" ) ); \
							group = QPalette::Inactive; \
						} \
						else if ( line.startsWith( "NColorGroups" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "NColorGroups" ) ); \
							group = QPalette::NColorGroups; \
						} \
						else if ( line.startsWith( "Current" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "Current" ) ); \
							group = QPalette::Current; \
						} \
						else if ( line.startsWith( "All" ) ) \
						{ \
							line.remove( 0, (int)std::strlen( "All" ) ); \
							group = QPalette::All; \
						} \
					} \
					else \
					{ \
						continue; \
					} \
					while ( !line.isEmpty() && ( line[0] == ' ' || line[0] == '\t' ) ) { line.remove( 0, 1 ); } \
					if ( line[0] != '=' ) continue; line.remove( 0, 1 );\
					while ( !line.isEmpty() && ( line[0] == ' ' || line[0] == '\t' ) ) { line.remove( 0, 1 ); } \
					if ( line[0] != "#" ) continue; line.remove( line.indexOf( ';' ), line.size() - line.indexOf( ';' ) ); \
					_PaletteColors.push_back( std::make_tuple( role, group, QColor( line ) ) ); line.clear(); continue; \
				}

				CASE( PlaceholderText );
				CASE( HighlightedText );
				CASE( AlternateBase );
				CASE( LinkVisited );
				CASE( ToolTipBase );
				CASE( ToolTipText );
				CASE( NColorRoles );
				CASE( WindowText );
				CASE( ButtonText );
				CASE( BrightText );
				CASE( Highlight );
				CASE( Midlight );
				CASE( NoRole );
				CASE( Window );
				CASE( Shadow );
				CASE( Button );
				CASE( Light );
				CASE( Dark );
				CASE( Text );
				CASE( Base );
				CASE( Link );
				CASE( Mid );

#undef CASE
			}
			else
			{
				_QSS.push_back( line );
			}
		}
	}
	file.close();
}

XS::Skin::~Skin()
{

}

QStyle * XS::Skin::baseStyle() const
{
	return styleBase();
}

void XS::Skin::polish( QPalette & palette )
{
	for ( const auto & it : _PaletteColors )
	{
		palette.setColor( std::get<1>( it ), std::get<0>( it ), std::get<2>( it ) );
	}
}

void XS::Skin::polish( QApplication * app )
{
	if ( !app ) return;

	QFont defaultFont = QApplication::font();
	defaultFont.setPointSize( defaultFont.pointSize() + 1 );
	app->setFont( defaultFont );

	app->setStyleSheet( _QSS );
}

QStyle * XS::Skin::styleBase( QStyle * style /*= Q_NULLPTR */ ) const
{
	static QStyle * base = !style ? QStyleFactory::create( "Fusion" ) : style;

	return base;
}

/*!
 * \file	Registry.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/18
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef REGISTRY_H__AAC4D951_0107_4D17_9798_51BA6F904E75
#define REGISTRY_H__AAC4D951_0107_4D17_9798_51BA6F904E75

#include "Type.h"

BEG_XS_NAMESPACE

class XS_API Registry
{
public:
	static void Register( const QMetaObject * meta, const std::function< QObject * ( QObject * ) > & construct );

	static QObject * Construct( const QString & name, QObject * parent );

	template< typename T > static T * ConstructT( const QString & name, QObject * parent )
	{
		return reinterpret_cast<T *>( Construct( name, parent ) );
	}

	static QList< const QMetaObject * > GetDerivedClass( const QMetaObject * super );

private:
	Registry();

	~Registry();

	static Registry * _p();

private:
	QMap< const QMetaObject *, QList< const QMetaObject * > > _Deriveds;
	QMap< QString, std::function< QObject * ( QObject * ) > > _Constructs;
};

template< typename ... T > struct MetaTypeCollector;

END_XS_NAMESPACE


#define REG_OBJECT( TYPE ) \
namespace XS \
{ \
	template<> struct MetaTypeCollector< TYPE > \
	{ \
		MetaTypeCollector() \
		{ \
			XS::Registry::Register( &TYPE::staticMetaObject, []( QObject * parent ) { return new TYPE( parent ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XS::MetaTypeCollector< TYPE > >::Register(); \
		} \
	}; \
};

#define REG_WIDGET( TYPE ) \
namespace XS \
{ \
	template<> struct MetaTypeCollector< TYPE > \
	{ \
		MetaTypeCollector() \
		{ \
			XS::Registry::Register( &TYPE::staticMetaObject, []( QObject * parent ) { return new TYPE( qobject_cast< QWidget * >( parent ) ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XS::MetaTypeCollector< TYPE > >::Register(); \
		} \
	}; \
};

#endif//REGISTRY_H__AAC4D951_0107_4D17_9798_51BA6F904E75

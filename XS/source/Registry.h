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

#include <functional>

#include "Global.h"

BEG_XS_NAMESPACE

class XS_API Registry
{
public:
	static void Register( const QString & name, const std::function< QWidget * ( QWidget * ) > & construct );

	static QWidget * Construct( const QString & name, QWidget * parent );

	template< typename T > static T * ConstructT( const QString & name, QWidget * parent )
	{
		return reinterpret_cast<T *>( Construct( name, parent ) );
	}

private:
	Registry();

	~Registry();

	static Registry * _p();

private:
	std::map< QString, std::function< QWidget * ( QWidget * ) > > _Constructs;
};

END_XS_NAMESPACE


#define REG_WIDGET( TYPE ) \
namespace XE \
{ \
	template<> struct MetaTypeCollector< TYPE > \
	{ \
		MetaTypeCollector() \
		{ \
			XS::Registry::Register( #TYPE, []( QWidget * parent ) { return new TYPE( parent ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XE::MetaTypeCollector< TYPE > >::Register(); \
		} \
	}; \
};

#endif//REGISTRY_H__AAC4D951_0107_4D17_9798_51BA6F904E75

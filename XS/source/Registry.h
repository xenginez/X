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

#define DECLARE_WIDGET( SPACE, TYPE ) \
struct _STATIC_REGISTRY__##SPACE##__##TYPE##_ \
{ \
	using Type = SPACE::TYPE; \
	_STATIC_REGISTRY__##SPACE##__##TYPE##_() \
	{ \
		XS::Registry::Register( #SPACE"::"#TYPE, []( QWidget * parent ) { return new Type( parent ); } ); \
	} \
}; \
static _STATIC_REGISTRY__##SPACE##__##TYPE##_ ___STATIC_REGISTRY__##SPACE##__##TYPE##___ = {};

#define XS_DECLARE_WIDGET( TYPE ) DECLARE_WIDGET( XS, TYPE )

#endif//REGISTRY_H__AAC4D951_0107_4D17_9798_51BA6F904E75

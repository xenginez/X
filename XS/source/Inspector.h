/*!
 * \file	Inspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/24
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231
#define INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231

#include "Widget.h"
#include "ObjectProxy.h"

BEG_XS_NAMESPACE

class XS_API Inspector : public XS::Widget
{
	Q_OBJECT

public:
	Inspector( QWidget * parent = nullptr );

	~Inspector() override;

public:
	static void Register( const XE::String & tname, const QString & name );

	static void Register( const XE::MetaTypeCPtr & type, const QString & name );

	static Inspector * Create( XS::ObjectProxy * proxy, QWidget * parent = nullptr );

public:
	virtual void Expand();

	virtual void Collapse();

	virtual void Refresh() = 0;

protected:
	XS::ObjectProxy * GetObjectProxy();

	void SetObjecrProxy( XS::ObjectProxy * proxy );

protected:
	void SetContentWidget( QWidget * widget );

private:
	XS::ObjectProxy * _Proxy = nullptr;
};

class XS_API GroupInspector : public Inspector
{
	Q_OBJECT

public:
	GroupInspector( QWidget * parent = nullptr );

	~GroupInspector() override;
};

END_XS_NAMESPACE

#define REG_INSPECTOR( TYPE, NAME ) \
namespace XS \
{ \
	template<> struct MetaTypeCollector< TYPE, NAME  > \
	{ \
		MetaTypeCollector() \
		{ \
			XS::Inspector::Register( ::TypeID<TYPE>::Get(), #NAME ); \
			XS::Registry::Register( &NAME::staticMetaObject, []( QObject * parent ) { return new NAME( qobject_cast< QWidget * >( parent ) ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XS::MetaTypeCollector< TYPE, NAME  > >::Register(); \
		} \
	}; \
};

#define REG_INSPECTOR_T( KEY, TYPE ) \
namespace XS \
{ \
	template<> struct MetaTypeCollector< TYPE > \
	{ \
		MetaTypeCollector() \
		{ \
			XS::Inspector::Register( #KEY, #TYPE ); \
			XS::Registry::Register( &TYPE::staticMetaObject, []( QObject * parent ) { return new TYPE( qobject_cast< QWidget * >( parent ) ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XS::MetaTypeCollector< TYPE > >::Register(); \
		} \
	}; \
};

#endif//INSPECTOR_H__D0D642BB_FD51_49D3_BDCA_AF020BFAE231

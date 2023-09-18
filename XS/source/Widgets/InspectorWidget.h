/*!
 * \file	InspectorWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/16
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef INSPECTORWIDGET_H__9496F958_FB75_4B9A_AC7B_9D0E29C4142E
#define INSPECTORWIDGET_H__9496F958_FB75_4B9A_AC7B_9D0E29C4142E

#include "Widget.h"
#include "Core/ObjectProxy.h"

BEG_XS_NAMESPACE

class XS_API InspectorWidget : public XS::Widget
{
	Q_OBJECT

public:
	InspectorWidget( QWidget * parent = nullptr );

	~InspectorWidget() override;

public:
	static void Register( const XE::String & tname, const QString & name );

	static void Register( const XE::MetaTypeCPtr & type, const QString & name );

	static InspectorWidget * Create( XS::ObjectProxy * proxy, QWidget * parent = nullptr );

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

class XS_API GroupInspector : public InspectorWidget
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
			XS::InspectorWidget::Register( ::TypeID<TYPE>::Get(), #NAME ); \
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
			XS::InspectorWidget::Register( #KEY, #TYPE ); \
			XS::Registry::Register( &TYPE::staticMetaObject, []( QObject * parent ) { return new TYPE( qobject_cast< QWidget * >( parent ) ); } ); \
		} \
		static void Use() \
		{ \
			XE::ActiveSingleton< XS::MetaTypeCollector< TYPE > >::Register(); \
		} \
	}; \
};

#endif//INSPECTORWIDGET_H__9496F958_FB75_4B9A_AC7B_9D0E29C4142E
/*!
 * \file	NodeGraphicsItem.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC
#define NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC

#include <QObject>

class NodeGraphicsItem : public QObject
{
	Q_OBJECT

public:
	NodeGraphicsItem( QObject * parent );
	~NodeGraphicsItem();
};

#endif//NODEGRAPHICSITEM_H__9C58B676_3EF5_47A9_A477_0D7FF68996CC

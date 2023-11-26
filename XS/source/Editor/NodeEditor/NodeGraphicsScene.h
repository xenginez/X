/*!
 * \file	NodeGraphicsScene.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSSCENE_H__FCAF4F81_DBBE_4BEB_990A_B78FAD49136D
#define NODEGRAPHICSSCENE_H__FCAF4F81_DBBE_4BEB_990A_B78FAD49136D

#include <QGraphicsScene>

class NodeGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	NodeGraphicsScene( QObject * parent );
	~NodeGraphicsScene();
};

#endif//NODEGRAPHICSSCENE_H__FCAF4F81_DBBE_4BEB_990A_B78FAD49136D

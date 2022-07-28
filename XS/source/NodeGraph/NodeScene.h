/*!
 * \file	NodeScene.h
 *
 * \author	ZhengYuanQing
 * \date	2022/07/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef NODESCENE_H__173EBE3A_0E2A_48F1_A05B_EF242BD52D80
#define NODESCENE_H__173EBE3A_0E2A_48F1_A05B_EF242BD52D80

#include <QVector>
#include <QGraphicsScene>

#include "Core/Global.h"

BEG_XS_NAMESPACE

class NodeItem;
class NodeView;
class NodeScene;
class NodeConnect;

class XS_API NodeScene : public QGraphicsScene
{
	Q_OBJECT

public:
	NodeScene( QWidget * parent = nullptr );

	~NodeScene() override;

public:
	virtual XS::NodeItem * CreateNode( const XE::Variant & val ) = 0;

	virtual void DeleteNode( const XS::NodeItem * val ) = 0;
};

END_XS_NAMESPACE

#endif//NODESCENE_H__173EBE3A_0E2A_48F1_A05B_EF242BD52D80

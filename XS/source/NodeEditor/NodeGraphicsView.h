/*!
 * \file	NodeGraphicsView.h
 *
 * \author	ZhengYuanQing
 * \date	2023/09/14
 * \email	zhengyuanqing.95@gmail.com
 *
*/
#ifndef NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B
#define NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class NodeGraphicsView;
};
QT_END_NAMESPACE

class NodeGraphicsView : public QWidget
{
	Q_OBJECT

public:
	NodeGraphicsView( QWidget * parent = nullptr );
	~NodeGraphicsView();

private:
	Ui::NodeGraphicsView * ui;
};

#endif//NODEGRAPHICSVIEW_H__0BDEDAEB_3D81_4485_87E7_AC940E007D1B

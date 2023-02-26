/*!
 * \file	ClassInspector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/03/27
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef CLASSINSPECTOR_H__ECFFC5F1_ABF5_4B24_85D1_BDC4A2667E93
#define CLASSINSPECTOR_H__ECFFC5F1_ABF5_4B24_85D1_BDC4A2667E93

#include <QTreeWidget>

#include "Core/Inspector.h"

BEG_XS_NAMESPACE

class XS_API ClassInspector : public XS::Inspector
{
	Q_OBJECT

public:
	ClassInspector( QWidget * parent /* = nullptr */ );

	~ClassInspector() override;

public:
	void Expand() override;

	void Collapse() override;

	void Refresh() override;

private:
	void Merge( QTreeWidget * parent, QTreeWidgetItem * parent_item, QTreeWidget * child, QTreeWidgetItem * child_item );

private:
	QTreeWidget * _QTreeWidget;
};

END_XS_NAMESPACE

#endif//CLASSINSPECTOR_H__ECFFC5F1_ABF5_4B24_85D1_BDC4A2667E93

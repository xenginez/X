/*!
 * \file	TemplateListItem.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/16
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TEMPLATELISTITEM_H__0A844DA6_0CA1_4340_A367_9CBD61CF8DEA
#define TEMPLATELISTITEM_H__0A844DA6_0CA1_4340_A367_9CBD61CF8DEA

#include "Global.h"

namespace Ui
{
	class TemplateListItem;
}

namespace Launcher
{
	class TemplateListItem : public QWidget
	{
		Q_OBJECT

	public:
		TemplateListItem( QListWidgetItem * item, QWidget * parent = nullptr );

		~TemplateListItem();

	signals:
		void downloadClicked( QListWidgetItem * item );

	protected:
		bool eventFilter( QObject * obj, QEvent * event ) override;

	private:
		QListWidgetItem * item;
		Ui::TemplateListItem * ui;
	};
}

#endif//TEMPLATELISTITEM_H__0A844DA6_0CA1_4340_A367_9CBD61CF8DEA

/*!
 * \file	ProjectListItem.h
 *
 * \author	ZhengYuanQing
 * \date	2022/02/14
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef PROJECTLISTITEM_H__E9DA4301_1CC4_4B2F_AD0D_CB1ABCBC8579
#define PROJECTLISTITEM_H__E9DA4301_1CC4_4B2F_AD0D_CB1ABCBC8579

#include "Global.h"

namespace Ui
{
	class ProjectListItem;
}

namespace Launcher
{
	class ProjectListItem : public QWidget
	{
		Q_OBJECT

	public:
		ProjectListItem( QListWidgetItem * item, QWidget * parent = nullptr );

		~ProjectListItem();

	signals:
		void itemFixed( QListWidgetItem * item );

		void itemUnfixed( QListWidgetItem * item );

	protected:
		bool eventFilter( QObject * obj, QEvent * event ) override;

	private:
		QListWidgetItem * item;
		Ui::ProjectListItem * ui;
	};
}

#endif//PROJECTLISTITEM_H__E9DA4301_1CC4_4B2F_AD0D_CB1ABCBC8579

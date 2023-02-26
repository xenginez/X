/*!
 * \file	TypeSelector.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/21
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPESELECTOR_H__659D3185_C18B_4DE1_A614_2EB9DF4FD3BB
#define TYPESELECTOR_H__659D3185_C18B_4DE1_A614_2EB9DF4FD3BB

#include "Global.h"

UI_DECL( TypeSelectorDialog );

class QTreeWidgetItem;

BEG_XS_NAMESPACE

class XS_API TypeSelectorDialog : public QDialog
{
	Q_OBJECT

public:
	enum Type
	{
		ENUM = 1 << 0,
		CLASS = 1 << 1,
		ALL = ENUM | CLASS,
	};

public:
	TypeSelectorDialog( Type type = Type::ALL, QWidget * parent = nullptr );

	~TypeSelectorDialog() override;

public:
	const XE::MetaTypeCPtr & GetSelectType() const;

private slots:
	void OnYesPushButtonClicked();

	void OnCancelPushButtonClicked();

	void OnSearchTextChanged( const QString & text );

	void OnTreeViewItemClicked( QTreeWidgetItem * item, int column );

	void OnTreeViewItemDoubleClicked( QTreeWidgetItem * item, int column );

private:
	void FillTree();

private:
	Type _Type;
	XE::MetaTypeCPtr _MetaType;
	Ui::TypeSelectorDialog * ui;
};

END_XS_NAMESPACE

#endif//TYPESELECTOR_H__659D3185_C18B_4DE1_A614_2EB9DF4FD3BB

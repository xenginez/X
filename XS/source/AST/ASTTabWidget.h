/*!
 * \file	ASTTabWidget.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/13
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F
#define ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F

#include "Widget.h"
#include "Inspector.h"
#include "NodeWidget.h"

class QListWidgetItem;
class QTreeWidgetItem;

UI_DECL( ASTTabWidget );

BEG_XS_NAMESPACE

class XS_API ASTTabWidget : public XS::Widget
{
	Q_OBJECT

public:
	ASTTabWidget( QWidget * parent = nullptr );

	~ASTTabWidget() override;

private slots:
	void OnAddToolButtonClicked( bool clicked );

	void OnToolToolButtonClicked( bool clicked );

	void OnEnumListItemChanged( QListWidgetItem * item );

	void OnEnumListItemDoubleClicked( QListWidgetItem * item );

protected:
	void showEvent( QShowEvent * event ) override;

protected:
	void OnRedo() override;

	void OnUndo() override;

	void OnSave() override;

private:
	void Rename();

private:
	void FillTree();
	void FillEnum( const XE::ASTEnumPtr & ast, QTreeWidgetItem * parent );
	void FillClass( const XE::ASTClassPtr & ast, QTreeWidgetItem * parent );
	void FillModule( const XE::ASTModulePtr & ast, QTreeWidgetItem * parent );
	void FillMethod( const XE::ASTMethodPtr & ast, QTreeWidgetItem * parent );
	void FillProperty( const XE::ASTPropertyPtr & ast, QTreeWidgetItem * parent );
	void FillFunction( const XE::ASTFunctionPtr & ast, QTreeWidgetItem * parent );
	void FillVariable( const XE::ASTVariablePtr & ast, QTreeWidgetItem * parent );

private:
	Ui::ASTTabWidget * ui;
	XE::SyntaxTreePtr _Tree;
	XS::Inspector * _Inspector = nullptr;
};

END_XS_NAMESPACE

#endif//ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F

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
	void OnSearchTextChanged( const QString & text );

	void OnTreeWidgetItemChanged( QTreeWidgetItem * item, int col );

	void OnTreeWidgetItemDoubleClicked( QTreeWidgetItem * item, int col );

	void OnTreeWidgetCustomContextMenuRequested( const QPoint & pos );

protected:
	void showEvent( QShowEvent * event ) override;

protected:
	void OnRedo() override;

	void OnUndo() override;

	void OnSave() override;

private:
	void FillTree();
	QTreeWidgetItem * FillEnum( const XE::ASTInfoEnumPtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillClass( const XE::ASTInfoClassPtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillModule( const XE::ASTInfoModulePtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillMethod( const XE::ASTInfoMethodPtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillProperty( const XE::ASTInfoPropertyPtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillFunction( const XE::ASTInfoFunctionPtr & ast, QTreeWidgetItem * parent );
	QTreeWidgetItem * FillVariable( const XE::ASTInfoVariablePtr & ast, QTreeWidgetItem * parent );

private:
	void CreateEnum( QTreeWidgetItem * group );
	void CreateClass( QTreeWidgetItem * group );
	void CreateModule( QTreeWidgetItem * group );
	void CreateMethod( QTreeWidgetItem * group );
	void CreateProperty( QTreeWidgetItem * group );
	void CreateFunction( QTreeWidgetItem * group );
	void CreateVariable( QTreeWidgetItem * group );
	void CreateMethodParameter( QTreeWidgetItem * group );
	void CreateFunctionParameter( QTreeWidgetItem * group );
	void CreateMethodLocalVariable( QTreeWidgetItem * group );
	void CreateFunctionLocalVariable( QTreeWidgetItem * group );

private:
	XE::ASTInfoMetaPtr GetASTParent( QTreeWidgetItem * item, int type );

private:
	Ui::ASTTabWidget * ui;
	XE::ASTInfoModulePtr _Module;
	XS::Inspector * _Inspector = nullptr;
};

END_XS_NAMESPACE

#endif//ASTTABWIDGET_H__11CD18E7_C0B7_40CD_AB6A_5A8AF075920F

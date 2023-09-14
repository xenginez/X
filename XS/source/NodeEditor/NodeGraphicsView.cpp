#include "NodeGraphicsView.h"
#include "ui_NodeGraphicsView.h"

NodeGraphicsView::NodeGraphicsView(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::NodeGraphicsView())
{
	ui->setupUi(this);
}

NodeGraphicsView::~NodeGraphicsView()
{
	delete ui;
}

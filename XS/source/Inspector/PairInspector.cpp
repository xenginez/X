#include "PairInspector.h"

#include "ui_PairInspector.h"

REG_WIDGET( XS::PairInspector );

XS::PairInspector::PairInspector( QWidget * parent /*= nullptr */ )
	: XS::Inspector( parent ), ui( new Ui::PairInspector )
{
	ui->setupUi( this );
}

XS::PairInspector::~PairInspector()
{

}

void XS::PairInspector::Refresh()
{

}

#include "Inspector.h"

#include "BoolInspector.h"
#include "EnumInspector.h"
#include "FlagInspector.h"
#include "ClassInspector.h"
#include "HandleInspector.h"
#include "IntegerInspector.h"
#include "FloatingInspector.h"

XS::Inspector::Inspector( QWidget * parent /*= nullptr */ )
	:Widget( parent )
{

}

XS::Inspector::~Inspector()
{

}

XS::Inspector * XS::Inspector::Create( const XE::MetaTypeCPtr & type, QWidget * parent /*= nullptr */ )
{
	QString inspector_name;

	if (inspector_name.isEmpty())
	{
		// TODO: 根据variant的类型选择Inspector
	}

	auto inspector = Registry::ConstructT< Inspector >( inspector_name, parent );

	return inspector;
}

const XE::Variant & XS::Inspector::GetVariant() const
{
	return _Variant;
}

void XS::Inspector::SetVariant( const XE::Variant & var )
{
	_Variant = var;

	Refresh();
}

void XS::Inspector::Refresh()
{

}

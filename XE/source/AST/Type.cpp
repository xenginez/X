#include "Type.h"

#include "ASTNode.h"

BEG_META( XE::ASTMeta )
type->Property( "Name", &XE::ASTMeta::Name );
type->Property( "Owner", &XE::ASTMeta::Owner )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Module", &XE::ASTMeta::Module )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTEnum )
type->Property( "Flag", &XE::ASTEnum::Flag );
type->Property( "Elements", &XE::ASTEnum::Elements );
END_META()

BEG_META( XE::ASTClass )
type->Property( "Super", &XE::ASTClass::Super );
type->Property( "Destruct", &XE::ASTClass::Destruct )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Construct", &XE::ASTClass::Construct )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Methods", &XE::ASTClass::Methods )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Propertys", &XE::ASTClass::Propertys )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTModule )
type->Property( "Enums", &XE::ASTModule::Enums )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Clsses", &XE::ASTModule::Clsses )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Modules", &XE::ASTModule::Modules )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Functions", &XE::ASTModule::Functions )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Variables", &XE::ASTModule::Variables )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTMethod )
type->Property( "Result", &XE::ASTMethod::Result )->Attribute( XE::NonInspectorAttribute() );
type->Property( "StatementBody", &XE::ASTMethod::StatementBody )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Parameters", &XE::ASTMethod::Parameters )->Attribute( XE::NonInspectorAttribute() );
type->Property( "LocalVariables", &XE::ASTMethod::LocalVariables )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTProperty )
type->Property( "Type", &XE::ASTProperty::Type );
type->Property( "Flag", &XE::ASTProperty::Flag );
type->Property( "Offset", &XE::ASTProperty::Offset )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTFunction )
type->Property( "Result", &XE::ASTFunction::Result );
type->Property( "StatementBody", &XE::ASTFunction::StatementBody )->Attribute( XE::NonInspectorAttribute() );
type->Property( "Parameters", &XE::ASTFunction::Parameters )->Attribute( XE::NonInspectorAttribute() );
type->Property( "LocalVariables", &XE::ASTFunction::LocalVariables )->Attribute( XE::NonInspectorAttribute() );
END_META()

BEG_META( XE::ASTVariable )
type->Property( "Type", &XE::ASTVariable::Type );
type->Property( "Flag", &XE::ASTVariable::Flag );
END_META()

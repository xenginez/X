#include "ASTInfo.h"

#include "ASTNode.h"

BEG_META( XE::ASTInfoMeta )
type->Property( "Name", &XE::ASTInfoMeta::Name );
END_META()

BEG_META( XE::ASTInfoType )
END_META()

BEG_META( XE::ASTInfoEnum )
type->Property( "Flag", &XE::ASTInfoEnum::Flag );
type->Property( "Elements", &XE::ASTInfoEnum::Elements );
END_META()

BEG_META( XE::ASTInfoClass )
type->Property( "SuperClass", &XE::ASTInfoClass::SuperClass );
type->Property( "Destruct", &XE::ASTInfoClass::Destruct );
type->Property( "Construct", &XE::ASTInfoClass::Construct );
type->Property( "Methods", &XE::ASTInfoClass::Methods );
type->Property( "Propertys", &XE::ASTInfoClass::Propertys );
END_META()

BEG_META( XE::ASTInfoModule )
type->Property( "Enums", &XE::ASTInfoModule::Enums );
type->Property( "Classes", &XE::ASTInfoModule::Classes );
type->Property( "Modules", &XE::ASTInfoModule::Modules );
type->Property( "Functions", &XE::ASTInfoModule::Functions );
type->Property( "Variables", &XE::ASTInfoModule::Variables );
END_META()

BEG_META( XE::ASTInfoMethod )
type->Property( "Result", &XE::ASTInfoMethod::Result );
type->Property( "StatementBody", &XE::ASTInfoMethod::StatementBody );
type->Property( "Parameters", &XE::ASTInfoMethod::Parameters );
type->Property( "LocalVariables", &XE::ASTInfoMethod::LocalVariables );
END_META()

BEG_META( XE::ASTInfoProperty )
type->Property( "Type", &XE::ASTInfoProperty::Type );
type->Property( "Flag", &XE::ASTInfoProperty::Flag );
type->Property( "Offset", &XE::ASTInfoProperty::Offset );
END_META()

BEG_META( XE::ASTInfoFunction )
type->Property( "Result", &XE::ASTInfoFunction::Result );
type->Property( "StatementBody", &XE::ASTInfoFunction::StatementBody );
type->Property( "Parameters", &XE::ASTInfoFunction::Parameters );
type->Property( "LocalVariables", &XE::ASTInfoFunction::LocalVariables );
END_META()

BEG_META( XE::ASTInfoVariable )
type->Property( "Type", &XE::ASTInfoVariable::Type );
type->Property( "Flag", &XE::ASTInfoVariable::Flag );
END_META()


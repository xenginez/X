#include "SyntaxTree.h"

BEG_META( XE::SyntaxTree )
type->Property( "Name", &XE::SyntaxTree::_Name );
type->Property( "Enums", &XE::SyntaxTree::_Enums );
type->Property( "Classes", &XE::SyntaxTree::_Classes );
type->Property( "Modules", &XE::SyntaxTree::_Modules );
type->Property( "Functions", &XE::SyntaxTree::_Functions );
type->Property( "Variables", &XE::SyntaxTree::_Variables );
END_META()

XE::SyntaxTree::SyntaxTree()
{

}

XE::SyntaxTree::~SyntaxTree()
{

}

const XE::String & XE::SyntaxTree::GetName() const
{
	return _Name;
}

void XE::SyntaxTree::SetName( const XE::String & val )
{
	_Name = val;
}

const XE::Array<XE::ASTEnumPtr> & XE::SyntaxTree::GetEnums() const
{
	return _Enums;
}

void XE::SyntaxTree::SetEnums( const XE::Array<XE::ASTEnumPtr> & val )
{
	_Enums = val;
}

const XE::Array<XE::ASTClassPtr> & XE::SyntaxTree::GetClasses() const
{
	return _Classes;
}

void XE::SyntaxTree::SetClasses( const XE::Array<XE::ASTClassPtr> & val )
{
	_Classes = val;
}

const XE::Array<XE::ASTModulePtr> & XE::SyntaxTree::GetModules() const
{
	return _Modules;
}

void XE::SyntaxTree::SetModules( const XE::Array<XE::ASTModulePtr> & val )
{
	_Modules = val;
}

const XE::Array<XE::ASTFunctionPtr> & XE::SyntaxTree::GetFunctions() const
{
	return _Functions;
}

void XE::SyntaxTree::SetFunctions( const XE::Array<XE::ASTFunctionPtr> & val )
{
	_Functions = val;
}

const XE::Array<XE::ASTVariablePtr> & XE::SyntaxTree::GetVariables() const
{
	return _Variables;
}

void XE::SyntaxTree::SetVariables( const XE::Array<XE::ASTVariablePtr> & val )
{
	_Variables = val;
}

void XE::SyntaxTree::AddEnum( const XE::ASTEnumPtr & val )
{
	_Enums.push_back( val );
}

void XE::SyntaxTree::AddClass( const XE::ASTClassPtr & val )
{
	_Classes.push_back( val );
}

void XE::SyntaxTree::AddModule( const XE::ASTModulePtr & val )
{
	_Modules.push_back( val );
}

void XE::SyntaxTree::AddFunction( const XE::ASTFunctionPtr & val )
{
	_Functions.push_back( val );
}

void XE::SyntaxTree::AddVariable( const XE::ASTVariablePtr & val )
{
	_Variables.push_back( val );
}

#include "ASTExecutor.h"

struct XE::ASTExecutor::Private
{

};

XE::ASTExecutor * XE::ASTExecutor::GetInstance()
{
	thread_local XE::ASTExecutor exec;
	return &exec;
}

XE::ASTExecutor::ASTExecutor()
	:_p( XE::New< Private >() )
{

}

XE::ASTExecutor::~ASTExecutor()
{
	XE::Delete( _p );
}

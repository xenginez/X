#include "Model.h"

IMPLEMENT_META( XE::Model );

XE::Model::Model()
{

}

XE::Model::~Model()
{

}

XE::CanvasPtr XE::Model::GetCanvas() const
{
	return _Canvas.lock();
}

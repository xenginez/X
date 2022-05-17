/*!
 * \file   Type.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef TYPE_H__30CAD44D_274D_4F6D_BF9B_E9A055B63588
#define TYPE_H__30CAD44D_274D_4F6D_BF9B_E9A055B63588

#include "Declare.h"
#include "Utils/Handle.hpp"

BEG_XE_NAMESPACE

DECL_HANDLE( Entity );

DECL_PTR( Level );
DECL_PTR( GameObject );
DECL_PTR( EntitySystem );
DECL_PTR( GameObjectComponent );

END_XE_NAMESPACE

#endif // TYPE_H__30CAD44D_274D_4F6D_BF9B_E9A055B63588

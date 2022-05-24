/*!
* \file   Declare.h
*
* \author ZhengYuanQing
* \date   2022/05/23
* \email  zhengyuanqing.95@gmail.com
*
*/
#ifndef DECLARE_H__D352D678_3D70_49B7_8523_114ECDB903E1
#define DECLARE_H__D352D678_3D70_49B7_8523_114ECDB903E1

#include <XE/XE.h>

#define BEG_XV_NAMESPACE namespace XV{
#define END_XV_NAMESPACE }

#ifdef XV_EXPORT
#define XV_API DLL_EXPORT
#else
#define XV_API DLL_IMPORT
#endif // XS_EXPORT

#define DECL_XV_ENUM( TYPE ) DECL_META_ENUM( XV, TYPE )
#define DECL_XV_CLASS( TYPE ) DECL_META_CLASS( XV, TYPE )

namespace XV
{
	IMPLEMENT_META_MODULE( XV );
}

#endif//DECLARE_H__D352D678_3D70_49B7_8523_114ECDB903E1
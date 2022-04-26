/*!
 * \file   Entity.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0
#define ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API Entity final
{
public:
	bool Enable = true;
	EntityHandle Handle;
};
DECL_XE_CLASS( Entity );

END_XE_NAMESPACE

#endif // ENTITY_H__62F64232_7901_46A5_BDAC_E69B8F30FAB0

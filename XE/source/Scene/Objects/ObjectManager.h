/*!
 * \file   ObjectManager.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/25
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef OBJECTMANAGER_H__CF40E949_9473_4209_8830_06F3BE740CF1
#define OBJECTMANAGER_H__CF40E949_9473_4209_8830_06F3BE740CF1

#include "Type.h"
#include "Core/Object.h"

BEG_XE_NAMESPACE

class XE_API ObjectManager : public XE::Object
{
	OBJECT( ObjectManager, XE::Object )

public:
	ObjectManager();

	~ObjectManager() override;

};

END_XE_NAMESPACE

#endif // OBJECTMANAGER_H__CF40E949_9473_4209_8830_06F3BE740CF1

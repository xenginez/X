/*!
 * \file   EntityManager.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/25
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef ENTITYMANAGER_H__8726EB96_DFE9_4474_BDCE_D12E10331999
#define ENTITYMANAGER_H__8726EB96_DFE9_4474_BDCE_D12E10331999

#include "Type.h"
#include "Core/Object.h"

BEG_XE_NAMESPACE

class XE_API EntityManager : public XE::Object
{
	OBJECT( EntityManager, XE::Object )

public:
	EntityManager();

	~EntityManager() override;

};

END_XE_NAMESPACE

#endif // ENTITYMANAGER_H__8726EB96_DFE9_4474_BDCE_D12E10331999

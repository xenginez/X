/*!
 * \file   World.h
 *
 * \author ZhengYuanQing
 * \date   2022/04/24
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D
#define WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D

#include "Type.h"
#include "Core/Object.h"

BEG_XE_NAMESPACE

class XE_API World : public XE::Object
{
	OBJECT( World, XE::Object )

public:
	World();

	~World() override;

public:

private:
	XE::String _Name;
	XE::LevelManager _LevelManager;
};

END_XE_NAMESPACE

#endif // WORLD_H__224B6B99_EA71_429B_BF6E_AB949AB38C2D

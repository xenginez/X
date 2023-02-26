/*!
 * \file	RoomService.h
 *
 * \author	ZhengYuanQing
 * \date	2023/02/05
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef ROOMSERVICE_H__95D1BFF0_9C4B_4BF6_BE5C_2048C059F264
#define ROOMSERVICE_H__95D1BFF0_9C4B_4BF6_BE5C_2048C059F264

#include "Type.h"

BEG_XV_NAMESPACE

class RoomService : public XE::Service
{
	OBJECT( RoomService, XE::Service )

public:
	RoomService();

	~RoomService() override;

public:

};

END_XV_NAMESPACE

#endif//ROOMSERVICE_H__95D1BFF0_9C4B_4BF6_BE5C_2048C059F264

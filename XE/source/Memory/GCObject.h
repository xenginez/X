/*!
 * \file	GCObject.h
 *
 * \author	ZhengYuanQing
 * \date	2022/08/15
 * \email	zhengyuanqing.95@gmail.com
 *
 */
#ifndef GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4
#define GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4

#include "Type.h"

BEG_XE_NAMESPACE

class XE_API GCObject
{
private:
	friend class XE::GCMemoryResource;

public:
	GCObject();

	virtual ~GCObject();

protected:
	virtual void Mark();
};

class XE_API GCRootObject : public GCObject
{
public:
	GCRootObject();

	~GCRootObject() override;
};

END_XE_NAMESPACE

#endif//GCOBJECT_H__358986EA_20A0_43CF_975E_6D0BCBEF1AB4

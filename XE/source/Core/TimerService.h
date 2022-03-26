/*!
 * \file   TimerService.h
 *
 * \author ZhengYuanQing
 * \date   2019/03/05
 * \email  zhengyuanqing.95@gmail.com
 *
 */
#ifndef __TIMERSERVICE_H__FF5BCF4F_FC59_4972_B484_868299AAFEC5
#define __TIMERSERVICE_H__FF5BCF4F_FC59_4972_B484_868299AAFEC5

#include "Service.h"

BEG_XE_NAMESPACE

class XE_API TimerService : public XE::Service
{
	OBJECT( TimerService, Service )

private:
	struct Private;

public:
	TimerService();

	~TimerService() override;

public:
	void Prepare() override;

	bool Startup() override;

	void Update() override;

	void Clearup() override;

public:
	virtual void Pause();

	virtual void Continue();

	virtual bool IsPause() const;

public:
	virtual XE::uint64 GetFrameCount() const;

public:
	virtual XE::float32 GetTimeScale() const;

	virtual void SetTimeScale( XE::float32 val );

	virtual XE::float32 GetMinDeltaTime() const;

	virtual void SetMinDeltaTime( XE::float32 val );

	virtual XE::float32 GetMaxDeltaTime() const;

	virtual void SetMaxDeltaTime( XE::float32 val );

public:
	virtual XE::float32 GetTime() const;

	virtual XE::float32 GetDeltaTime() const;

	virtual XE::float32 GetFixedDeltaTime() const;

	virtual XE::float32 GetUnscaleDeltaTime() const;

	virtual XE::float32 GetUnscaleFixedDeltaTime() const;

public:
	virtual XE::Disposable StartTimer( const std::chrono::high_resolution_clock::duration & duration, const XE::Delegate<bool()> & callback );

	virtual XE::Disposable StartUnscaleTimer( const std::chrono::high_resolution_clock::duration & duration, const XE::Delegate<bool()> & callback );

private:
	Private * _p;
};

END_XE_NAMESPACE

#endif // __TIMERSERVICE_H__FF5BCF4F_FC59_4972_B484_868299AAFEC5

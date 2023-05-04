#ifndef __RECORDLOCKER_H__
#define __RECORDLOCKER_H__

#include "LockAttempt.h"
#include "../context/SeasonSession.h"
#include "../context/Observerable.h"
#include "../runtime/UniqueIds.h"
#include <vector>

class RecordLockerObserver
{
public:
	virtual void OnLockAttempt(const LockAttempt* attempt) = 0;
};


class RecordLocker : public Observerable<RecordLockerObserver>
{
public:
	RecordLocker();
	~RecordLocker();
	
	virtual void CreateAttempts(const UniqueIds& invoiceIds) = 0;
	virtual void AttemptLockAt(uint32_t index);
	virtual void UnlockAll();
	
	size_t Count() const;
	const LockAttempt* GetAttemptAt(uint32_t index) const;
protected:
	virtual void Lock(uint32_t id) = 0;
	virtual void Unlock(uint32_t id) = 0;
	
	void NotifyAttempt(const LockAttempt*);	
protected:
	std::vector<LockAttempt*> m_attempts;	
};

#endif
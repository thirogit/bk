#include "stdafx.h"
#include "RecordLocker.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>

RecordLocker::RecordLocker() 
{
}

RecordLocker::~RecordLocker()
{
	DeepDelete(m_attempts);
}

	
size_t RecordLocker::Count() const
{
	return m_attempts.size();
}

const LockAttempt* RecordLocker::GetAttemptAt(uint32_t index) const
{
	return m_attempts[index];
}

void RecordLocker::AttemptLockAt(uint32_t index)
{
	LockAttempt* attempt = m_attempts[index];

	if(attempt->GetStatus() == LockAttemptStatus_Locked)
		return;

	try
	{
		attempt->SetStatus(LockAttemptStatus_InProgress);
		NotifyAttempt(attempt);
		Lock(attempt->GetId());
		attempt->SetStatus(LockAttemptStatus_Locked);
		NotifyAttempt(attempt);
	}
	catch(wexception& e)
	{
		attempt->SetErrorMsg(e.message());
		attempt->SetStatus(LockAttemptStatus_Failed);
		NotifyAttempt(attempt);
	}
}

void RecordLocker::UnlockAll()
{
	BOOST_FOREACH(LockAttempt* attempt,m_attempts)
	{
		if(attempt->GetStatus() == LockAttemptStatus_Locked)
		{
			Unlock(attempt->GetId());
			attempt->SetStatus(LockAttemptStatus_Unlocked);
		}
	}}

void RecordLocker::NotifyAttempt(const LockAttempt* attempt)
{
	VisitObservers(&RecordLockerObserver::OnLockAttempt,attempt);
}

#ifndef __LOCKATTEMPT_H__
#define __LOCKATTEMPT_H__

#include <stdint.h>
#include <string>

enum LockAttemptStatus
{
	LockAttemptStatus_NoAttempt,
	LockAttemptStatus_Failed,
	LockAttemptStatus_Locked,
	LockAttemptStatus_Unlocked,
	LockAttemptStatus_InProgress
};

class LockAttempt
{
public:
	LockAttempt(uint32_t id);
	
	uint32_t GetId() const;
	const std::wstring& GetErrorMsg() const;
	const std::wstring& GetDescriptor() const;
	LockAttemptStatus GetStatus() const;	

	void SetErrorMsg(const std::wstring& erroMsg);
	void SetDescriptor(const std::wstring& descriptor);
	void SetStatus(LockAttemptStatus status);	
	
private:
	std::wstring m_descriptor;
	std::wstring m_errorMsg;
	uint32_t m_Id;
	LockAttemptStatus m_status;
};


#endif
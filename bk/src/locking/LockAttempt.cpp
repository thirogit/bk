#include "stdafx.h"
#include "LockAttempt.h"

LockAttempt::LockAttempt(uint32_t id) : 
	m_status(LockAttemptStatus_NoAttempt),m_Id(id)
{
}

uint32_t LockAttempt::GetId() const
{
	return m_Id;
}

const std::wstring& LockAttempt::GetErrorMsg() const
{
	return m_errorMsg;
}

const std::wstring& LockAttempt::GetDescriptor() const
{
	return m_descriptor;
}

LockAttemptStatus LockAttempt::GetStatus() const
{
	return m_status;
}

void LockAttempt::SetErrorMsg(const std::wstring& errorMsg)
{
	m_errorMsg = errorMsg;
}

void LockAttempt::SetDescriptor(const std::wstring& descriptor)
{
	m_descriptor = descriptor;
}

void LockAttempt::SetStatus(LockAttemptStatus status)
{
	m_status = status;
}


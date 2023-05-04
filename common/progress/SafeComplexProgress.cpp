#include "stdafx.h"
#include "SafeComplexProgress.h"

SafeComplexProgress::SafeComplexProgress(ComplexProgress* pProgresDelegate) : m_pProgresDelegate(pProgresDelegate)
{
}

void SafeComplexProgress::SetTaskCount(int taskCount)
{
	if(m_pProgresDelegate)
		m_pProgresDelegate->SetTaskCount(taskCount);
}

SimpleProgress* SafeComplexProgress::BeginTask(const std::wstring& sTaskName)
{
	if(m_pProgresDelegate)
		return m_pProgresDelegate->BeginTask(sTaskName);

	return NULL;
}

void SafeComplexProgress::EndTask()
{
	if(m_pProgresDelegate)
		m_pProgresDelegate->EndTask();
}

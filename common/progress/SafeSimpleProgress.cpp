#include "stdafx.h"
#include "SafeSimpleProgress.h"

SafeSimpleProgress::SafeSimpleProgress(SimpleProgress* pProgress) : m_pProgress(pProgress)
{
}

SafeSimpleProgress::SafeSimpleProgress(const SafeSimpleProgress& src)
{
	operator=(src);
}

SafeSimpleProgress& SafeSimpleProgress::operator=(const SafeSimpleProgress& right)
{
	m_pProgress = right.m_pProgress;
	return *this;
}

void SafeSimpleProgress::SetSteps(int maxSteps)
{
	if(m_pProgress)
		m_pProgress->SetSteps(maxSteps);
}

bool SafeSimpleProgress::Step(int stepsToMake/* = 1*/)
{
	if(m_pProgress)
		return m_pProgress->Step(stepsToMake);

	return true;
}
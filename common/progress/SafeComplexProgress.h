#ifndef __SAFECOMPLEXPROGRESS_H__
#define __SAFECOMPLEXPROGRESS_H__

#include "ComplexProgress.h"

class SafeComplexProgress : public ComplexProgress
{
public:	
	SafeComplexProgress(ComplexProgress* pProgresDelegate);
	virtual void SetTaskCount(int taskCount);
	virtual SimpleProgress* BeginTask(const std::wstring& sTaskName);
	virtual void EndTask();
private:
	ComplexProgress* m_pProgresDelegate;
	
		
};

#endif
#ifndef __COMPLEXPROGRESS_H__
#define __COMPLEXPROGRESS_H__

#include "SimpleProgress.h"
#include <string>
class ComplexProgress
{
public:	
	virtual void SetTaskCount(int taskCount) = 0;
	virtual SimpleProgress* BeginTask(const std::wstring& sTaskName) = 0;
	virtual void EndTask() = 0;
		
};

#endif
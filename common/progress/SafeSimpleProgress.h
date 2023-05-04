#ifndef __SAFESIMPLEPROGRESS_H__
#define __SAFESIMPLEPROGRESS_H__

#include "SimpleProgress.h"

class SafeSimpleProgress : public SimpleProgress
{
public:	
	SafeSimpleProgress(SimpleProgress* pProgress);
	SafeSimpleProgress(const SafeSimpleProgress& src);
	SafeSimpleProgress& operator=(const SafeSimpleProgress& right);
	virtual void SetSteps(int maxSteps);
	virtual bool Step(int stepsToMake = 1);
private:
	SimpleProgress* m_pProgress;
};

#endif
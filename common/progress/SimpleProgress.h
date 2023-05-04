#ifndef __SIMPLEPROGRESS_H__
#define __SIMPLEPROGRESS_H__


class SimpleProgress
{
public:	
	virtual void SetSteps(int maxSteps) = 0;
	virtual bool Step(int stepsToMake = 1) = 0;
};

#endif
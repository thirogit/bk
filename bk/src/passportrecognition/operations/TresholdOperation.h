#ifndef __TRESHOLDOPERATION_H__
#define __TRESHOLDOPERATION_H__

#include "PassportImageOperation.h"

class TresholdOperation : public PassportImageOperation
{
public:
	TresholdOperation(uint8_t level);
	virtual void DoOperation(PassportImageObject* passportImgObj);
private:
	uint8_t m_level;
	
};


#endif
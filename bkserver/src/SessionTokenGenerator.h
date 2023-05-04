#ifndef __SESIONTOKENGENERATOR_H__
#define __SESIONTOKENGENERATOR_H__

class SessionTokenGenerator
{
public:
	virtual uint32_t ObtainNextToken() = 0;
	
};


#endif
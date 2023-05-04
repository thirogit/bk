#ifndef __NUMBERTOTEXT_H__
#define __NUMBERTOTEXT_H__

#include <stdint.h>
#include <string>

class NumberToText
{
public:
	std::wstring Translate(uint32_t number);	
private:
	std::wstring Translate999(uint32_t number);
	int DeclinationIndex(uint32_t number);
	
};



#endif

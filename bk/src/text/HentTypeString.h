#ifndef __HENTTYPESTRING_H__
#define __HENTTYPESTRING_H__

#include <data/HentType.h>
#include <string>

class HentTypeString : private std::wstring
{
public:	
	HentTypeString(HentType hentType);
	const std::wstring& ToString();
};

#endif
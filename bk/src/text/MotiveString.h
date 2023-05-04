#ifndef __MOTIVESTRING_H__
#define __MOTIVESTRING_H__

#include <data/Motive.h>
#include <string>

class MotiveString : private std::wstring
{
public:	
	MotiveString(Motive motive);
	const std::wstring& ToString();
};

#endif
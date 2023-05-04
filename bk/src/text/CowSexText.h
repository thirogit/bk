#ifndef __COWSEXTEXT_H__
#define __COWSEXTEXT_H__

#include <string>
#include <data\types\CowSex.h>

class CowSexText : private std::wstring
{
public:	
	CowSexText(const CowSex& sex);
	const std::wstring& ToString() const;
};

#endif
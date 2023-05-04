#ifndef __YESNOTEXT_H__
#define __YESNOTEXT_H__

#include <string>

class YesNoText : private std::wstring
{
public:	
	YesNoText(bool bYesNo);
	const std::wstring& ToString();
};

#endif
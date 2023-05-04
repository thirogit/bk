#ifndef __DOCTYPES_H__
#define __DOCTYPES_H__

#include <string>
#include "../DocType.h"

namespace DocTypes
{
	DocType GetType(const std::wstring& sType);
	DocType GetType(char cType);	
	std::wstring ToString(DocType type);
	DocType FromString(const std::wstring& type);
};

#endif
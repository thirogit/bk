#ifndef __PAYWAYS_H__
#define __PAYWAYS_H__

#include <string>
#include "../PayWay.h"

namespace PayWays
{
	PayWay FromString(const std::wstring& sPayWay);
	PayWay FromChar(char cPayWay);
	std::wstring ToString(PayWay payway);
	
};

#endif
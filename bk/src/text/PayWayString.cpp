#include "stdafx.h"
#include "PayWayString.h"
#include "../utils/StringLoader.h"
#include "../resource.h"


PayWayString::PayWayString(PayWay payWay)
{
	switch(payWay)
	{
		case PayWay_Cash:
			std::wstring::operator=(StringLoader(IDS_PAYWAY_CASH));
			break;
		case PayWay_Transfer:
			std::wstring::operator=(StringLoader(IDS_PAYWAY_TRANSFER));
			break;
	}
}

const std::wstring& PayWayString::ToString()
{
	return *this;  
}


	

	



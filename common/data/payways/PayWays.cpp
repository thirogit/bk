#include "stdafx.h"
#include "PayWays.h"
#include <boost/algorithm/string.hpp>

namespace PayWays
{

	PayWay FromChar(char cPayWay)
	{
		if (PayWay_Cash == (PayWay)cPayWay)
			return PayWay_Cash;

		if (PayWay_Transfer == (PayWay)cPayWay)
			return PayWay_Transfer;

		return PayWay_None;
	}

	static const wchar_t* payway_cash = L"cash";
	static const wchar_t* payway_transfer = L"transfer";



	PayWay FromString(const std::wstring& sPayWay)
	{
		if (sPayWay.length() == 1)
		{
			return FromChar(sPayWay[0]);
		}
		else
		{
			if (boost::iequals(payway_cash, sPayWay))
				return PayWay_Cash;

			if (boost::iequals(payway_transfer, sPayWay))
				return PayWay_Transfer;
		}

		return PayWay_None;
	}

	std::wstring ToString(PayWay payway)
	{
		switch (payway)
		{
		case PayWay_Cash:
			return payway_cash;
		case PayWay_Transfer:
			return payway_transfer;
		}

		return L"";

	}
}
	
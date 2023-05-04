#include "stdafx.h"
#include "PassportMetricUtils.h"
#include <configuration/passportrecognition/PassportFieldMetrics.h>
#include <boost/format.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace boost::xpressive;

namespace PassportMetricUtils
{
	std::wstring FormatPassportMetric(int metric)
	{
		int cm = metric/100;
		int mm = metric%100;

		return boost::str(boost::wformat(L"%d.%02d") % cm % mm);
	}

	int ParsePassportMetric(const std::wstring& sMetric)
	{
		wsregex rex =  (+_d) >> *('.' >> _d >> _d);
		wsmatch what;

		if (regex_match(sMetric, what, rex))
		{
			int size = what.size();
			//std::cout << what[1] << '\n'; // first capture
			//std::cout << what[2] << '\n'; // second capture
		}

		
		
		return 0;
	}
}

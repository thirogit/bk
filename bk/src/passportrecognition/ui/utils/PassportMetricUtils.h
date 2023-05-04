#ifndef __PASSPORTMETRICUTILS_H__
#define __PASSPORTMETRICUTILS_H__

#include <string>

namespace PassportMetricUtils 
{
	std::wstring FormatPassportMetric(int metric);
	int ParsePassportMetric(const std::wstring& sMetric);
};


#endif


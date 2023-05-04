#ifndef __EXPANDABLETEMPLATEDATASOURCE_H__
#define __EXPANDABLETEMPLATEDATASOURCE_H__

#include <string>
#include "Values.h"

namespace bkprint
{
	class BandEvaluator
	{
	public:
		virtual Values Header() = 0;
		virtual Values Footer() = 0;
		virtual size_t Count() const = 0;
		virtual Values At(int index) = 0;
	};

	class ExpandableTemplateDataSource
	{
		public:
			virtual Values GetHeaderValues() = 0;
			virtual Values GetFooterValues() = 0;
			virtual Values GetPageHeaderValues(int pageCount, int page) = 0;
			virtual Values GetPageFooterValues(int pageCount, int page) = 0;
			virtual BandEvaluator* GetBandValues(const std::wstring& bandName) = 0;		
	};

	class HyperExpandableTemplateDataSource
	{
	public:
		virtual int GetDSCount() = 0;
		virtual ExpandableTemplateDataSource* GetDS(int index) = 0;
	};
}
#endif
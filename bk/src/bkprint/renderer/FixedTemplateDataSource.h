#ifndef __FIXEDTEMPLATEDATASOURCE_H__
#define __FIXEDTEMPLATEDATASOURCE_H__

#include "Values.h"

namespace bkprint
{
	class FixedTemplateDataSource
	{
		public:
			virtual int GetSheetCount() = 0;
			virtual Values GetValuesForSheet(int sheet) = 0;					
	};

	class HyperFixedTemplateDataSource
	{
	public:
		virtual int GetDSCount() = 0;
		virtual FixedTemplateDataSource* GetDS(int index) = 0;
	};
}
#endif
#include "stdafx.h"
#include "PaperTypeUtil.h"
#include <string\TextUtils.h>
#include <map>

namespace bkprint
{
	namespace PaperTypeUtil
	{

		template<class K,class V>
		V get_default(const std::map<K, V>& m,const K& key,const V& whenNull)
		{
			std::map<K, V>::const_iterator it = m.find(key);
			if (m.end() != it)
			{
				return it->second;
			}

			return whenNull;
		}

		std::map<PaperType, CSize> _paperSizes = { { PaperType_A4, { 2100, 2970 } }, { PaperType_A5, { 1480, 2100 } }, { PaperType_A6, { 1480, 1050 } } };
		std::map<PaperType, std::wstring> _paperTypeNames = { { PaperType_A4, L"A4" }, { PaperType_A5, L"A5" } };

		std::map<std::wstring, PaperType > _paperNames = { { L"A4", PaperType_A4 }, 
														   { L"a4", PaperType_A4 },
														   { L"A5", PaperType_A5 },
														   { L"a5", PaperType_A5 },
														   { L"A6", PaperType_A6 },
														   { L"a6", PaperType_A6 } } ;

		std::wstring ToString(PaperType type)
		{
			return get_default(_paperTypeNames, type, std::wstring());
		}

		PaperType FromString(const std::wstring& type)
		{
			return get_default(_paperNames, type, PaperType_None);
		}

		CSize GetPaperSize(PaperType paperType, PaperOrientation orientation)
		{			
			CSize paperSize = get_default(_paperSizes, paperType, CSize(0, 0));

			if (orientation == PaperOrientation_Landscape)
				paperSize = CSize(paperSize.cy, paperSize.cx);

			return paperSize;
		}


	};
}
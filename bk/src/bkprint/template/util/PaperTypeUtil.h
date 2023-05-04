#if !defined(__PAPERTYPEUTIL_H__)
#define __PAPERTYPEUTIL_H__

#include <string>
#include "../../types/PaperType.h"
#include "../../types/PaperOrientation.h"


namespace bkprint
{
	namespace PaperTypeUtil
	{
		std::wstring ToString(PaperType type);
		PaperType FromString(const std::wstring& type);
		CSize GetPaperSize(PaperType paperType,PaperOrientation orientation);
	};
}

#endif 

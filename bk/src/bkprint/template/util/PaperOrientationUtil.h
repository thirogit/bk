#if !defined(__PAPERORIENTATIONUTIL_H__)
#define __PAPERORIENTATIONUTIL_H__

#include <string>
#include "../../types/PaperOrientation.h"

namespace bkprint
{

	namespace PaperOrientationUtil
	{
		std::wstring ToString(PaperOrientation orientation);
		PaperOrientation FromString(const std::wstring& orinatation);
	};
}

#endif 

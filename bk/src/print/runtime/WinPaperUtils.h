#ifndef __WINPAPERUTILS_H__
#define __WINPAPERUTILS_H__

#include "../../bkprint/types/PaperType.h"
#include "../../bkprint/types/PaperOrientation.h"
#include <wingdi.h>

namespace WinPaperUtils
{
	WORD GetOrientation(bkprint::PaperOrientation orientation);
	WORD GetPaperSize(bkprint::PaperType paperType);
}

#endif
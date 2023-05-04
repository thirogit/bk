#include "stdafx.h"
#include "WinPaperUtils.h"
#include <map>

namespace WinPaperUtils
{
	
	std::map<bkprint::PaperType, short> _winPaperTypes = { 
		{ bkprint::PaperType_A4, DMPAPER_A4 },
		{ bkprint::PaperType_A5, DMPAPER_A5 },
		{ bkprint::PaperType_A6, DMPAPER_A6 },
												};
	
	WORD GetOrientation(bkprint::PaperOrientation orientation)
	{
		switch(orientation)
		{			
		case bkprint::PaperOrientation_Portrait:
				return DMORIENT_PORTRAIT;
		case bkprint::PaperOrientation_Landscape:
				return DMORIENT_LANDSCAPE;			
		}
		return 0;			
	}
		
	WORD GetPaperSize(bkprint::PaperType paperType)
	{
		auto it = _winPaperTypes.find(paperType);
		if (_winPaperTypes.end() != it)
		{
			return it->second;
		}
		return 0;
	}
};

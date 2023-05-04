#include "stdafx.h"
#include "OwnDocViewText.h"

std::wstring OwnDocViewText::Text(const OwnDocView *pView,uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
	case OwnDocViewMember::Herd:
		text = pView->GetHerdCd();
		break;
	default:
		text = DocViewText::Text(pView,memberId);
		break;		
	}
	return text;
}

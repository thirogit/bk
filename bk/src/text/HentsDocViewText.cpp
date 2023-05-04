#include "stdafx.h"
#include "HentsDocViewText.h"

std::wstring HentsDocViewText::Text(const HentsDocView *pView,uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
	
	case HentsDocViewMember::Herd:
		text = pView->GetHerdCd();
		break;
	case HentsDocViewMember::Hent:
		text = pView->GetHentAlias();
		break;
	case HentsDocViewMember::Invoice:
		text = pView->GetInvoiceNo();
		break;	
	default:
		text = DocViewText::Text(pView,memberId);
		break;		
	}
	return text;
}

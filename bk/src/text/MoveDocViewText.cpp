#include "stdafx.h"
#include "MoveDocViewText.h"

std::wstring MoveDocViewText::Text(const MoveDocView *pView,uint32_t memberId)
{
	std::wstring text;
	switch(memberId)
	{
	
	case MoveDocViewMember::SrcHerd:
		text = pView->GetSrcHerdCd();
		break;
	case MoveDocViewMember::DstHerd:
		text = pView->GetDstHerdCd();
		break;	
	default:
		text = DocViewText::Text(pView,memberId);
		break;		
	}
	return text;
}

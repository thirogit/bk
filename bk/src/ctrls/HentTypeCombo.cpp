#include "stdafx.h"
#include "HentTypeCombo.h"
#include "../text/HentTypeString.h"

CHentTypeCombo::CHentTypeCombo() 
{
}

void CHentTypeCombo::ReloadCombo(HentType selection/* = HentType_None*/)
{
	HentType henttypes[] = 
	{
		HentType_Individual,
		HentType_Company,
		HentType_None
	};
	
	ResetContent();	

	int iItem,iDefaultItem = -1;
	HentType* pHentType = henttypes;
	while(*pHentType != HentType_None)
	{
		iItem = AddString(HentTypeString(*pHentType).ToString().c_str());
		if(iItem >= 0)
		{
			SetItemData(iItem,(DWORD_PTR)*pHentType);
			if(*pHentType == selection) 
				iDefaultItem = iItem;
			else if(iItem <= iDefaultItem)
				iDefaultItem++;
		}
		pHentType++;
	}
	SetCurSel(iDefaultItem);
}

HentType CHentTypeCombo::GetSelectedHentType()
{
	int curSel = GetCurSel();
	if(curSel >= 0)
		return (HentType)GetItemData(curSel);

	return HentType_None;
}


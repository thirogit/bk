#include "stdafx.h"
#include "MotiveCombo.h"
#include "../text/MotiveString.h"

CMotiveCombo::CMotiveCombo() 
{
}

void CMotiveCombo::InitCombo(Motive_Direction direction/* = MotiveDirection_None*/)
{
	Motive motives[] = 
	{
		Motive_Birth,
		Motive_Buy,
		Motive_Sell,
		Motive_Import,
		Motive_Export,
		Motive_Death,
		Motive_Kill,
		Motive_Slaugher,
		Motive_None
	};
	
	ResetContent();	

	int iItem;
	Motive* pMotive = motives;
	while(*pMotive != Motive_None)
	{
		if(direction == MotiveDirection_None || direction == Motives::GetMotiveDirection(*pMotive))
		{
			iItem = AddString(MotiveString(*pMotive).ToString().c_str());
			if(iItem >= 0)
			{
				SetItemData(iItem,(DWORD_PTR)*pMotive);				
			}			
		}
		pMotive++;
	}
	SetCurSel(-1);
}

void CMotiveCombo::SelectMotive(Motive motive)
{
	for(int i = 0,count = GetCount();i < count;i++)
	{
		if(motive == (Motive)GetItemData(i))
		{
			SetCurSel(i);
			return;
		}
	}
	SetCurSel(-1);
}

Motive CMotiveCombo::GetSelectedMotive()
{
	int curSel = GetCurSel();
	if(curSel >= 0)
		return (Motive)GetItemData(curSel);

	return Motive_None;
}


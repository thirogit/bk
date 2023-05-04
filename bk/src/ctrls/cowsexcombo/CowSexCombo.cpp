#include "stdafx.h"
#include "CowSexCombo.h"
#include "../../text/CowSexText.h"
#include <list>
#include <boost/foreach.hpp>

void CCowSexCombo::InitCombo(const CowSex& selected/* = SEX_UNK*/)
{
	ResetContent();

	std::list<COWSEX> sexes = { SEX_XX, SEX_XY, SEX_UNK};

	int item, nSel = -1;

	BOOST_FOREACH(COWSEX sex, sexes)
	{
		item = AddString(CowSexText(sex).ToString().c_str());
		SetItemData(item, sex);
		if (selected == sex) nSel = item;
	}
	
	SetCurSel(nSel);

}

CowSex CCowSexCombo::GetSex()
{
	return (COWSEX)GetItemData(GetCurSel());
}

void CCowSexCombo::SelectSex(const CowSex& sex)
{
	for(int i = 0,s = GetCount(); i < s;i++)
	{
		if(sex == (COWSEX)GetItemData(i))
		{
			SetCurSel(i);
			break;
		}
	}
}



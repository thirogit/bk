#include "stdafx.h"
#include "HerdsComboContent.h"

void HerdsComboContent::CreateContent(SeasonSession* facade)
{
	IHerd* pHerd;
	PtrEnumerator<IHerd> herdEnum = facade->EnumHerds();
	while(herdEnum.hasNext())
	{
		pHerd = *herdEnum;
		AddItem(pHerd->GetId(),pHerd->GetHerdName());
		herdEnum.advance();
	}
}

void HerdsComboContent::DestroyContent()
{
	ComboContent::Destroy();
}
#include "stdafx.h"
#include "PayWayCombo.h"
#include "../../text/PayWayString.h"

void CPaywayCombo::ReloadCombo(PayWay selection/* = PayWay_None*/)
{
	ResetContent();
	ModifyStyle(CBS_SORT,0,0);

	PayWay payWays[] =
	{
		PayWay_Cash,
		PayWay_Transfer,
		PayWay_None,
	}; 
	
	ResetContent();	

	int iItem,iDefaultItem = -1;
	PayWay* pPayWay = payWays;
	while(*pPayWay != PayWay_None)
	{
		iItem = AddString(PayWayString(*pPayWay).ToString().c_str());
		if(iItem >= 0)
		{
			SetItemData(iItem,(DWORD_PTR)*pPayWay);
			if(*pPayWay == selection) 
				iDefaultItem = iItem;
			else if(iItem <= iDefaultItem)
				iDefaultItem++;
		}
		pPayWay++;
	}
	SetCurSel(iDefaultItem);

}

	
PayWay CPaywayCombo::GetSelectedPayWay()
{
	int curSel = GetCurSel();
	if(curSel >= 0)
		return (PayWay)GetItemData(curSel);

	return PayWay_None;
}


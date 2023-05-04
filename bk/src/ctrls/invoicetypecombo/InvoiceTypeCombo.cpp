#include "stdafx.h"
#include "InvoiceTypeCombo.h"
#include "../../text/InvoiceTypeString.h"

void CInvoiceTypeCombo::ReloadCombo(InvoiceType type/* = InvoiceType_None*/)
{
	ResetContent();
	ModifyStyle(CBS_SORT,0,0);

	InvoiceType invoiceTypes[] =
	{
		InvoiceType_Regular,
		InvoiceType_Lump,
		InvoiceType_None,
	}; 
	
	ResetContent();	

	int iItem,iDefaultItem;
	InvoiceType* pType = invoiceTypes;
	while(*pType != InvoiceType_None)
	{
		iItem = AddString(InvoiceTypeString(*pType).ToString().c_str());
		if(iItem >= 0)
		{
			SetItemData(iItem,(DWORD_PTR)*pType);
			if(*pType == type) 
				iDefaultItem = iItem;
			else if(iItem <= iDefaultItem)
				iDefaultItem++;
		}
		pType++;
	}
	SetCurSel(iDefaultItem);

}
	 
InvoiceType CInvoiceTypeCombo::GetSelectedType()
{
	int curSel = GetCurSel();
	if(curSel >= 0)
		return (InvoiceType)GetItemData(curSel);

	return InvoiceType_None;
}


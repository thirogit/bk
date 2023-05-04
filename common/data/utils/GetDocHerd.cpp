#include "stdafx.h"
#include "GetDocHerd.h"

#include "../IInDoc.h"
#include "../IBuyDoc.h"
#include "../ISellDoc.h"
#include "../IOutDoc.h"

uint32_t GetDocHerd(const IDoc* pDoc)
{
	switch(pDoc->GetDocType())
	{
	case DocType_In:
		return ((IInDoc*)pDoc)->GetHerdId();
	case DocType_Out:
		return ((IOutDoc*)pDoc)->GetHerdId();
	case DocType_Buy:
		return ((IBuyDoc*)pDoc)->GetHerdId();
	case DocType_Sell:
		return ((ISellDoc*)pDoc)->GetHerdId();
	}
	
	return NULL_ID;
}
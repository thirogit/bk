#include "stdafx.h"
#include "OptionCtrlFactory.h"
#include "DisplacementEventsCtrl.h"
#include "DecimalFormatCtrl.h"
#include "StringOptionCtrl.h"
#include "IntegerOptionCtrl.h"
#include "BooleanOptionCtrl.h"
#include "DecimalOptionCtrl.h"
#include "HentNoOptionCtrl.h"
#include "DayDateOptionCtrl.h"


DocumentOptionCtrlFactory::DocumentOptionCtrlFactory(CWnd* pParent) : m_pParent(pParent)
{

}

DocumentOptionCtrl* DocumentOptionCtrlFactory::CreateCtrl(const DocumentOptionDefinition& definition,UINT nId)
{
	DocumentOptionCtrl* pCtrl = NULL;
	switch (definition.GetType())
	{
	case DocumentOptionType_String:
		pCtrl = new StringOptionCtrl();
		break;
	case DocumentOptionType_Integer:
		pCtrl = new IntegerOptionCtrl();
		break;
	case DocumentOptionType_DisplacementEvents:
		pCtrl = new DisplacementEventsCtrl();
		break;	
	case DocumentOptionType_DecimalFormat:
		pCtrl = new DecimalFormatCtrl();
		break;
	case DocumentOptionType_Boolean:
		pCtrl = new BooleanOptionCtrl();
		break;
	case DocumentOptionType_Decimal:
		pCtrl = new DecimalOptionCtrl();
		break;
	case DocumentOptionType_HentNo:
		pCtrl = new HentNoOptionCtrl();
		break;
	case DocumentOptionType_DayDate:
		pCtrl = new DayDateOptionCtrl();
		break;
	}

	if (pCtrl != NULL)
	{
		pCtrl->Create(definition.GetCode(),m_pParent);		
	}

	return pCtrl;
}
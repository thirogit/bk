#include "stdafx.h"
#include "NewDeductionDetailsDlg.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"


CNewDeductionDetailsDlg::CNewDeductionDetailsDlg(CWnd* pParent/* = NULL*/) : CDeductionDetailsDlg(pParent)
{

}

std::wstring CNewDeductionDetailsDlg::GetCaption() const
{
	return StringLoader(IDS_NEWDEDUCTIONCAPTION);
}
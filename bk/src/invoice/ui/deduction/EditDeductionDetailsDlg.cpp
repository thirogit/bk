#include "stdafx.h"
#include "EditDeductionDetailsDlg.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"

CEditDeductionDetailsDlg::CEditDeductionDetailsDlg(const DeductionDetails& details, CWnd* pParent/* = NULL*/) : CDeductionDetailsDlg(details, pParent)
{

}

std::wstring CEditDeductionDetailsDlg::GetCaption() const
{
	return StringLoader(IDS_EDITDEDUCTIONCAPTION);
}

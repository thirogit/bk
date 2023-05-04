#ifndef __NEWDEDUCTIONDETAILSDLG_H__
#define __NEWDEDUCTIONDETAILSDLG_H__

#include "DeductionDetailsDlg.h"

class CNewDeductionDetailsDlg : public CDeductionDetailsDlg
{	
public:
	CNewDeductionDetailsDlg(CWnd* pParent = NULL);
	
	virtual std::wstring GetCaption() const;

};

#endif
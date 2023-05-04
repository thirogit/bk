#ifndef __EDITDEDUCTIONDETAILSDLG_H__
#define __EDITDEDUCTIONDETAILSDLG_H__

#include "DeductionDetailsDlg.h"

class CEditDeductionDetailsDlg : public CDeductionDetailsDlg
{	
public:
	CEditDeductionDetailsDlg(const DeductionDetails& details, CWnd* pParent = NULL);	
protected:
	virtual std::wstring GetCaption() const;

	
};

#endif
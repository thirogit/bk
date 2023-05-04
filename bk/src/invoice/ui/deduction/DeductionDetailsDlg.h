#ifndef __DEDUCTIONDETAILSDLG_H__
#define __DEDUCTIONDETAILSDLG_H__

#include "../../../resource.h"
#include <configuration/deductions/DeductionDetails.h>
#include <afxwin.h>
#include "../../../ctrls/decimaledit/DecimalEdit.h"
#include "../../../ctrls/stdedit/stdedit.h"

class CDeductionDetailsDlg : public CDialog
{	
public:
	CDeductionDetailsDlg(CWnd* pParent = NULL);
	CDeductionDetailsDlg(const DeductionDetails& details,CWnd* pParent = NULL);
	enum { IDD = IDD_DEDUCTION};	
	virtual ~CDeductionDetailsDlg();	
	const DeductionDetails& GetDeducationDetails() const;

protected:
	virtual std::wstring GetCaption() const = 0;

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
		
	CDecimalEdit m_fraction;
	CStdEdit m_code;
	CStdEdit m_reason;

private:
	DeductionDetails m_details;
	
	DECLARE_MESSAGE_MAP();
};

#endif
#ifndef __DEDUCTIONDEFINITIONDLG_H__
#define __DEDUCTIONDEFINITIONDLG_H__

#include "../../resource.h"
#include <afxwin.h>
#include "../../ctrls/decimaledit/DecimalEdit.h"
#include "../../ctrls/stdedit/stdedit.h"
#include "CRUDDeductionDefinition.h"

class CDeductionDefinitionDlg : public CDialog
{	
public:
	CDeductionDefinitionDlg(CRUDDeductionDefinition* definition, CWnd* pParent = NULL);
	
	enum { IDD = IDD_DEDUCTION_DEFINITION};	
	virtual ~CDeductionDefinitionDlg();

protected:
	
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
		
	CRUDDeductionDefinition* m_definition;
	CDecimalEdit m_fraction;
	CStdEdit m_code;
	CStdEdit m_reason;
	BOOL m_alwaysEnabled;
	
	DECLARE_MESSAGE_MAP();
};

#endif
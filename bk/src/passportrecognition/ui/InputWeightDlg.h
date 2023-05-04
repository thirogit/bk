#ifndef __INPUTWEIGHTDLG_H__
#define __INPUTWEIGHTDLG_H__

#include <afxwin.h>
#include <types/NullDecimal.h>
#include "../../resource.h"
#include "../../ctrls/decimaledit/DecimalEdit.h"
#include <exception/TagException.h>
#include "../../ctrls/tipwnd/TimerTipWnd.h"

typedef std::function<void(const NullDecimal&)> WeightValidator;
struct tag_WeightValidationException{};
typedef TagException<tag_WeightValidationException> WeightValidationException;

class CInputWeightDlg : public CDialog
{
public:
	CInputWeightDlg(CWnd* pParent = NULL);
	virtual ~CInputWeightDlg();
	enum { IDD = IDD_EDITBOXDLG };

	NullDecimal ask();
	CInputWeightDlg& weight(const NullDecimal& weight);
	CInputWeightDlg& validator(WeightValidator& validator);

protected:
	CDecimalEdit  m_WeightEdit;
	CTimerTipWnd m_validationMsg;
	CDialog::DoModal;

	NullDecimal	m_Weight;
	WeightValidator m_validator;

	virtual BOOL OnInitDialog();
	void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
};

#endif
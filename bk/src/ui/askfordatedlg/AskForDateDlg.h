#ifndef __ASKFORDATEDLG_H__
#define __ASKFORDATEDLG_H__

#include <types/DateTime.h>
#include <functional>
#include "../../ctrls/dateedit/dateedit.h"
#include "../../resource.h"

class CAskForDateDlg : public CDialog
{
	DECLARE_DYNAMIC(CAskForDateDlg)

public:
	CAskForDateDlg(CWnd* pParent = NULL);   
	virtual ~CAskForDateDlg();

	enum { IDD = IDD_EDITINPUT };

	DateTime AskForDate();
	void SetLabel(const std::wstring& sLabel);
	void SetWindowCaption(const std::wstring& sCaption);
	void SetDate(const DateTime& dt);

	template<typename F>
	void SetValidator(F& validator)
	{
		m_validator = validator;
	}
	
protected:
	CDateEdit  m_dateEdit;
	CDialog::DoModal;

	DateTime		m_dt;
	std::wstring		m_sLabel;
	std::wstring		m_sCaption;
	std::function<void (const DateTime&)> m_validator;


	virtual BOOL OnInitDialog();
	void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
};


#endif
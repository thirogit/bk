#ifndef __SIMPLEPROGRESSDIALOG_H__
#define __SIMPLEPROGRESSDIALOG_H__

#include <string>
#include <progress\SimpleProgress.h>
#include <afxwin.h>


#define WM_PROGRESS_SETSTEPS (WM_APP+2201)
#define WM_PROGRESS_STEP (WM_APP+2202)

class CSimpleProgressDialog : public CDialog, public SimpleProgress
{

public:
    CSimpleProgressDialog (const std::wstring& sTaskName);
    virtual ~CSimpleProgressDialog ();

	void SetTaskName(const std::wstring& sTaskName);

	virtual void SetSteps(int maxSteps);
	virtual bool Step(int stepsToMake = 1);
	BOOL Create(CWnd *pParent);
	BOOL WasCanceled();
	BOOL DestroyDialog();
	
private:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	void PumpMessages();
	virtual void OnCancel();	

    void ReEnableParent();
	
private:
	BOOL m_bCancel;
	std::wstring m_sTaskName;
	CWnd* m_pParentWnd;
	BOOL m_bParentDisabled;
	CProgressCtrl	m_Progress;

private:
	 
    
	afx_msg	LRESULT OnMessageSetSteps(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMessageStep(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);

	DECLARE_MESSAGE_MAP()
};


#endif
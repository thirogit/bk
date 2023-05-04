#include "stdafx.h"
#include "WaitDialog.h"
#include "../../resource.h"
#include <boost/thread/future.hpp>
#include <future>

CWaitDialog::CWaitDialog (CWnd* pParentWnd, const std::wstring& sWaitText) : CDialog ()
{
   m_pParentWnd = pParentWnd;
   m_sWaitText = sWaitText;   
}

void CWaitDialog::WaitFor(boost::function<void()> &f)
{
	Show();

	std::packaged_task<void()> waitTask(f);
	std::future<void> future = waitTask.get_future();

	boost::thread task(boost::move(waitTask));
	while(!future._Is_ready())
	{
		Pump();
	}
	Hide();

	future.get();
}

CWaitDialog::~CWaitDialog ()
{
    
}

void CWaitDialog::Show ()
{
    m_pParentWnd->EnableWindow (FALSE);
    Create (IDD_BUSY_DLG,m_pParentWnd);
    CStatic* pWaitTextCtrl = (CStatic*) GetDlgItem (IDC_BUSY_TEXT);
	pWaitTextCtrl->SetWindowText (m_sWaitText.c_str());     
    ShowWindow (SW_SHOW);
}

BOOL CWaitDialog::Pump ()
{
    MSG msg;

    // Retrieve and dispatch any waiting messages.
    while (::PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE)) {
        if (!AfxGetApp ()->PumpMessage ()) {
            ::PostQuitMessage (0);
            return FALSE;
        }
    }

    // Simulate the framework's idle processing mechanism.
    LONG lIdle = 0;
    while (AfxGetApp ()->OnIdle (lIdle++));
    return TRUE;
}

void CWaitDialog::Hide ()
{
	m_pParentWnd->EnableWindow (TRUE);
	ShowWindow (SW_HIDE);
    DestroyWindow ();   
}

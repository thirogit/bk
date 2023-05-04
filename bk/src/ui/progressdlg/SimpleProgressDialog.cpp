#include "stdafx.h"
#include "SimpleProgressDialog.h"
#include "../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSimpleProgressDialog::CSimpleProgressDialog(const std::wstring& sTaskName)
{	
	m_sTaskName = sTaskName;
    m_bCancel		= FALSE;  
    m_bParentDisabled = FALSE;
	m_pParentWnd = NULL;
}

CSimpleProgressDialog::~CSimpleProgressDialog()
{
    if(m_hWnd != NULL)
		DestroyDialog();
}

BOOL CSimpleProgressDialog::DestroyDialog()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}

void CSimpleProgressDialog::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled = FALSE;
}

BOOL CSimpleProgressDialog::Create(CWnd *pParent)
{
    m_pParentWnd = CWnd::GetSafeOwner(pParent);    
	return CDialog::Create(IDD_SIMPLE_PROGRESS_DLG,pParent);
}

void CSimpleProgressDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TASK_PROGRESS, m_Progress);
}

BEGIN_MESSAGE_MAP(CSimpleProgressDialog, CDialog)

	ON_MESSAGE(WM_PROGRESS_STEP,OnMessageStep)
	ON_MESSAGE(WM_PROGRESS_SETSTEPS,OnMessageSetSteps)
	ON_WM_SHOWWINDOW()

END_MESSAGE_MAP()

void CSimpleProgressDialog::OnCancel()
{
    m_bCancel = TRUE;
}

void CSimpleProgressDialog::PumpMessages()
{
    ASSERT(m_hWnd!=NULL);
    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if(!IsDialogMessage(&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);  
      }
    }
}

BOOL CSimpleProgressDialog::WasCanceled()
{
    PumpMessages();

    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    BOOL bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}


void CSimpleProgressDialog::SetTaskName(const std::wstring& sTaskName)
{
  if(m_hWnd != NULL)
	SetDlgItemText(IDC_TASK_NAME_LBL,sTaskName.c_str());
}
    

BOOL CSimpleProgressDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    m_Progress.SetRange(0,1);
    m_Progress.SetStep(1);
    m_Progress.SetPos(1);

	SetTaskName(m_sTaskName);
    
	
    return TRUE;  
}

void CSimpleProgressDialog::SetSteps(int maxSteps)
{
	PostMessage(WM_PROGRESS_SETSTEPS,maxSteps);
}

bool CSimpleProgressDialog::Step(int stepsToMake/*=1*/)
{
	PostMessage(WM_PROGRESS_STEP, stepsToMake);
	return m_bCancel == FALSE;
}

LRESULT CSimpleProgressDialog::OnMessageSetSteps(WPARAM wParam,LPARAM lParam)
{
	m_Progress.SetRange32(0,(int)wParam);
	m_Progress.SetPos(0);
	m_Progress.SetStep(1);
	return (LRESULT)0;
}
LRESULT CSimpleProgressDialog::OnMessageStep(WPARAM wParam,LPARAM lParam)
{
	m_Progress.StepIt();
	return (LRESULT)0;
}


void CSimpleProgressDialog::OnShowWindow(BOOL bShow,UINT nStatus)
{
	// m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.
    if(m_pParentWnd != NULL)
    {
      m_pParentWnd->EnableWindow(!bShow);
      m_bParentDisabled = bShow;
    }

}

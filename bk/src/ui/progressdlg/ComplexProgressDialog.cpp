#include "stdafx.h"
#include "ComplexProgressDialog.h"
#include "../../resource.h"
#include <boost/thread/future.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CComplexProgressDialog, CDialog)

	ON_MESSAGE(WM_PROGRESS_SET_ACTIVITY_STEPS,	OnMessageSetActivitySteps)
	ON_MESSAGE(WM_PROGRESS_STEP_ACTIVITY,		OnMessageStepActivity)
	ON_MESSAGE(WM_PROGRESS_SET_TASK_STEPS,		OnMessageSetTaskSteps)
	ON_MESSAGE(WM_PROGRESS_STEP_TASK,			OnMessageStepTask)
	ON_WM_SHOWWINDOW()

END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////
CComplexProgressDialog::SimpleProgressProxy::SimpleProgressProxy(CComplexProgressDialog* pOwner) : m_pOwner(pOwner)
{
}
void CComplexProgressDialog::SimpleProgressProxy::SetSteps(int maxSteps)
{
	m_pOwner->SetTaskSteps(maxSteps);
}

bool CComplexProgressDialog::SimpleProgressProxy::Step(int stepsToMake)
{	
	return m_pOwner->StepTask(stepsToMake);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
CComplexProgressDialog::CComplexProgressDialog(const std::wstring& sActivityTitle, CWnd* pParent) : m_TaskProgressProxy(this)
{
	m_sActivityTitle = sActivityTitle;
    m_bCancel		= FALSE;  
    m_bParentDisabled = FALSE;
	m_pParentWnd = pParent;
}


void CComplexProgressDialog::WaitFor(boost::function<void(ComplexProgress*)> &f)
{
	Show();
		
	boost::packaged_task<void> waitTask(boost::bind(f,((ComplexProgress*)this)));
	boost::unique_future<void> future = waitTask.get_future();

	boost::thread thread(boost::move(waitTask));
	while (!future.is_ready())
	{
		Pump();
	}
	Hide();

	future.get();
}




CComplexProgressDialog::~CComplexProgressDialog ()
{
}

void CComplexProgressDialog::Show()
{
	m_pParentWnd->EnableWindow(FALSE);
	Create(IDD_COMPLEX_PROGRESS_DLG, m_pParentWnd);
	//CStatic* pWaitTextCtrl = (CStatic*)GetDlgItem(IDC_BUSY_TEXT);
	//pWaitTextCtrl->SetWindowText(m_sWaitText.c_str());
	ShowWindow(SW_SHOW);
}

BOOL CComplexProgressDialog::Pump()
{
	MSG msg;

	// Retrieve and dispatch any waiting messages.
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}
	}

	// Simulate the framework's idle processing mechanism.
	LONG lIdle = 0;
	while (AfxGetApp()->OnIdle(lIdle++));
	return TRUE;
}

void CComplexProgressDialog::Hide()
{
	m_pParentWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);
	DestroyWindow();
}


CComplexProgressDialog& CComplexProgressDialog::operator()(const std::wstring& fromtitle, const std::wstring& totitle)
{
	m_titleTranslations[fromtitle] = totitle;
	return *this;
}

SimpleProgress* CComplexProgressDialog::BeginTask(const std::wstring& sTaskName)
{
	std::wstring translatedTaskTitle = sTaskName;
	auto it = m_titleTranslations.find(sTaskName);
	if (it != m_titleTranslations.end())
	{
		translatedTaskTitle = it->second;
	}

	SetTaskName(translatedTaskTitle);
	return &m_TaskProgressProxy;
}

void CComplexProgressDialog::EndTask()
{
	StepActivity();
}

void CComplexProgressDialog::SetTaskCount(int taskCount)
{
	SetActivitySteps(taskCount);
}

bool CComplexProgressDialog::StepTask(int stepsToMake)
{
	PostMessage(WM_PROGRESS_STEP_TASK, stepsToMake);
	return !m_bCancel;
}

void CComplexProgressDialog::SetTaskSteps(int maxSteps)
{
	PostMessage(WM_PROGRESS_SET_TASK_STEPS,maxSteps);
}

void CComplexProgressDialog::SetActivitySteps(int maxSteps)
{
	PostMessage(WM_PROGRESS_SET_ACTIVITY_STEPS,maxSteps);
}

bool CComplexProgressDialog::StepActivity()
{
	PostMessage(WM_PROGRESS_STEP_ACTIVITY);
	return m_bCancel == FALSE;
}


void CComplexProgressDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ACTIVITY_PROGRESS,m_ActivityProgress);
	DDX_Control(pDX, IDC_TASK_PROGRESS,m_TaskProgress);  
}

void CComplexProgressDialog::OnCancel()
{
    m_bCancel = TRUE;
}


bool CComplexProgressDialog::WasCanceled()
{
    // Reset m_bCancel to FALSE so that
    // CheckCancelButton returns FALSE until the user
    // clicks Cancel again. This will allow you to call
    // CheckCancelButton and still continue the operation.
    // If m_bCancel stayed TRUE, then the next call to
    // CheckCancelButton would always return TRUE

    bool bResult = m_bCancel;
    m_bCancel = FALSE;

    return bResult;
}


void CComplexProgressDialog::SetActivityTitle(const std::wstring& sActivityTitle)
{
  if(m_hWnd != NULL)
	SetDlgItemText(IDC_ACTIVITY_NAME_LBL,sActivityTitle.c_str());
}

void CComplexProgressDialog::SetTaskName(const std::wstring& sTaskName)
{
  if(m_hWnd != NULL)
	SetDlgItemText(IDC_TASK_NAME_LBL,sTaskName.c_str());
}   

BOOL CComplexProgressDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    m_ActivityProgress.SetRange(0,1);
    m_ActivityProgress.SetStep(1);
    m_ActivityProgress.SetPos(1);

	m_TaskProgress.SetRange(0,1);
    m_TaskProgress.SetStep(1);
    m_TaskProgress.SetPos(1);

	SetTaskName(m_sTaskName);
	SetActivityTitle(m_sActivityTitle);
    
	
    return TRUE;  
}


LRESULT CComplexProgressDialog::OnMessageSetActivitySteps(WPARAM wParam,LPARAM lParam)
{
	m_ActivityProgress.SetRange32(0,(int)wParam);
	m_ActivityProgress.SetPos(0);
	m_ActivityProgress.SetStep(1);
	return (LRESULT)0;
}

LRESULT CComplexProgressDialog::OnMessageStepActivity(WPARAM wParam,LPARAM lParam)
{
	m_ActivityProgress.StepIt();
	return (LRESULT)0;
}

LRESULT CComplexProgressDialog::OnMessageSetTaskSteps(WPARAM wParam,LPARAM lParam)
{
	m_TaskProgress.SetRange32(0,(int)wParam);
	m_TaskProgress.SetPos(0);
	m_TaskProgress.SetStep(1);
	return (LRESULT)0;
}

LRESULT CComplexProgressDialog::OnMessageStepTask(WPARAM wParam,LPARAM lParam)
{
	m_TaskProgress.SetStep(wParam);
	m_TaskProgress.StepIt();
	return (LRESULT)0;
}

void CComplexProgressDialog::OnShowWindow(BOOL bShow,UINT nStatus)
{
	// m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.
    if(m_pParentWnd != NULL)
    {
      m_pParentWnd->EnableWindow(!bShow);
      m_bParentDisabled = bShow == TRUE;
    }

}

#include "stdafx.h"
#include "App.h"
#include "ChildFrm.h"
#include "SeasonAccess.h"
#include "SessionAwareWnd.h"
#include <arrays/DeepDelete.h>
#include "ui\waitdlg\WaitDialog.h"
#include "utils\StringLoader.h"
#include "resource.h"
#include "repo/ui/RepoDlg.h"
#include "settings/SettingsDlg.h"
#include "collector/ui/CollectorManagerDlg.h"

using namespace std;

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_FILE_CLOSE, &CChildFrame::OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_MESSAGE(WM_GETSEASONSESSION, OnGetSeasonSession)
	ON_COMMAND(ID_START_REPOSITORY, &OnOpenRepository)
	ON_COMMAND(ID_SETTINGS, &OnSettings)
	ON_COMMAND(ID_START_COLLECTOR, &OnCollector)

	
	//ON_UPDATE_COMMAND_UI(ID_START_REPOSITORY,On)
END_MESSAGE_MAP()


CChildFrame::CChildFrame(SeasonSession* pSession) : m_pSeasonSession(pSession)
{		
}

CChildFrame::~CChildFrame()
{
	
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	
	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG


void CChildFrame::PostNcDestroy()
{
	CWaitDialog waitForClose(AfxGetMainWnd(), StringLoader(IDS_WAITCLOSINGSEASON));
	waitForClose.WaitForTask(std::bind(&SeasonSession::Close, m_pSeasonSession));
	CMDIChildWndEx::PostNcDestroy();
}

// CChildFrame message handlers
void CChildFrame::OnFileClose() 
{
	SendMessage(WM_CLOSE);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	SeasonAccess::CreateAssociation(this,m_pSeasonSession);
	
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	

	return 0;
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);

	m_wndView.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	// otherwise, do default handling
	return CMDIChildWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

LRESULT CChildFrame::OnGetSeasonSession(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)m_pSeasonSession;
}


void CChildFrame::OnOpenRepository()
{
	CRepoDlg repoDlg(this);
	repoDlg.DoModal();
}

void CChildFrame::OnSettings()
{	
	CSettingsDlg dlg(this);
	dlg.SetTreeViewMode(TRUE, TRUE, TRUE);
	dlg.SetTreeWidth(220);
	
	dlg.DoModal();
}

void CChildFrame::OnCollector()
{
	CollectorManagerDlg collectorMgrDlg(this);
	collectorMgrDlg.DoModal();
}
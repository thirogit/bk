
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "App.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "context\ContextInitialiser.h"
#include "context\ContextDestructor.h"
#include "season\OpenSeasonDlg.h"
#include "ui\progressdlg\ComplexProgressDialog.h"
#include "utils/StringLoader.h"
#include <boost\function.hpp>
#include <gdiplus.h>
#include <RCF\RCF.hpp>
#include <service\TypesRegistration.h>
#include "welcome\WelcomeDlg.h"
#include "ui\ExceptionMsgBox.h"
#include "context\ContextException.h"
#include "utils/Product.h"
#include "barcodescanner\BarcodeScannerService.h"
#include "print\DocumentManagerService.h"
#include "logo\LogoService.h"
#include "collector\CollectorServer.h"

BEGIN_MESSAGE_MAP(CBKApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBKApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CBKApp::OnFileNew)
END_MESSAGE_MAP()

CBKApp::CBKApp()
{
	m_bHiColorIcons = TRUE;
	SetAppID(_T("bk.BK.2013"));	
}

CBKApp theApp;


XContext* CBKApp::GetContext()
{
	return &m_Context;
}
ServiceRegistry* CBKApp::GetServiceRegistry()
{
	return &m_Registry;
}


BOOL CBKApp::InitInstance()
{
	CWinAppEx::InitInstance();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	RegisterTypes();
		
	XContextInitialiser xcontextinit(&m_Context);
	
	EnableTaskbarInteraction();
		
	SetRegistryKey(_T("BK2013Client"));
	
	InitContextMenuManager();

	InitKeyboardManager();

	AfxEnableControlContainer();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	WelcomeDlg welcomeDlg(&xcontextinit);
	if(welcomeDlg.DoModal() != IDOK)
		return FALSE;

	m_Registry.RegisterService<BarcodeScannerService>((BarcodeScannerService*)NULL);
	m_Registry.RegisterService<LogoService>((LogoService*)NULL);
	m_Registry.RegisterService<DocumentManagerService>((DocumentManagerService*)NULL);
	m_Registry.RegisterService<CollectorServer>((CollectorServer*)NULL);
	
	m_Registry.Start();	

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create main MDI frame window
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// try to load shared MDI menus and accelerator table
	//TODO: add additional member variables and load calls for
	//	additional menu types your application may need
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_MAINMENU));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAINMENU));
	
	// The main window has been initialized, so show and update it
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();

	return TRUE;
}

int CBKApp::ExitInstance()
{
	if (m_hMDIMenu != NULL)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != NULL)
		FreeResource(m_hMDIAccel);

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	m_Registry.Stop();

	XContextDestructor destructor(&m_Context);
	destructor.Destroy();
	
	return CWinAppEx::ExitInstance();
}

// CBKApp message handlers

void CBKApp::OnFileNew() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	OpenSeasonDlg seasonDlg(pFrame);
	ISeason* pSeason = seasonDlg.ChooseSeason(&m_Context);

	if(pSeason != NULL)
	{
		//CWaitDialog waitForSeasonFetchDlg(pFrame,StringLoader(IDS_FETCHING_SEASON));
		
		try
		{
			CComplexProgressDialog progressDialog(StringLoader(IDS_FETCHING_SEASON), pFrame);

			
			progressDialog(L"fetch_classes", StringLoader(IDS_FETCHING_CLASSES))
				(L"fetch_stocks", StringLoader(IDS_FETCHING_STOCKS))
				(L"fetch_hents", StringLoader(IDS_FETCHING_HENTS))
				(L"fetch_cows", StringLoader(IDS_FETCHING_COWS))
				(L"fetch_indocs", StringLoader(IDS_FETCHING_INDOCS))
				(L"fetch_buydocs", StringLoader(IDS_FETCHING_BUYDOCS))
				(L"fetch_movedocs", StringLoader(IDS_FETCHING_MOVEDOCS))
				(L"fetch_selldocs", StringLoader(IDS_FETCHING_SELLDOCS))
				(L"fetch_outdocs", StringLoader(IDS_FETCHING_OUTDOCS))
				(L"fetch_buyinvoices", StringLoader(IDS_FETCHING_BUYINVOICES))
				(L"fetch_sellinvoces", StringLoader(IDS_FETCHING_SELLINVOICES))
				(L"fetch_settings", StringLoader(IDS_FETCHING_SETTINGS))
				(L"creating_herd_registry", StringLoader(IDS_CREATING_HERD_REGISTRY))
				(L"catalog_indocs", StringLoader(IDS_CATALOGING_INDOCS))
				(L"catalog_buydocs", StringLoader(IDS_CATALOGING_BUYDOCS))
				(L"catalog_movedocs", StringLoader(IDS_CATALOGING_MOVEDOCS))
				(L"catalog_outdocs", StringLoader(IDS_CATALOGING_OUTDOCS))
				(L"catalog_selldocs", StringLoader(IDS_CATALOGING_SELLDOCS));

		SeasonSession* pSession = m_Context.OpenSeason(pSeason->GetId());
		pSession->BeginFetch();

		progressDialog.WaitForTask(std::bind(&SeasonSession::DoFetch, pSession, std::placeholders::_1));
															
		pSession->EndFetch();
															
			pFrame->LockWindowUpdate();
			CChildFrame* pChildFrame = pFrame->CreateSessionFrame(pSession, IDR_MAINMENU, m_hMDIMenu, m_hMDIAccel);
			pFrame->UnlockWindowUpdate();

		}
		catch (ContextException& e)
		{
			ExceptionMsgBox box;
			box.Show(e);
			
		}		
	}
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CBKApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetDlgItemText(IDC_VERSION, Product().Version().c_str());
	return TRUE;
}

// CBKApp customization load/save methods

void CBKApp::PreLoadState()
{
}

void CBKApp::LoadCustomState()
{
}

void CBKApp::SaveCustomState()
{
}






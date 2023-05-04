#include "stdafx.h"
#include "FindContainerDlg.h"
#include "FindDlg.h"


BEGIN_MESSAGE_MAP(FindContainerDlg, CDialog)
	ON_BN_CLICKED(IDC_DOFIND, OnOK)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	ON_NOTIFY(FN_FINDREQUESTED, IDC_CONTENT, OnFindRequested)
	ON_WM_SIZE()
END_MESSAGE_MAP()


FindContainerDlg::FindContainerDlg(CRuntimeClass* pContentClass,CWnd* pParent) : 
	CSessionAwareDialog(FindContainerDlg::IDD,pParent), m_pContentClass(pContentClass)
{
}

void FindContainerDlg::Create()
{
	CSessionAwareDialog::Create(FindContainerDlg::IDD, m_pParentWnd);
	CRect rect;
	GetClientRect(rect);
	m_InitialSize.SetSize(rect.Width(), rect.Height());
}

void FindContainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLBAR, m_Toolbar);
	DDX_Control(pDX, IDC_FOUNDCOUNT, m_FoundCount);
	DDX_Control(pDX, IDC_FOUNDCOUNTLABEL, m_FoundLabel);
	DDX_Control(pDX, IDC_HIDE, m_HideBtn);
}
BOOL FindContainerDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();

	CWnd* contentWnd = GetDlgItem(IDC_CONTENT);
	CRect contentRect;
	contentWnd->GetWindowRect(&contentRect);
	ScreenToClient(&contentRect);
	contentWnd->DestroyWindow();

	m_pContent = (FindDlg*)m_pContentClass->CreateObject();
	m_pContent->Create(this);
	m_pContent->SetDlgCtrlID(IDC_CONTENT);

	m_pContent->SetWindowPos(&m_Toolbar, contentRect.left, contentRect.top, contentRect.Width(), contentRect.Height(), SWP_SHOWWINDOW);

	m_FoundLabel.SetBackgroundColor(RGB(0, 0, 0));
	m_FoundLabel.SetTextColor(RGB(255, 255, 255));
	m_FoundCount.SetBackgroundColor(RGB(255, 255, 255));

	
	LOGFONT logFont;
	m_FoundCount.GetFont()->GetLogFont(&logFont);
	logFont.lfHeight -= 5;
	logFont.lfWeight = 1;
	m_foundCountFont.CreateFontIndirectW(&logFont);
	m_FoundCount.SetFont(&m_foundCountFont);


	m_Toolbar.AddButton(std::bind(&FindContainerDlg::OnOK,this),CPng(IDPNG_SEARCH));
	m_Toolbar.AddButton(std::bind(&FindContainerDlg::OnClearFound,this),CPng(IDPNG_TRASHBIG));
	m_Toolbar.AddButton(std::bind(&FindContainerDlg::OnAppend, this), CPng(IDPNG_APPEND));
	//m_Toolbar.AddButton(std::bind(&FindContainerDlg::SelectFound, this), CPng(IDPNG_TARGET));

	m_HideBtn.SetImage(IDBNG_HIDEARROW);

	UpdateFoundCount(NullInt());

	m_pContent->SetFocus();
	return TRUE ;
}


void FindContainerDlg::OnAppend()
{

}


void FindContainerDlg::OnClearFound()
{
	UpdateFoundCount(NullInt());
	GetParent()->SendMessage(WM_CLEARFOUND, 0, (LPARAM)0);
}

void FindContainerDlg::OnCancel()
{
	GetParent()->SendMessage(WM_HIDEFIND, 0, (LPARAM)0);	
}

void FindContainerDlg::OnFindRequested(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMFINDRQ* ntfn = (NMFINDRQ*)pNMHDR;

	int foundCount = (int)GetParent()->SendMessage(WM_FIND, 0, (LPARAM)ntfn->predicte);
	UpdateFoundCount(NullInt(foundCount));
}

void FindContainerDlg::OnOK()
{
	m_pContent->SendMessage(WM_FIND, 0, 0);
	
}

void FindContainerDlg::UpdateFoundCount(const NullInt& count)
{
	if (count.IsNull())
	{
		SetDlgItemText(IDC_FOUNDCOUNT, L"-");

	}
	else
	{
		SetDlgItemInt(IDC_FOUNDCOUNT, count.ToInt(0));
	}
}

void FindContainerDlg::OnHide()
{
	UpdateFoundCount(NullInt());
	GetParent()->SendMessage(WM_HIDEFIND, 0, 0);
}

void FindContainerDlg::OnSize(UINT nType, int cx, int cy)
{
	CSessionAwareDialog::OnSize(nType, cx, cy);
	if (cy > m_InitialSize.cy && m_HideBtn.m_hWnd != NULL)
	{
		CRect wndRect;
		m_HideBtn.GetWindowRect(wndRect);
		ScreenToClient(wndRect);
		m_HideBtn.SetWindowPos(NULL, 0, 0, wndRect.Width(), cy - 3,SWP_NOZORDER|SWP_NOMOVE);
	}
}
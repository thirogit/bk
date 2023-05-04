#include "stdafx.h"
#include "SettingsDlg.h"

IMPLEMENT_DYNAMIC(CSettingsDlg, CSessionAwareWnd<CTreePropSheet>)
CSettingsDlg::CSettingsDlg(CWnd* pParentWnd) : CSessionAwareWnd<CTreePropSheet>(IDS_SETTINGS_DLG_CAPTION, pParentWnd, 0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AddPropPages();
}

CSettingsDlg::~CSettingsDlg()
{
	RemovePropPages();
}

void CSettingsDlg::AddPropPages()
{
	m_pDeductionDefinitionsPage = new DeductionDefinitionsPage();
	m_pDefaultValuedPage = new DefaultValuesPage();
	m_pBehaviourPage = new BehaviourPage();
	
	SetPageIcon(m_pDeductionDefinitionsPage, IDI_PIECHART);
	SetPageIcon(m_pDefaultValuedPage, IDI_SCREWDRIVER);
	SetPageIcon(m_pBehaviourPage, IDI_BRAIN);
	
	AddPage(m_pDeductionDefinitionsPage);
	AddPage(m_pDefaultValuedPage);
	AddPage(m_pBehaviourPage);
	
}

void CSettingsDlg::RemovePropPages()
{
	delete m_pDeductionDefinitionsPage;
	delete m_pDefaultValuedPage;
	delete m_pBehaviourPage;
	
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CSessionAwareWnd<CTreePropSheet>)
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CSettingsDlg::OnInitDialog()
{
	BOOL bResult = CSessionAwareWnd<CTreePropSheet>::OnInitDialog();
//	CAppUtils::MarkWindowAsUnpinnable(m_hWnd);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//CenterWindow(CWnd::FromHandle(hWndExplorer));

	
	return bResult;
}

void CSettingsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CTreePropSheet::OnPaint();
	}
}

HCURSOR CSettingsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

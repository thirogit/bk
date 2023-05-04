#include "stdafx.h"
#include "CollectorDownloadDlg.h"
#include "../import/ImportProblemIdentifier.h"


static const int _hide_tooltip_after_ms = 2000;
static const int _tooltip_fontheight = 45;

BEGIN_MESSAGE_MAP(CollectorDownloadDlg, CLazyDlg<CSessionAwareDialog>)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CollectorDownloadDlg::CollectorDownloadDlg(const DownloadSnapshot* snapshot, CWnd* pParent) : CLazyDlg<CSessionAwareDialog>(CollectorDownloadDlg::IDD, pParent)
{
	m_Snapshot = new DownloadSnapshot(*snapshot);
}

CollectorDownloadDlg::~CollectorDownloadDlg()
{
	delete m_Snapshot;
}

void CollectorDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CLazyDlg<CSessionAwareDialog>::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NAVIGATION, m_Nav);
}

BOOL  CollectorDownloadDlg::OnInitDialog()
{
	CLazyDlg<CSessionAwareDialog>::OnInitDialog();
	return TRUE ;
}


void  CollectorDownloadDlg::OnOK()
{
	EndDialog(IDOK); 
}

void CollectorDownloadDlg::OnDestroy()
{
		
	CSessionAwareDialog::OnDestroy();
}

void CollectorDownloadDlg::OnAfterDlgShow()
{
	
	





}
void CollectorDownloadDlg::ShowCollectorLostMsg()
{
	/*m_Msg.SetText(StringLoader(IDS_COLLECTORLOST));
	m_Msg.SetStyle(TipWndStyle().inner(_red).outer(_red).textsize(_tooltip_fontheight));
	m_Msg.Show(_hide_tooltip_after_ms);
	MessageBeep(MB_ICONERROR);*/
}



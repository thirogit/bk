#include "stdafx.h"
#include "updater.h"
#include "updaterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CupdaterDlg::CupdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CupdaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CupdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CupdaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

END_MESSAGE_MAP()


BOOL CupdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  
}


void CupdaterDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CupdaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


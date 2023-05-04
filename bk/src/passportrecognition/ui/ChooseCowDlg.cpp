#include "stdafx.h"
#include "ChooseCowDlg.h"
#include "../../utils/StringLoader.h"

BEGIN_MESSAGE_MAP(CChooseCowDlg, CSessionAwareDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CChooseCowDlg::CChooseCowDlg(PassportRecognitionSession* session, CWnd* pParent /*=NULL*/) : 
	CSessionAwareDialog(CChooseCowDlg::IDD, pParent),
	m_session(), m_content(NULL)
{
	
}

CChooseCowDlg::~CChooseCowDlg()
{
}

void CChooseCowDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COWLIST,m_cowList);
}

BOOL CChooseCowDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();
	
	
	ListHeaderDescriptor header;
	header(RecognitionItemViewMembers::RecognitionItemView_CowNo, StringLoader(IDS_HEADER_COWNO))
		(RecognitionItemViewMembers::RecognitionItemView_PassNo, StringLoader(IDS_HEADER_PASSNO));


	m_content = new RecognitionSessionContent(GetSeasonSession());
	m_content->Create(m_session);
	m_cowList.SetHeaderDescriptor(header);
	m_cowList.SetContent(m_content);
	
	
	return TRUE;
}

void CChooseCowDlg::OnOK()
{
	/*int curSel = m_cowList.GetCurSel();
	if(curSel < 0)
	{
		AfxMessageBox(IDS_ERRCHOOSECOWEAN,MB_OK,0);
		return;
	}
	m_pSelectedCow = m_cowArray[curSel];*/
	EndDialog(IDOK);
}


ItemId CChooseCowDlg::ChooseCow()
{
	if(DoModal() == IDOK)
	{		
		//return chooseCow.m_pSelectedCow;
	}	
	return ItemId();
}

void CChooseCowDlg::OnDestroy()
{
	if (m_content)
	{
		m_content->Destroy();
		delete m_content;
		m_content = NULL;
	}

	CSessionAwareDialog::OnDestroy();
}
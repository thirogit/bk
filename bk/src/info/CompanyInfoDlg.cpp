#include "stdafx.h"
#include "CompanyInfoDlg.h"
#include "../utils/StringLoader.h"
#include "../SeasonAccess.h"


BEGIN_MESSAGE_MAP(CompanyInfoDlg, CSessionAwareDialog)
		
END_MESSAGE_MAP()


CompanyInfoDlg::CompanyInfoDlg(CWnd* pParent) : CSessionAwareDialog(CompanyInfoDlg::IDD,pParent)
{	
}

void CompanyInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	
	
	
}


BOOL CompanyInfoDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();

	/*ListHeaderDescriptor hentsHeader;
	
	hentsHeader (HentViewMember::Name,					StringLoader(IDS_HEADER_HENTNAME))
				(HentViewMember::HentNo,				StringLoader(IDS_HEADER_HENTNO))
				(HentViewMember::Alias,					StringLoader(IDS_HEADER_HENTALIAS))
				(HentViewMember::Street,				StringLoader(IDS_HEADER_STREET))
				(HentViewMember::POBox,					StringLoader(IDS_HEADER_POBOX))
				(HentViewMember::City,					StringLoader(IDS_HEADER_CITY))
				(HentViewMember::Zip,					StringLoader(IDS_HEADER_ZIP))				
				(HentViewMember::Type,					StringLoader(IDS_HEADER_HENTTYPE));

	SeasonSession* session = GetSeasonSession();
	m_hdrLayout.Load(session);
	m_ResultList.SetHeaderDescriptor(hentsHeader);
	m_ResultList.SetHeaderLayout(&m_hdrLayout);
	m_ResultList.SetContent(&m_ResultContent);
	m_ResultList.SetRowDblClickHandler(std::bind(&HentSearchDlg::OnResultRowDblClick, this, std::placeholders::_1));
	OnQueryChange();
	m_ResultContent.AddContentObserver(this);
	UpdateResultCount();
	m_SelectedHentId = NULL_ID;

	m_Msg.Create(this);
	m_Msg.SetShowCenteredInParent();*/

	return TRUE ;
}



void CompanyInfoDlg::OnOK()
{
	EndDialog(IDOK);
}


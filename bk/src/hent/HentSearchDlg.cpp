#include "stdafx.h"
#include "HentSearchDlg.h"
#include "../utils/StringLoader.h"
#include "../view/HentView.h"
#include "../SeasonAccess.h"
#include "HentEditor.h"
#include "HentDlg.h"
#include <string\IntString.h>
#include "../AppServices.h"

#define MIN_QUERY_LENGTH 3
#define TOOLTIP_SHOW_DURATION 1000

BEGIN_MESSAGE_MAP(HentSearchDlg, CSessionAwareDialog)
		ON_NOTIFY(EN_EDITCOMPLETE,IDC_QUERYPATTERN,OnQueryComplete)
		ON_EN_CHANGE(IDC_QUERYPATTERN,OnQueryChange)
		ON_BN_CLICKED(IDC_SEARCH,OnSearchClicked)
		ON_BN_CLICKED(IDC_ADDHENT,OnAddHent)
		ON_BN_CLICKED(IDC_CLEARRESULTS,OnClearResults)
		ON_WM_DESTROY()
END_MESSAGE_MAP()


HentSearchDlg::HentSearchDlg(CWnd* pParent) : CSessionAwareDialog(HentSearchDlg::IDD,pParent), m_hdrLayout(L"HENTSEARCH")
{	
}

void HentSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);
	
	DDX_Control (pDX, IDC_RESULTLIST, m_ResultList) ;
	DDX_Control (pDX, IDC_QUERYPATTERN, m_QueryPatternEdit);
	DDX_Control(pDX, IDC_SCANNERSTATUS, m_ScannerStatusInd);
	
}

void HentSearchDlg::OnDestroy()
{
	m_ResultContent.DestroyContent();
	m_hdrLayout.Save(GetSeasonSession());

	AppServices::GetBCScannerService()->RemoveListener(this);
	
	CSessionAwareDialog::OnDestroy();
}

BOOL HentSearchDlg::OnInitDialog()
{
	CSessionAwareDialog::OnInitDialog();

	ListHeaderDescriptor hentsHeader;
	
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
	m_Msg.SetShowCenteredInParent();

	AppServices::GetBCScannerService()->AddListener(this);
	UpdateBarcodeIndictor();


	return TRUE ;
}

void HentSearchDlg::QueryHent(const std::wstring& sQuery)
{
	CWnd* parent = GetParent();
	CWnd* owner = GetOwner();

	SeasonSession* session = GetSeasonSession();
	m_ResultContent.DestroyContent();
	m_ResultContent.CreateContent(session,sQuery);

	if (m_ResultContent.GetRowCount() == 0)
	{
		Color red(255, 242, 0);
		m_Msg.SetText(StringLoader(IDS_NOHENTFOUND));
		m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(25));
		m_Msg.Show(TOOLTIP_SHOW_DURATION);
		MessageBeep(MB_ICONHAND);
	}

	
}

void HentSearchDlg::OnQueryComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	OnSearchClicked();
	
	*pResult = 0;
}

void HentSearchDlg::OnQueryChange()
{
	GetDlgItem(IDC_SEARCH)->EnableWindow(m_QueryPatternEdit.GetWindowTextLength() >= MIN_QUERY_LENGTH);
}

void HentSearchDlg::OnSearchClicked()
{
	std::wstring sQueryText = m_QueryPatternEdit.GetText();
	if(sQueryText.length() >= MIN_QUERY_LENGTH)
	{
		QueryHent(sQueryText);	
	}
	else
	{
		Color red(255, 0, 0);
		m_Msg.SetText(StringLoader(IDS_ERR_MINQUERYLENGTH));
		m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(25));
		m_Msg.Show(TOOLTIP_SHOW_DURATION);
		MessageBeep(MB_ICONERROR);
	}
}

void HentSearchDlg::OnAddHent()
{	
	SeasonSession* session = GetSeasonSession();
	HentEditor editor(session);
	editor.New();
	HentDlg hendDlg(&editor,this);
	hendDlg.DoModal();	
}

void HentSearchDlg::OnClearResults()
{
	m_ResultContent.DestroyContent();
}

void HentSearchDlg::UpdateResultCount()
{
	SetDlgItemText(IDC_RESULTCOUNT,IntString(m_ResultContent.GetRowCount()).c_str());
}

void HentSearchDlg::OnCountChanged()
{
	UpdateResultCount();
}

uint32_t HentSearchDlg::GetSelectedHent() const
{
	return m_SelectedHentId;
}

void  HentSearchDlg::OnResultRowDblClick(int row)
{
	OnOK();
}

void HentSearchDlg::OnOK()
{
	int nSelectedItem = m_ResultList.GetCurSel();
	if(nSelectedItem < 0)
	{
		AfxMessageBox(IDS_ERRSELECTHENT,MB_OK|MB_ICONERROR);
	}
	else
	{
		HentRow* pRow = m_ResultContent.GetHentAtRow(nSelectedItem);
		m_SelectedHentId = pRow->Content()->GetHentId();
		EndDialog(IDOK);
	}
}

uint32_t HentSearchDlg::FindHent()
{
	if (DoModal() == IDOK)
		return m_SelectedHentId;

	return NULL_ID;
}

void HentSearchDlg::OnBarcode(const std::wstring& sBarcode)
{
	Color red(255, 0, 0);
	try
	{
		HentNo hentNo = HentNoFactory::FromString(sBarcode);
		std::wstring sHentNo = hentNo.ToString();
		m_QueryPatternEdit.SetCommitedText(sHentNo.c_str());
		m_QueryPatternEdit.SetSel(0, -1);
		QueryHent(sHentNo);
	}
	catch (...)
	{
		m_Msg.SetText(StringLoader(IDS_BADHENTNOBARCODE));
		m_Msg.SetStyle(TipWndStyle().inner(red).outer(red).textsize(25));
		m_Msg.Show(TOOLTIP_SHOW_DURATION);
		MessageBeep(MB_ICONWARNING);
	}
}

void HentSearchDlg::OnScannerConnected()
{
	UpdateBarcodeIndictor();
}

void HentSearchDlg::OnScannerDisconnected()
{
	UpdateBarcodeIndictor();
}

void HentSearchDlg::UpdateBarcodeIndictor()
{
	m_ScannerStatusInd.SetStatus(AppServices::GetBCScannerService()->GetConnectionStatus());
}
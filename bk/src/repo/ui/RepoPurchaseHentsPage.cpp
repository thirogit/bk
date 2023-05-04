#include "stdafx.h"
#include "RepoPurchaseHentsPage.h"
#include "../../utils/StringLoader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseHentsPage, ETSLayoutDialog)
CRepoPurchaseHentsPage::CRepoPurchaseHentsPage() : ETSLayoutDialog(CRepoPurchaseHentsPage::IDD,NULL,NULL,false)
{	

}

CRepoPurchaseHentsPage::~CRepoPurchaseHentsPage()
{
	
}

void CRepoPurchaseHentsPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);		
	DDX_Control(pDX,IDC_ITEMLIST,m_HentList);
}


BEGIN_MESSAGE_MAP(CRepoPurchaseHentsPage, ETSLayoutDialog)
	
END_MESSAGE_MAP()


BOOL CRepoPurchaseHentsPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	CDialog::OnInitDialog();	

	CreateRoot(HORIZONTAL,0)->addItem(&m_HentList);


	ListHeaderDescriptor hentHeader; 
	hentHeader(PurchaseHentViewMember::Name, StringLoader(IDS_HEADER_HENTNAME))
		(PurchaseHentViewMember::HentNo, StringLoader(IDS_HEADER_HENTNO))
		(PurchaseHentViewMember::Alias, StringLoader(IDS_HEADER_HENTALIAS))
		(PurchaseHentViewMember::Street, StringLoader(IDS_HEADER_STREET))
		(PurchaseHentViewMember::POBox, StringLoader(IDS_HEADER_POBOX))
		(PurchaseHentViewMember::City, StringLoader(IDS_HEADER_CITY))
		(PurchaseHentViewMember::Zip, StringLoader(IDS_HEADER_ZIP))
		(PurchaseHentViewMember::PlateNo, StringLoader(IDS_HEADER_PLATENO))
		(PurchaseHentViewMember::Type, StringLoader(IDS_HEADER_HENTTYPE))
		(PurchaseHentViewMember::PhoneNo, StringLoader(IDS_HEADER_PHONENO))
		(PurchaseHentViewMember::FiscalNo, StringLoader(IDS_HEADER_FISCALNO))
		(PurchaseHentViewMember::WetNo, StringLoader(IDS_HEADER_WETNO))
		(PurchaseHentViewMember::PersonalNo, StringLoader(IDS_HEADER_PERSONALNO))
		(PurchaseHentViewMember::StatNo, StringLoader(IDS_HEADER_STATNO))
		(PurchaseHentViewMember::PersonalIdNo, StringLoader(IDS_HEADER_PERSONALIDNO))
		(PurchaseHentViewMember::PersonalIdIssueDt, StringLoader(IDS_HEADER_PERSIDISSUEDT))
		(PurchaseHentViewMember::PersonalIdIssuePost, StringLoader(IDS_HEADER_PERSIDISSUEPORT))
		(PurchaseHentViewMember::AccountNo, StringLoader(IDS_HEADER_ACCOUNTNO))
		(PurchaseHentViewMember::BankName, StringLoader(IDS_HEADER_BANKNAME))
		(PurchaseHentViewMember::WetLicenceNo, StringLoader(IDS_HEADER_WETLICENCENO))
		(PurchaseHentViewMember::CellPhoneNo, StringLoader(IDS_HEADER_CELLPHONENO))
		(PurchaseHentViewMember::EmailAddress, StringLoader(IDS_HEADER_EMAILADDRESS))
		(PurchaseHentViewMember::Latitude, StringLoader(IDS_HEADER_LATITUDE))
		(PurchaseHentViewMember::Longitude, StringLoader(IDS_HEADER_LONGITUDE));


	m_HentList.SetHeaderDescriptor(hentHeader);
	
	return TRUE;
}


void CRepoPurchaseHentsPage::UpdateFrom(IPurchase* pPurchase)
{
	m_HentListContent.Clear();
	m_HentList.EnableWindow(pPurchase != NULL);
	
	m_HentList.SetContent(NULL);
	if (pPurchase != NULL)
	{
		PtrEnumerator<IPurchaseHent> enumHents = pPurchase->EnumHents();
		while (enumHents.hasNext())
		{
			m_HentListContent.AddRow(*enumHents);
			enumHents.advance();
		}

		m_HentList.SetContent(&m_HentListContent);
	}

}




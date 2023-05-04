#include "stdafx.h"
#include "ImageSettingsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CImageSettingsPage, CPropertyPage)

CImageSettingsPage::CImageSettingsPage(ImageSettings* pImageSettings) 	: CPropertyPage(CImageSettingsPage::IDD)
{
	m_pImageSettings = pImageSettings;

	
}

CImageSettingsPage::~CImageSettingsPage()
{
	
}

void CImageSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BLACKLEVEL,m_BlackLevel);
	DDX_Control(pDX,IDC_CENTER,m_Center);
	DDX_Control(pDX,IDC_SIGMA,m_Sigma);
	DDX_Check(pDX,IDC_DOPALETENHANCE,m_bDoPalleteEnhance);
	
}


BEGIN_MESSAGE_MAP(CImageSettingsPage, CPropertyPage)
	ON_BN_CLICKED(IDC_DOPALETENHANCE,OnDoPalleteEnhance)
END_MESSAGE_MAP()


BOOL CImageSettingsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_BlackLevel.SetBounds(128,210);
	m_BlackLevel.SetTic(10);
	m_BlackLevel.SetPage(10);
	m_BlackLevel.SetPos(m_pImageSettings->GetBlackLevel());

	m_Center.SetBounds(0,255);
	m_Center.SetTic(1);
	m_Center.SetPage(10);
	m_Center.SetPos(m_pImageSettings->GetPaletteCenter());

	m_Sigma.SetBounds(1,510);
	m_Sigma.SetTic(50);
	m_Sigma.SetPage(100);
	m_Sigma.SetPos(m_pImageSettings->GetPaletteSigma());

	m_bDoPalleteEnhance = m_pImageSettings->GetDoPaletteEnhance();

	UpdateData(FALSE);

	OnDoPalleteEnhance();

	return TRUE;
}

void CImageSettingsPage::OnDoPalleteEnhance()
{
	UpdateData();
	m_Center.EnableWindow(m_bDoPalleteEnhance);
	m_Sigma.EnableWindow(m_bDoPalleteEnhance);
}

void CImageSettingsPage::OnOK()
{
	UpdateData();

	m_pImageSettings->SetBlackLevel(m_BlackLevel.GetPos());
	m_pImageSettings->SetDoPaletteEnhance(m_bDoPalleteEnhance == TRUE);
	m_pImageSettings->SetPaletteCenter(m_Center.GetPos());
	m_pImageSettings->SetPaletteSigma(m_Sigma.GetPos());

	
}

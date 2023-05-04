#include "stdafx.h"
#include "ScannerSettingsPage.h"
#include <string/TextUtils.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



IMPLEMENT_DYNAMIC(CScannerSettingsPage, CPropertyPage)

CScannerSettingsPage::CScannerSettingsPage(ScannerSettings *pScannerSettings) 	: CPropertyPage(CScannerSettingsPage::IDD)
{
	m_pScannerSettings = pScannerSettings;	
}

CScannerSettingsPage::~CScannerSettingsPage()
{
	
}

void CScannerSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	
	DDX_Control(pDX,IDC_BRIGHTNESS,m_BrightnessSlider);
	DDX_Control(pDX,IDC_RESOLUTIONCOMBO,m_ResolutionCb);
	DDX_Control(pDX,IDC_CONTRAST,m_ContrastSlider);	
	
}


BEGIN_MESSAGE_MAP(CScannerSettingsPage, CPropertyPage)
	
END_MESSAGE_MAP()


BOOL CScannerSettingsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	const UINT uScannerResolutions[] = {300, 400, 600, 0};

	int iItem,iSelectedItem = -1;
	std::wstring sItemText;
	for(const UINT* puScannerResolution  = uScannerResolutions;*puScannerResolution; puScannerResolution++)
	{
		sItemText = TextUtils::Format(L"%d DPI",*puScannerResolution);
		iItem = m_ResolutionCb.AddString(sItemText.c_str());
		if(*puScannerResolution == m_pScannerSettings->GetResolution()) iSelectedItem = iItem;

		if(iItem < iSelectedItem) iSelectedItem++;

		m_ResolutionCb.SetItemData(iItem,*puScannerResolution);
	}	

	m_ResolutionCb.SetCurSel(iSelectedItem);


	m_BrightnessSlider.SetBounds(-100,100);
	m_BrightnessSlider.SetTic(20);
	m_BrightnessSlider.SetPage(10);

	m_BrightnessSlider.SetPos(m_pScannerSettings->GetBrightness());

	m_ContrastSlider.SetBounds(-100,100);
	m_ContrastSlider.SetTic(20);
	m_ContrastSlider.SetPage(10);

	m_ContrastSlider.SetPos(m_pScannerSettings->GetContrast());
	
	return TRUE;
}

void CScannerSettingsPage::OnOK()
{
	UpdateData();

	m_pScannerSettings->SetBrightness(m_BrightnessSlider.GetPos());
	m_pScannerSettings->SetContrast(m_ContrastSlider.GetPos());
	m_pScannerSettings->SetResolution(m_ResolutionCb.GetItemData(m_ResolutionCb.GetCurSel()));
	

}

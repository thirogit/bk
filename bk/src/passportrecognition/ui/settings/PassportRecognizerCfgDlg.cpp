#include "stdafx.h"
#include "PassportRecognizerCfgDlg.h"
#include "../../../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CPassportRecognizerCfgDlg::CPassportRecognizerCfgDlg(PassportRecognizerSettings* pRecognizerSettings,CWnd* pWndParent/* = NULL*/)  : 
	CPropertySheet(IDS_PASSPORTRECOGNITIONOPTIONSDLGCAPTION, pWndParent),
		m_ScannerSettingsPage(&pRecognizerSettings->GetScannerSettings()),
		m_ImageSettingsPage(&pRecognizerSettings->GetImageSettings()),
		m_DataSettingsPage(&pRecognizerSettings->GetDataSettings()),
		m_EngineSettingsPage(&pRecognizerSettings->GetEngineSettings())
{

	AddPage(&m_DataSettingsPage);
	AddPage(&m_ScannerSettingsPage);
	AddPage(&m_ImageSettingsPage);	
	AddPage(&m_EngineSettingsPage);
	
}

CPassportRecognizerCfgDlg::~CPassportRecognizerCfgDlg()
{
}


BEGIN_MESSAGE_MAP(CPassportRecognizerCfgDlg, CPropertySheet)

END_MESSAGE_MAP()



BOOL CPassportRecognizerCfgDlg::OnInitDialog() 
{

	CPropertySheet::OnInitDialog();

	
	return TRUE;
}

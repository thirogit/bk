#include "stdafx.h"
#include "EngineSettingsPage.h"
#include "../../../ctrls/stdedit/stdedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC(CEngineSettingsPage, CPropertyPage)

CEngineSettingsPage::CEngineSettingsPage(EngineSettings* pEngineSettings) 	: CPropertyPage(CEngineSettingsPage::IDD)
{
	m_pEngineSettings = pEngineSettings;

	
}

CEngineSettingsPage::~CEngineSettingsPage()
{
	
}

void CEngineSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ENGINEPATH, m_EnginePath);
	DDX_Control(pDX, IDC_LICENSEFILEPATH, m_LicenseFilePath);
	DDX_StdText(pDX,IDC_DEVELOPERSN,m_DeveloperSN);
	DDX_StdText(pDX, IDC_PROJECTID, m_ProjectId);
	
}


BEGIN_MESSAGE_MAP(CEngineSettingsPage, CPropertyPage)
	
END_MESSAGE_MAP()


BOOL CEngineSettingsPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_EnginePath.EnableFolderBrowseButton();
	m_EnginePath.SetWindowText(m_pEngineSettings->GetEnginePath().c_str());

	m_LicenseFilePath.EnableFileBrowseButton();
	m_LicenseFilePath.SetWindowText(m_pEngineSettings->GetLicenseFilePath().c_str());

	m_DeveloperSN = m_pEngineSettings->GetDeveloperSN();

	m_ProjectId = m_pEngineSettings->GetProjectId();
	
	UpdateData(FALSE);

	return TRUE;
}

void CEngineSettingsPage::OnOK()
{
	UpdateData();

	CString sEnginePath;
	m_EnginePath.GetWindowText(sEnginePath);

	CString sLicenseFilePath;
	m_LicenseFilePath.GetWindowText(sLicenseFilePath);

	m_pEngineSettings->SetDeveloperSN(m_DeveloperSN);
	m_pEngineSettings->SetEnginePath((LPCWSTR)sEnginePath);
	m_pEngineSettings->SetLicenseFilePath((LPCWSTR)sLicenseFilePath);
	m_pEngineSettings->SetProjectId(m_ProjectId);	
	
}

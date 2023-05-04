#ifndef __ENGINESETTINGSPAGE_H__
#define __ENGINESETTINGSPAGE_H__

#include <afxdlgs.h>
#include "../../settings/EngineSettings.h"
#include <afxeditbrowsectrl.h>
#include "../../../resource.h"

class CEngineSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEngineSettingsPage)

public:
	CEngineSettingsPage(EngineSettings* pEngineSettings);
	virtual ~CEngineSettingsPage();

	enum { IDD = IDD_RECOGNITIONENGINECFG };

protected:

	CMFCEditBrowseCtrl m_EnginePath;
	CMFCEditBrowseCtrl m_LicenseFilePath;
	std::wstring m_DeveloperSN;
	std::wstring m_ProjectId;

	EngineSettings* m_pEngineSettings;
protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
		
	DECLARE_MESSAGE_MAP()

};

#endif
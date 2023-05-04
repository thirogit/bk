#ifndef __PASSPORTRECOGNIZERCFGDLG_H__
#define __PASSPORTRECOGNIZERCFGDLG_H__

#include "../../settings/PassportRecognizerSettings.h"

#include "ScannerSettingsPage.h"
#include "ImageSettingsPage.h"
#include "DataSettingsPage.h"
#include "EngineSettingsPage.h"


class CPassportRecognizerCfgDlg : public CPropertySheet
{
public:
	CPassportRecognizerCfgDlg(PassportRecognizerSettings* pRecognizerSettings,CWnd* pWndParent = NULL);
	virtual ~CPassportRecognizerCfgDlg();

protected:

	CScannerSettingsPage m_ScannerSettingsPage;
	CImageSettingsPage m_ImageSettingsPage;
	CDataSettingsPage m_DataSettingsPage;
	CEngineSettingsPage m_EngineSettingsPage;
	
	PassportRecognizerSettings* m_pRecognizerSettings;
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};

#endif
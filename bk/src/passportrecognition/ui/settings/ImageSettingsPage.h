#ifndef __IMAGESETTINGSPAGE_H__
#define __IMAGESETTINGSPAGE_H__

#include "../ctrls/IntValueSliderCtrl.h"
#include "../../settings/ImageSettings.h"
#include "../../../resource.h"
#include <afxdlgs.h>

class CImageSettingsPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CImageSettingsPage)

public:
	CImageSettingsPage(ImageSettings* pImageSettings);
	virtual ~CImageSettingsPage();
	enum { IDD = IDD_RECOGNITIONIMAGEOPTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	BOOL m_bDoPalleteEnhance;
	
	CIntValueSliderCtrl m_BlackLevel;
	CIntValueSliderCtrl m_Center;
	CIntValueSliderCtrl m_Sigma;

	ImageSettings* m_pImageSettings;

	
protected:	
	afx_msg void OnDoPalleteEnhance();

	DECLARE_MESSAGE_MAP()
};

#endif
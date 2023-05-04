#pragma once
#include "../../resource.h"
#include "../SettingsPropPage.h"
#include "../../ctrls/BaseCombo.h"
#include "../../ctrls/vatratecombo/VATCombo.h"
#include "../../ctrls/paywaycombo/PayWayCombo.h"
#include "../../ctrls/integeredit/IntegerEdit.h"
#include "../../ctrls/HentTypeCombo.h"
#include "../../ctrls/MotiveCombo.h"
#include "../../ctrls/cowsexcombo/CowSexCombo.h"
#include <afxcmn.h>
#include "../DefaultValues.h"

class DefaultValuesPage : public SettingsPropPage
{
	DECLARE_DYNAMIC(DefaultValuesPage)

public:
	DefaultValuesPage();
	virtual ~DefaultValuesPage();

	enum { IDD = IDD_DEFAULTVALUES };

private:
	CBaseCombo	  m_stocks;
	CBaseCombo	  m_classes;

	CVATCombo    m_vatRateRegInvoiceCb;
	CPaywayCombo m_paywayRegInvoiceCb;

	CVATCombo    m_vatRateLumpInvoiceCb;
	CPaywayCombo m_paywayLumpInvoiceCb;

	BOOL         m_bSetInMotive;
	BOOL         m_bSetOutMotive;
	BOOL         m_bSetHentType;
	BOOL         m_bSetStock;
	BOOL         m_bSetClass;
	BOOL		 m_bSetPayWayRegInvoice;
	BOOL		 m_bSetPayWayLumpInvoice;
	
	CMotiveCombo m_inmotives;
	CMotiveCombo m_outmotives;
	CCowSexCombo m_sexes;
	CHentTypeCombo m_henttypes;
	
	CIntegerEdit m_payDueDaysRegular;
	CIntegerEdit m_payDueDaysLump;
	BOOL		 m_bSetPayDueDaysRegInvoice;
	BOOL		 m_bSetPayDueDaysLumpInvoice;
	

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void DoDataExchange(CDataExchange* pDX);  	
	
	void SaveDefaultValues();
	void Set(const DefaultValues& values);
	void Get(DefaultValues& values);

	afx_msg void OnDefaultStock();
	afx_msg void OnDefaultCowClass();
	afx_msg void OnDefaultHentType();
	afx_msg void OnDefaultInMotive();
	afx_msg void OnDefaultOutMotive();
	afx_msg void OnDefaultPayWayRegInvoice();
	afx_msg void OnDefaultPayWayLumpInvoice();
	afx_msg void OnDefaultPayDueDaysRegInvoice();
	afx_msg void OnDefaultPayDueDaysLumpInvoice();

	DECLARE_MESSAGE_MAP()

};

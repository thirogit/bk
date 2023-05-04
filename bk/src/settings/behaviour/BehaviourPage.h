#pragma once
#include "../../resource.h"
#include "../SettingsPropPage.h"
#include "../../uitypes/Color.h"
#include <types/NullDecimal.h>

class BehaviourPage : public SettingsPropPage
{
	DECLARE_DYNAMIC(BehaviourPage)

public:
	BehaviourPage();
	virtual ~BehaviourPage();

	enum { IDD = IDD_BEHAVIOUROPTIONS };
private:
	BOOL m_bAskBeforeExit;
	BOOL m_bMarkCowsOlderThan;
	int m_MarkCowsAgeInDays;
	Color m_MarkAgeColor;
	BOOL m_bWarnBeforeMarkAge;
	int m_WarnBeforeDays;
	Color m_WarnBeforeMarkAgreColor;
	BOOL m_WarnWhenLoss;
	Color m_LossWarningColor;
	BOOL m_WarnPriceLessThan;
	NullDecimal m_LowPriceLimit;
	BOOL m_WarnProfit;
	NullDecimal m_ProfitTreshold;
	BOOL m_WarnWhenPlateNoIsMissing;

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void DoDataExchange(CDataExchange* pDX);  	
	

	DECLARE_MESSAGE_MAP()
};

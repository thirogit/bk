#ifndef __BANKACCOUNTSCOMBOCONTENT_H__
#define __BANKACCOUNTSCOMBOCONTENT_H__

#include "../ctrls/BaseCombo.h"
#include <data\IBankAccount.h>
#include "../context/SeasonSession.h"

class BankAccountsComboContent : public ComboContent
{
public:
	void CreateContent(SeasonSession* facade);
	void DestroyContent();
};

#endif
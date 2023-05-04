#include "stdafx.h"
#include "BankAccountsComboContent.h"


void BankAccountsComboContent::CreateContent(SeasonSession* facade)
{
	PtrEnumerator<IBankAccount>& accountsEnum = facade->EnumBankAccounts();
	IBankAccount* pAccount = NULL;
	while (accountsEnum.hasNext())
	{
		pAccount = *accountsEnum;
		AddItem(pAccount->GetId(), pAccount->GetAlias() + L" - " + pAccount->GetAccountNo().GetNumber());
		accountsEnum.advance();
	}
}

void BankAccountsComboContent::DestroyContent()
{
	ComboContent::Destroy();
}

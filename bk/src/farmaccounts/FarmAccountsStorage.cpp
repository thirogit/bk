#include "stdafx.h"
#include "FarmAccountsStorage.h"
#include <filesystem\paths.h>
#include <boost\foreach.hpp>
#include <data\IIdable.h>
#include <pugixml.hpp>

#define ACCOUNTS_FILE L"accounts.xml"

FarmAccounts FarmAccountsStorage::Load()
{
	FarmAccounts accounts;

	pugi::xml_document doc;
			 
    std::wstring storageFile = GetStorageFile(ACCOUNTS_FILE);

	if(paths_DoesPathExist(storageFile))
	{
		pugi::xml_parse_result result = doc.load_file(storageFile.c_str());

		pugi::xml_node xmlaccounts = doc.child(L"accounts");

		if (xmlaccounts)
		{
		
			for (pugi::xml_node xmlaccount = xmlaccounts.first_child(); xmlaccount; xmlaccount = xmlaccount.next_sibling())
			{
				try
				{
					FarmAccount account(NULL_ID);
					account.SetName(xmlaccount.child(L"name").text().as_string());
					account.SetFarm(FarmNoFactory::FromString(xmlaccount.child(L"farmno").text().as_string()));
					account.SetAddress(xmlaccount.child(L"address").text().as_string());
					account.SetPort(xmlaccount.child(L"port").text().as_int());
					account.SetLogin(xmlaccount.child(L"login").text().as_string());
					account.SetPassword(xmlaccount.child(L"password").text().as_string());
					accounts.AddAccount(account);
				}
				catch (...)
				{
				}
			}
		}
	}	
	return std::move(accounts);
}

void FarmAccountsStorage::Save(const FarmAccounts& accounts)
{

	pugi::xml_document doc;

	pugi::xml_node xmlaccounts = doc.append_child(L"accounts");
	PtrEnumerator<const FarmAccount> accountEnum = accounts.EnumAccounts();
	while(accountEnum.hasNext())
	{
		const FarmAccount* account = *accountEnum;

		pugi::xml_node xmlaccount = xmlaccounts.append_child(L"account");

		xmlaccount.append_child(L"name").text().set(account->GetName().c_str());
		xmlaccount.append_child(L"address").text().set(account->GetAddress().c_str());
		xmlaccount.append_child(L"port").text().set(account->GetPort());
		xmlaccount.append_child(L"login").text().set(account->GetLogin().c_str());
		xmlaccount.append_child(L"password").text().set(account->GetPassword().c_str());
		xmlaccount.append_child(L"farmno").text().set(account->GetFarm().ToString().c_str());
		
		accountEnum.advance();
	}

	std::wstring storageDir = GetStorageDirectory();
	paths_EnsurePathExist(storageDir);
	std::wstring storageFile = paths_ConcatPath(storageDir, ACCOUNTS_FILE);
		
	doc.save_file(storageFile.c_str());

}
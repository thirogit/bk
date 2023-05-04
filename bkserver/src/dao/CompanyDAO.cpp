#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if 0
bool RepoAccountDAO::LoadRepoAccounts(PtrFlushArray<RepoAccount>& result,SimpleProgress *progress/* = NULL*/)
{
	SQLSelect res;
	RepoAccount *oneRow = NULL;
	int colNAME,colADDRESS,colLOGIN,colPASSWORD,colACCOUNTID;

	int step = 0;
	int steps = res.SelectScalar("SELECT COUNT(*) FROM repo_accounts",0,-1);
	if(steps < 0) return false;
	if(progress) progress->SetSteps(steps);

	result.RemoveAll();

	try
	{
		
		res.Select("SELECT * FROM repo_accounts");
		colACCOUNTID = res.ColIndex("ACCOUNT_ID");
		colNAME = res.ColIndex("ACCOUNT_NAME");
		colADDRESS = res.ColIndex("REPO_ADDRESS");
		colLOGIN = res.ColIndex("REPO_LOGIN");
		colPASSWORD = res.ColIndex("REPO_PASSWORD");
	
		while(res.nextRow())
		{	
		    oneRow =  new RepoAccount(res.getColInt(colACCOUNTID));
			
			oneRow->SetAddress(res.getColStr(colADDRESS));
			oneRow->SetName(res.getColStr(colNAME));
			oneRow->SetLogin(res.getColStr(colLOGIN));
			oneRow->SetPassword(res.getColStr(colPASSWORD));
			
			result.Add(oneRow);
			oneRow = NULL;
			if(progress && !progress->Step()) return false;
		}
	}
	catch (SQLException e)
	{
		if(oneRow) delete oneRow;
		BOOST_THROW_EXCEPTION(e);
	}
	return true;
}

void RepoAccountDAO::UpdateRepoAccount(Transaction transaction,const RepoAccount* pAccount)
{
	SQLQuery stmt;
	stmt << "UPDATE repo_accounts SET account_name = %0q,repo_address = %1q,repo_login = %2q,repo_password = %3q WHERE account_id = %4;";
	stmt.parse();
	
  
	LOG("UpdateRepoAccount() stmt: %s",stmt.str((LPCSTR)pAccount->GetName(),
											 (LPCSTR)pAccount->GetAddress(),
											 (LPCSTR)pAccount->GetLogin(),
											 (LPCSTR)pAccount->GetPassword(),
											 pAccount->GetId()).c_str());

	transaction->Update(stmt.str((LPCSTR)pAccount->GetName(),
							 (LPCSTR)pAccount->GetAddress(),
							 (LPCSTR)pAccount->GetLogin(),
							 (LPCSTR)pAccount->GetPassword(),
							 pAccount->GetId()).c_str());
}

void RepoAccountDAO::DeleteRepoAccount(Transaction transaction,const RepoAccount* pAccount)
{
	SQLQuery stmt;
	stmt << "DELETE FROM repo_accounts WHERE account_id = %0;";
	stmt.parse();
	
  
	LOG("DeleteRepoAccount() stmt: %s",stmt.str(pAccount->GetId()).c_str());

	transaction->Update(stmt.str(pAccount->GetId()).c_str());
}

RepoAccount* RepoAccountDAO::InsertRepoAccount(Transaction transaction,const RepoAccount* pAccount)
{
	SQLQuery q;
	q << "INSERT INTO repo_accounts(account_name,repo_address,repo_login,repo_password) VALUES (%0q,%1q,%2q,%3q);";
	q.parse();
	
  
	LOG("InsertRepoAccount() stmt: %s",q.str((LPCSTR)pAccount->GetName(),
											 (LPCSTR)pAccount->GetAddress(),
											 (LPCSTR)pAccount->GetLogin(),
											 (LPCSTR)pAccount->GetPassword()).c_str());

	transaction->Insert(q.str((LPCSTR)pAccount->GetName(),
							 (LPCSTR)pAccount->GetAddress(),
							 (LPCSTR)pAccount->GetLogin(),
							 (LPCSTR)pAccount->GetPassword()).c_str());

	SQLSelect maxIdQ(transaction);

	int newAccountId = maxIdQ.SelectScalar("SELECT MAX(account_id) FROM repo_accounts",1);

	RepoAccount* pNewAccount = new RepoAccount(newAccountId);
	pNewAccount->CopyFrom(*pAccount);
	return pNewAccount;


	
}
#endif

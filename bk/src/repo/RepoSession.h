#ifndef __REPOSESSION_H__
#define __REPOSESSION_H__

#include "../context/SeasonSession.h"
#include <progress\SimpleProgress.h>

class RepoSession
{		
	
public:
	RepoSession(SeasonSession* seasonSession);
	~RepoSession();
	
	void Open();
	void Close();
	PurchaseHeaders QueryPurchases(const DateTime& strDt, const DateTime& endDt, SimpleProgress* progress);
	boost::optional<Purchase> GetPurchase(const std::wstring& purchaseId);
	DownloadPurchaseTransactionPtr DownloadPurchase(const std::wstring& purchaseId);

private:
	SeasonSession* m_pSeasonSession;

};
	

#endif
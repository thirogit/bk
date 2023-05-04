#ifndef __DOWNLOADPURCHASETRANSACTION_H__
#define __DOWNLOADPURCHASETRANSACTION_H__

#include "ITransactionInterface.h"
#include <data\runtime\PurchaseDownloadIssue.h>
#include <CommonTypes.h>
#include <data\repo\Purchase.h>
#include <boost\shared_ptr.hpp>
#include <data\runtime\SolutionType.h>
#include <data\runtime\SolutionParameters.h>
#include <data\runtime\ItemIndex.h>

class DownloadPurchaseTransaction
{
public:
	DownloadPurchaseTransaction(ITransactionInterface* pInterface);
	~DownloadPurchaseTransaction();
	
	count_t GetIssueCount();
	IPurchaseDownloadIssue* GetIssueAt(int index);
	IPurchaseDownloadIssue* GetIssue(ItemId id);
	const IPurchase* GetPurchase() const;
	
	void SolveIssue(ItemId id, const SolutionParameters& parameters);
	void SolveIssueCategory(const PurchaseDownloadIssueCategory& category,const SolutionParameters& parameters);
	void SetDocDate(const DateTime& docDate);
	void Rollback();
	void Commit();
	void Open();

protected:
	void FetchIssues();
	void Clean();
private:	
	ITransactionInterface* m_pInterface;
	Purchase* m_purchase;
	ItemIndex<PurchaseDownloadIssue> m_issues;
};

typedef boost::shared_ptr<DownloadPurchaseTransaction> DownloadPurchaseTransactionPtr;

#endif
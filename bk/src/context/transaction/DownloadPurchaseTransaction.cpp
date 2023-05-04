#include <stdafx.h>
#include "DownloadPurchaseTransaction.h"
#include <data\runtime\PurchaseDownloadOperations.h>
#include <data\runtime\PurchaseDownloadIssues.h>
#include <arrays/DeepDelete.h>

DownloadPurchaseTransaction::DownloadPurchaseTransaction(ITransactionInterface* pInterface) : m_pInterface(pInterface)
{
	m_purchase = NULL;
}

DownloadPurchaseTransaction::~DownloadPurchaseTransaction()
{		
	Clean();
	delete m_pInterface;
}

void DownloadPurchaseTransaction::Clean()
{
	delete m_purchase;
	m_purchase = NULL;
	DeepDelete(m_issues);
	m_issues.clear();
}

void DownloadPurchaseTransaction::Rollback()
{	
	m_pInterface->Rollback();
	Clean();
}

void DownloadPurchaseTransaction::Commit()
{	
	m_pInterface->Commit();
}

count_t DownloadPurchaseTransaction::GetIssueCount()
{
	return m_issues.size();
}

IPurchaseDownloadIssue* DownloadPurchaseTransaction::GetIssueAt(int index)
{
	return m_issues.item_at(index);
}

IPurchaseDownloadIssue* DownloadPurchaseTransaction::GetIssue(ItemId id)
{
	return m_issues.find_item(id);
}

void DownloadPurchaseTransaction::FetchIssues()
{
	DeepDelete(m_issues);
	m_issues.clear();

	PurchaseDownloadIssues issues(std::move(boost::any_cast<PurchaseDownloadIssues>(m_pInterface->Execute(PurchaseDownloadOperations::Operation_GetIssues))));

	PtrEnumerator<IPurchaseDownloadIssue> issueEnum = issues.EnumIssues();
	while (issueEnum.hasNext())
	{
		m_issues.add_item(new PurchaseDownloadIssue(*issueEnum));
		issueEnum.advance();
	}
}

void DownloadPurchaseTransaction::Open()
{
	m_purchase = new Purchase(boost::any_cast<Purchase>(m_pInterface->Execute(PurchaseDownloadOperations::Operation_GetPurchase)));
	FetchIssues();	
}

const IPurchase* DownloadPurchaseTransaction::GetPurchase() const
{
	return m_purchase;
}

void DownloadPurchaseTransaction::SolveIssue(ItemId id, const SolutionParameters& parameters)
{
	m_pInterface->Execute(PurchaseDownloadOperations::Operation_SolveSingleIssue, OperationArguments::pack(id,parameters));
	FetchIssues();
}

void DownloadPurchaseTransaction::SolveIssueCategory(const PurchaseDownloadIssueCategory& category, const SolutionParameters& parameters)
{
	m_pInterface->Execute(PurchaseDownloadOperations::Operation_SolveCategoryIssue, 
		OperationArguments::pack(category, parameters));
	FetchIssues();
}

void DownloadPurchaseTransaction::SetDocDate(const DateTime& docDate)
{
	m_pInterface->Execute(PurchaseDownloadOperations::Operation_SetDocDate,
		OperationArguments::pack(docDate));
}
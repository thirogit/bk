#pragma once

#include "../IClientTransaction.h"
#include <data/repo/Purchase.h>

#include "../ISeasonInterface.h"
#include "../../SeasonSessionFacade.h"
#include "../../repo/RepoClient.h"
#include <data/runtime/ItemIndex.h>
#include "IntegrityProblem.h"
#include "../../log/LogInterface.h"
#include "../../log/LoggerAware.h"

class DownloadPurchaseTransaction : public IClientTransaction, public LoggerAware
{
public:
	DownloadPurchaseTransaction(ISeasonInterface* pInterface, 
								LogInterface* logger,
								repo::RepoClient* repoClient,
								uint32_t transactionId);

	~DownloadPurchaseTransaction();
	void Open(const std::wstring& purchaseId);
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args);
	virtual void Commit();
	virtual void Abort();
	virtual uint32_t GetId() const;

private:
	void IdentifyIntegrityIssues(SeasonSessionFacade* facade);
	bool IsSolved(IntegrityProblem* problem);
	bool IsAlreadyIdentified(IntegrityProblem* problem);
	bool DoesStillExist(IntegrityProblem* problem, SeasonSessionFacade* facade);
	void SolveIssueCategory(const PurchaseDownloadIssueCategory& category, SolutionParameters& parameters);
	
	void RemoveObsoleteSolutions();
	bool IsObsolete(Solution* solution);
	void Cleanup();
	void SolveIssue(ItemId id, SolutionParameters& parameters);
	IntegrityProblem* FindProblem(ItemId id);
	void ApplySolutions(Purchase* purchaseToFix);
	void ThrowNotAllIssuesSolved();
	void ThrowIntegrityError();
	void LogProblems();
protected:
	Purchase* m_Purchase;	
	uint32_t m_TransactionId;	
	ISeasonInterface* m_pInterface;
	repo::RepoClient* m_pRepoClient;
	ItemIndex<IntegrityProblem> m_problems;
	std::vector<Solution*> m_solutions;
	DateTime m_docDate;
	
};

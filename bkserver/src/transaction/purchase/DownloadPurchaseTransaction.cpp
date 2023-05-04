#include "stdafx.h"
#include "DownloadPurchaseTransaction.h"
#include <boost/range/adaptor/map.hpp>
#include <boost/foreach.hpp>
#include "../../ClientSeasonSession.h"
#include "../../SeasonSession.h"
#include <data/runtime/PurchaseDownloadOperations.h>
#include <functional>
#include <arrays/DeepDelete.h>
#include "../TransactionException.h"
#include "../../repo/RepoException.h"
#include <errortoken/ErrorToken.h>
#include <data/runtime/PurchaseDownloadIssues.h>
#include <boost/foreach.hpp>
#include "AgentIntegrityProblem.h"
#include "HerdIntegrityProblem.h"
#include "StockIntegrityProblem.h"
#include "ClassIntegrityProblem.h"
#include "InvoiceHentIntegrityProblem.h"
#include "FirstOwnerHentIntegrityProblem.h"
#include "ProblemVerifingVisitor.h"
#include "IntegrityProblemIdentifier.h"
#include "SolutionApplyingVisitor.h"
#include "../../SessionException.h"
#include "PurchaseTraverser.h"

DownloadPurchaseTransaction::DownloadPurchaseTransaction(ISeasonInterface* pInterface, 
														 LogInterface* logger,
														 repo::RepoClient* repoClient,
														 uint32_t transactionId) : LoggerAware(logger)
{
	m_Purchase = NULL;
	m_pRepoClient = repoClient;
	m_pInterface = pInterface;
}

DownloadPurchaseTransaction::~DownloadPurchaseTransaction()
{
	Cleanup();
}

void DownloadPurchaseTransaction::Cleanup()
{
	DeepDelete(m_problems);
	m_problems.clear();
	delete m_Purchase;
	m_Purchase = NULL;
}

void DownloadPurchaseTransaction::Open(const std::wstring& purchaseId)
{
	Log(LogLevel_Info,L"opening download purchase transaction for purchase with id = %s", purchaseId);
	try
	{
		Log(LogLevel_Debug, L"retreiving purchase with id = %s", purchaseId);
		repo::DubiousPurchase purchase = m_pRepoClient->GetPurchase(purchaseId);
		if (!purchase.is_initialized())
		{
			Log(LogLevel_Debug, L"there is no purchase with id = %s, throwing", purchaseId);
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"repo_no_purchase")(L"id", purchaseId).str()));
		}
		else
		{
			m_Purchase = new Purchase(purchase.get_ptr());
		}
	}
	catch (repo::RepoException& e)
	{
		Log(LogLevel_Error, L"caught repo error: %s, rethrowing", e.message());
		BOOST_THROW_EXCEPTION(TransactionException(e.message()));
	}	

	Log(LogLevel_Debug, L"running integrity check for purchase with id = %s", purchaseId);

	std::function<void(SeasonSessionFacade*)> integrityCheck = std::bind(&DownloadPurchaseTransaction::IdentifyIntegrityIssues, this, std::placeholders::_1);
	m_pInterface->Do(integrityCheck);	

	Log(LogLevel_Info, L"identified %d problems", m_problems.size());
	LogProblems();
}

void DownloadPurchaseTransaction::LogProblems()
{
	if (m_problems.size() > 0)
	{
		std::wstringstream m;
		m << L"Problems:\n";
		int index = 1;
		BOOST_FOREACH(IntegrityProblem* problem, m_problems)
		{
			m << index << L": " <<  problem->DescribeProblem() << L" @ " << problem->GetPath().ToString() << L", id=" << ItemIdText::ToString(problem->GetId()) << L"\n";
			index++;
		}
		Log(LogLevel_Debug, m.str());
	}
}

void DownloadPurchaseTransaction::RemoveObsoleteSolutions()
{
	std::vector<Solution*> validSolutions;
	BOOST_FOREACH(Solution* solution, m_solutions)
	{
		if (!IsObsolete(solution))
		{
			validSolutions.push_back(solution);
		}
		else
		{
			delete solution;
		}		
	}
	m_solutions.clear();
	std::copy(validSolutions.begin(), validSolutions.end(), std::back_inserter(m_solutions));
}

bool DownloadPurchaseTransaction::IsObsolete(Solution* solution)
{
	BOOST_FOREACH(IntegrityProblem* problem, m_problems)
	{
		if (solution->IsSolving(problem))
			return false;
	}

	return true;
}

void DownloadPurchaseTransaction::IdentifyIntegrityIssues(SeasonSessionFacade* facade)
{
	std::vector<IntegrityProblem*> validProblems;

	IntegrityProblemIdentifier identifier(m_Purchase);
	identifier.IdentifyProblems(facade);

	IntegrityProblem* problem;
	PtrEnumerator<IntegrityProblem> problemEnum = identifier.EnumProblems();
	while (problemEnum.hasNext())
	{
		problem = *problemEnum;
		if (!IsAlreadyIdentified(problem))
		{
			validProblems.push_back(problem->Clone());
		}
		problemEnum.advance();
	}

	BOOST_FOREACH(IntegrityProblem* problem, m_problems)
	{
		if (DoesStillExist(problem, facade))
		{
			validProblems.push_back(problem);
		}
		else
		{
			delete problem;
		}
	}

	m_problems.clear();

	BOOST_FOREACH(IntegrityProblem* problem, validProblems)
	{
		m_problems.add_item(problem);
	}

	RemoveObsoleteSolutions();	
}

boost::any DownloadPurchaseTransaction::ExecuteOperation(uint16_t operation, const OperationArguments& args)
{
	switch(operation)
	{
		case PurchaseDownloadOperations::Operation_GetPurchaseId:
			return m_Purchase->GetId();
		
		case PurchaseDownloadOperations::Operation_GetIssues:
		{
			PurchaseDownloadIssues issues;
			BOOST_FOREACH(IntegrityProblem* problem, m_problems)
			{
				if (!IsSolved(problem))
				{
					PurchaseDownloadIssue issue = problem->ToIssue();
					issues.Add(&issue);
				}
			}
			return std::move(issues);
		}		
		
		case PurchaseDownloadOperations::Operation_SolveSingleIssue:
		{
			ItemId id = args.get<ItemId>(0);
			SolutionParameters parameters = args.get<SolutionParameters>(1);			
			SolveIssue(id, parameters);
		}
		break;
		
		case PurchaseDownloadOperations::Operation_GetPurchase:
			return *m_Purchase;

		case PurchaseDownloadOperations::Operation_SolveCategoryIssue:
		{
			PurchaseDownloadIssueCategory category = args.get<PurchaseDownloadIssueCategory>(0);
			SolutionParameters parameters = args.get<SolutionParameters>(1);
			SolveIssueCategory(category, parameters);
			break;
		}
		case PurchaseDownloadOperations::Operation_SetDocDate:
		{
			DateTime dt = args.get<DateTime>(0);
			m_docDate = dt;
			break;
		}
	}
	return boost::any();
}


bool DownloadPurchaseTransaction::IsSolved(IntegrityProblem* problem)
{
	BOOST_FOREACH(Solution* solution, m_solutions)
	{
		if (solution->IsSolving(problem))
			return true;
	}
	return false;
}

bool DownloadPurchaseTransaction::DoesStillExist(IntegrityProblem* problem,SeasonSessionFacade* facade)
{
	ProblemVerifingVisitor visitor(facade);
	problem->Accept(&visitor);
	return visitor.IsVerified();	
}

bool DownloadPurchaseTransaction::IsAlreadyIdentified(IntegrityProblem* newProblem)
{
	BOOST_FOREACH(IntegrityProblem* existingProblem, m_problems)
	{
		if (existingProblem->IsSameAs(newProblem))
			return true;
	}
	return false;
}

void DownloadPurchaseTransaction::SolveIssueCategory(const PurchaseDownloadIssueCategory& category, SolutionParameters& parameters)
{
	std::vector<IntegrityProblem*> problemsOfCategory;
	BOOST_FOREACH(IntegrityProblem* problem, m_problems)
	{
		if (problem->GetCategory().IsSame(category))
		{
			problemsOfCategory.push_back(problem);
		}
	}

	if (!problemsOfCategory.empty())
	{
		IntegrityProblem* problem = problemsOfCategory[0];
		Solution* solution = problem->Solve(parameters);
		Solution* genericSolution = solution->Generalize();
		delete solution;

		if (genericSolution->IsImmediate())
		{
			SolutionApplyingVisitor visitor(m_Purchase);
			genericSolution->Accept(&visitor);
			delete genericSolution;


			std::for_each(problemsOfCategory.begin(), problemsOfCategory.end(), [this](IntegrityProblem* problem)
																				{
																					m_problems.remove_item(problem->GetId());																					
																				});
			DeepDelete(problemsOfCategory);

		}
		else
		{
			m_solutions.push_back(genericSolution);
		}		
	}
}

void DownloadPurchaseTransaction::SolveIssue(ItemId id, SolutionParameters& parameters)
{
	IntegrityProblem* problem = m_problems.find_item(id);
	if (problem)
	{
		if (parameters.type() == SolutionType_Add)
		{
			Log(LogLevel_Info, L"solving problem " + ItemIdText::ToString(id) + L" by adding [category soution]");
			SolveIssueCategory(problem->GetCategory(), parameters);
		}
		else
		{
			Log(LogLevel_Info, L"solving problem " + ItemIdText::ToString(id));
			Solution* solution = problem->Solve(parameters);

			if (solution->IsImmediate())
			{
				SolutionApplyingVisitor visitor(m_Purchase);
				solution->Accept(&visitor);
				delete solution;

				m_problems.remove_item(id);
				delete problem;
			}
			else
			{
				m_solutions.push_back(solution);
			}
		}	

		m_pInterface->DoAny(std::bind(&DownloadPurchaseTransaction::IdentifyIntegrityIssues, this, std::placeholders::_1));
	}
}

uint32_t DownloadPurchaseTransaction::GetId() const
{
	return m_TransactionId;
}

void DownloadPurchaseTransaction::ApplySolutions(Purchase* purchaseToFix)
{
	BOOST_FOREACH(Solution* solution, m_solutions)
	{
		SolutionApplyingVisitor visitor(purchaseToFix);
		solution->Accept(&visitor);
	}
}

void DownloadPurchaseTransaction::ThrowNotAllIssuesSolved()
{
	BOOST_THROW_EXCEPTION(TransactionException(L"repo_not_all_issues_solved"));
}

void DownloadPurchaseTransaction::ThrowIntegrityError()
{
	BOOST_THROW_EXCEPTION(TransactionException(L"repo_integrity_error"));
}

void DownloadPurchaseTransaction::Commit()
{
	Log(LogLevel_Info, L"commiting purchase download transaction");

	const std::wstring purchaseId = m_Purchase->GetId();

	try
	{
		m_pInterface->DoAny([this, purchaseId](SeasonSessionFacade* facade)
		{
			Log(LogLevel_Debug, L"running integrity recheck");
			IdentifyIntegrityIssues(facade);

			BOOST_FOREACH(IntegrityProblem* problem, m_problems)
			{
				if (!IsSolved(problem))
				{
					Log(LogLevel_Debug, L"unsolved problem: %s %s @ %s", ItemIdText::ToString(problem->GetId()), problem->DescribeProblem(), problem->GetPath().ToString());
					ThrowNotAllIssuesSolved();
				}
			}

			Purchase fixedPurchase(purchaseId);
			fixedPurchase.CopyFrom(m_Purchase);

			Log(LogLevel_Debug, L"appling solutions to purchase");
			ApplySolutions(&fixedPurchase);

			Log(LogLevel_Debug, L"inserting purchase with id=%s and docdate=%s", purchaseId,TextUtils::IfEmpty(m_docDate.GetDayDate(),L"null"));
			facade->InsertPurchase(&fixedPurchase,m_docDate);
		});

		Log(LogLevel_Debug, L"marking purchase(id=%s) as downloaded", purchaseId);
		m_pRepoClient->MarkPurchase(purchaseId,true);

		Cleanup();
	}
	catch (SessionException& e)
	{
		BOOST_THROW_EXCEPTION(TransactionException(e.message()));
	}
		
	
}

void DownloadPurchaseTransaction::Abort()
{
	Cleanup();
}



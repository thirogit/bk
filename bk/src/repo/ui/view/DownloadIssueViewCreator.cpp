#include "stdafx.h"
#include "DownloadIssueViewCreator.h"
#include <utils/SafeGet.h>
#include <boost/assign.hpp>
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <mustache/Mustache.h>
#include <errortoken/ErrorToken.h>
#include <boost/foreach.hpp>

DownloadIssueViewCreator::DownloadIssueViewCreator()
{

	m_issueCodeToMsg = {

		{L"repo_issue_no_agent", IDS_ISSUE_NOAGENT},
		{L"repo_issue_no_herd", IDS_ISSUE_NOHERD },
		{L"repo_issue_no_invoice_hent", IDS_ISSUE_NOINVOICEHENT},
		{L"repo_issue_no_class", IDS_ISSUE_NOCLASS},
		{L"repo_issue_no_stock", IDS_ISSUE_STOCK},
		{L"repo_issue_no_firstowner", IDS_ISSUE_NOFSTOWNERHENT},
		{L"repo_issue_herd_cow_duplication", IDS_ISSUE_HERDCOWDUPLICATION}
		}
	;
}

void DownloadIssueViewCreator::RefreshView(DownloadIssueView* pView, const IPurchaseDownloadIssue* pIssue)
{
	pView->SetDomain(pIssue->GetCategory().GetDomain());

	try
	{
		ErrorToken errorToken = ErrorToken::tokenize(pIssue->GetError());
		std::wstring errorCd = errorToken.code();

		auto it = m_issueCodeToMsg.find(errorCd);
		if (it != m_issueCodeToMsg.end())
		{
			std::wstring errorMsg = StringLoader(it->second);
			Mustache mustache = Mustache::Parse(errorMsg);
			MustacheVariables variables;

			BOOST_FOREACH(Token token, errorToken)
			{
				variables.Put(token.name, token.value);
			}

			pView->SetMessage(mustache.Format(variables));
		}
		else
		{
			pView->SetMessage(pIssue->GetError());
		}
	}
	catch (ErrorTokenException& e)
	{
		pView->SetMessage(pIssue->GetError());
	}
}

DownloadIssueView* DownloadIssueViewCreator::CreateView(const IPurchaseDownloadIssue* pIssue)
{
	DownloadIssueView* pView = new DownloadIssueView(pIssue->GetId());
	RefreshView(pView, pIssue);
	return pView;
}

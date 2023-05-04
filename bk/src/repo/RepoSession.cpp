#include "stdafx.h"
#include "RepoSession.h"
#include <boost/foreach.hpp>
#include <progress\SafeSimpleProgress.h>
#include "../context/ContextException.h"
#include "RepoException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


RepoSession::RepoSession(SeasonSession* seasonSession) : m_pSeasonSession(seasonSession)	
{
	
}

RepoSession::~RepoSession()
{
	
}

void RepoSession::Open()
{
	try
	{
		m_pSeasonSession->OpenPurchaseRepository();
	}
	catch (ContextException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(e.message()));
	}
}


void RepoSession::Close()
{	
	try
	{
		m_pSeasonSession->ClosePurchaseRepository();
	}
	catch (ContextException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(e.message()));
	}
}

PurchaseHeaders RepoSession::QueryPurchases(const DateTime& strDt, const DateTime& endDt, SimpleProgress* pProgress)
{
	try
	{
		SafeSimpleProgress progress(pProgress);
		int chunks = m_pSeasonSession->QueryPurchases(strDt, endDt);
		progress.SetSteps(chunks);

		PurchaseHeaders result;
		while (true)
		{
			PurchaseHeaders chunk = m_pSeasonSession->FetchPurchaseHeaders();

			BOOST_FOREACH(IPurchaseHeader* purchaseHeader, chunk)
			{
				result.Add(purchaseHeader);
			}

			if (chunk.Count() == 0)
				break;
		}

		return std::move(result);
	}
	catch (ContextException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(e.message()));
	}

}

boost::optional<Purchase> RepoSession::GetPurchase(const std::wstring& purchaseId)
{
	try
	{
		return m_pSeasonSession->FetchPurchase(purchaseId);
	}
	catch (ContextException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(e.message()));
	}
}


DownloadPurchaseTransactionPtr RepoSession::DownloadPurchase(const std::wstring& purchaseId)
{
	return m_pSeasonSession->OpenDownloadPurchaseTransaction(purchaseId);
}
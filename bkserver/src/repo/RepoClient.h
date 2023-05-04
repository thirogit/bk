#ifndef __REPOCLIENT_H__
#define __REPOCLIENT_H__

#include <set>
#include <types/DateTime.h>
#include <data/types/FarmNo.h>
#include <data/repo/Purchases.h>
#include "CursorDescriptor.h"
#include "../cloud/HttpClient.h"
#include <url.h>
#include "../cloud/Auth0Account.h"
#include "../cloud/Auth0TokenProvider.h"
#include "Cursor.h"
#include <data/repo/PurchaseHeaders.h>
#include <casajson/json.h>

namespace repo 
{ 
	typedef Cursor<PurchaseHeaders> PurchaseHeaderCursor;
	typedef boost::shared_ptr<PurchaseHeaderCursor> PurchaseHeaderCursorPtr;
	typedef boost::optional<Purchase> DubiousPurchase;


	class RepoClientFactory;

	class RepoClient 
	{

		class PurchaseHeaderCursorImpl : public PurchaseHeaderCursor
		{
		public:
			PurchaseHeaderCursorImpl(CursorDescriptor& descriptor, RepoClient* pClient);
			virtual PurchaseHeaders FetchMany(int howMany);
			virtual ~PurchaseHeaderCursorImpl();
		private:
			RepoClient * m_pClient;
		};

	public:
	
		virtual ~RepoClient();

		PurchaseHeaderCursorPtr QueryPurchases(const DateTime& startDt, const DateTime& endDt);
		void MarkPurchase(const std::wstring& purchaseId, bool downloaded);
		DubiousPurchase GetPurchase(const std::wstring& purchaseId);

	private:
		RepoClient(const Url::Url& repoUrl,const Auth0Account& credentials, const FarmNo& farmNo);
		
		json::value MakePostCall(const std::string& resource,const json::value& rq);
		json::value MakeGetCall(const std::string& resource, const std::map<std::string, std::string>& headers, const std::map<std::string, std::string>& parameters);
		std::map<std::string, std::string> CreateHeaders();
		
		PurchaseHeaders FetchPurchaseHeaders(const std::wstring&cursorId, int howMany);

		CursorDescriptor CallQueryPurchases(const DateTime& startDt, const DateTime& endDt);
		
		void SetPurchaseDeduction(const json::value& jsonDeductions, PurchaseInvoiceDeduction* deduction);
		void SetPurchaseInvoice(const json::value& jsonInvoice, PurchaseInvoice* invoice);
		void SetPurchaseHent(const json::value& jsonValue, PurchaseHent* hent);
		void SetPurchaseCow(const json::value& jsonCow, PurchaseCow* cow);
		void SetPurchase(const json::value& jsonPurchase, Purchase* purchase);
		void SetPurchaseHeader(const json::value& jsonPurchase, PurchaseHeader* purchaseHeader);
			
	private:	
		HttpClient m_httpClient;
		Url::Url m_repoUrl;
		Auth0Account m_credentials;
		Auth0TokenProvider m_tokenProvider;
		FarmNo m_farmNo;

		friend class RepoClientFactory;
	};

}
#endif

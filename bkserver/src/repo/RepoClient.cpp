#include "stdafx.h"
#include "RepoClient.h"
#include "RepoException.h"
#include <string/TextUtils.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <data/payways/PayWays.h>
#include <data/invoicetypes/InvoiceTypes.h>
#include <data/henttypes/HentTypes.h>
#include <utils/JsonUtils.h>
#include <boost/assign/list_of.hpp>
#include <boost/foreach.hpp>

using namespace boost::assign;

namespace repo 
{


//#######################################################################################
RepoClient::PurchaseHeaderCursorImpl::PurchaseHeaderCursorImpl(CursorDescriptor& descriptor, RepoClient* pClient) :
	m_pClient(pClient),
	PurchaseHeaderCursor(descriptor.GetCursorId(),descriptor.GetCount(), CursorFetchLimit(descriptor.GetMinFetch(), descriptor.GetMaxFetch()))
{
}

RepoClient::PurchaseHeaderCursorImpl::~PurchaseHeaderCursorImpl()
{	
}

PurchaseHeaders RepoClient::PurchaseHeaderCursorImpl::FetchMany(int howMany)
{
	return std::move(m_pClient->FetchPurchaseHeaders(GetId(),howMany));
}
//#######################################################################################


RepoClient::RepoClient(const Url::Url& repoUrl,  const Auth0Account& credentials, const FarmNo& farmNo) : 
	m_repoUrl(repoUrl), m_credentials(credentials), m_tokenProvider(credentials), m_farmNo(farmNo)
{
}

RepoClient::~RepoClient()
{	
}

json::value RepoClient::MakePostCall(const std::string& resource,const json::value& rq)
{	
	Url::Url url = m_repoUrl / resource;
	CallResponsePtr response = m_httpClient.MakePostCall(url, CreateHeaders(), TextUtils::ToUTF8(rq.to_string()));

	if (response->result_int() != 200)
	{	
		BOOST_THROW_EXCEPTION(RepoException(TextUtils::Format(L"error posting to resource: %s , result: %d, reason: %s", w(resource), response->result_int(), w(response->reason().to_string()))));
	}

	auto body = response->body();
	if (!body.empty())
	{
		return json::value::parse(w(body));
	}
	return json::value::null();
}

std::map<std::string, std::string> RepoClient::CreateHeaders()
{
	try
	{
		//Auth0Token token = m_tokenProvider.ObtainToken();
		//return map_list_of<std::string, std::string>("Authorization", (token.token_type + " " + token.token))( "Content-Type", "application/json" );
		return map_list_of<std::string, std::string>("Content-Type", "application/json");
	}
	catch (std::runtime_error& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(w(e.what())));
	}
}

json::value RepoClient::MakeGetCall(const std::string& resource, const std::map<std::string, std::string>& headers,const std::map<std::string, std::string>& parameters)
{
	typedef const std::map<std::string, std::string> map_t;

	Url::Url url = m_repoUrl / resource;
	

	BOOST_FOREACH(const map_t::value_type& entry, parameters)
	{
		url &= entry;
	}		

	CallResponsePtr response = m_httpClient.MakeGetCall(url, CreateHeaders());

	if (response->result_int() != 200)
	{
		BOOST_THROW_EXCEPTION(RepoException(TextUtils::Format(L"error getting resource: %s , result: %d, reason: %s",w(resource), response->result_int() , w(response->reason().to_string()))));
	}

	return json::value::parse(w(response->body()));
}

PurchaseHeaderCursorPtr RepoClient::QueryPurchases(const DateTime& startDt, const DateTime& endDt)
{
	CursorDescriptor cursorDescriptor = CallQueryPurchases(startDt, endDt);
	PurchaseHeaderCursorImpl* pCursor = new PurchaseHeaderCursorImpl(cursorDescriptor, this);
	return PurchaseHeaderCursorPtr(pCursor);
}


void RepoClient::MarkPurchase(const std::wstring& purchaseId, bool downloaded)
{
	json::value parameters;
	parameters[_U("purchaseid")] = json::value(purchaseId);
	parameters[_U("downloaded")] = json::value(downloaded);
	parameters[_U("farmno")] = json::value(m_farmNo.ToString());
	json::value jsonresult = MakePostCall("markpurchase", parameters);
}

CursorDescriptor RepoClient::CallQueryPurchases(const DateTime& startDt, const DateTime& endDt)
{
	json::value parameters;	
	parameters[_U("fromdt")] = json::value(uint64_t(startDt.SecondsSinceEpoh()));
	parameters[_U("todt")] = json::value(uint64_t(endDt.SecondsSinceEpoh()));
	parameters[_U("farmno")] = json::value(m_farmNo.ToString());
	json::value jsonresult = MakePostCall("createpurchasecursor", parameters);

	CursorDescriptor result(jsonresult[_U("cursorid")].as_string());
	result.SetCount(jsonresult[_U("count")].as_integer());
	result.SetMaxFetch(jsonresult[_U("maxfetch")].as_integer());
	result.SetMinFetch(jsonresult[_U("minfetch")].as_integer());

	return result;	
}


PurchaseHeaders RepoClient::FetchPurchaseHeaders(const std::wstring& cursorId,int howMany)
{
	PurchaseHeaders result;
	json::value jsonresult = MakeGetCall("fetchpurchases", CreateHeaders(), map_list_of<std::string, std::string>("cursorid",u(cursorId)));

	if (jsonresult.is_array())
	{
		json::array jsonPurchaseHeaders = jsonresult.as_array();
		for (int i = 0, count = jsonPurchaseHeaders.size(); i < count; i++)
		{
			json::value jsonPurchaseHeader = jsonPurchaseHeaders[i];
			PurchaseHeader* purchaseHdr = result.Add(jsonPurchaseHeader[_U("id")].as_string());
			SetPurchaseHeader(jsonPurchaseHeader, purchaseHdr);
		}
	}
	return std::move(result);
	
}


DubiousPurchase RepoClient::GetPurchase(const std::wstring& purchaseId)
{
	DubiousPurchase result;

	std::map<std::string, std::string> parameters = map_list_of<std::string, std::string>("purchaseid", u(purchaseId))
		("farmno", u(m_farmNo.ToString()));

	json::value jsonresult = MakeGetCall("fetchpurchase", CreateHeaders(), parameters);

	if (!jsonresult.is_null())
	{
		result = Purchase(purchaseId);	

		try
		{
			SetPurchase(jsonresult, result.get_ptr());
		}
		catch (wexception& e)
		{
			BOOST_THROW_EXCEPTION(RepoException(L"error fetching purchase " + purchaseId + L": " + e.message()));
		}
	}
	return std::move(result);
}


void RepoClient::SetPurchase(const json::value& jsonPurchase, Purchase* purchase)
{	
	purchase->SetAgentCode(json_nullToEmpty(j(jsonPurchase)[_U("agentCd")]));
	purchase->SetEndDate(json_DateTime(j(jsonPurchase)[_U("endDt")]));
	purchase->SetStartDate(json_DateTime(j(jsonPurchase)[_U("startDt")]));
	purchase->SetPlateNo(json_nullToEmpty(j(jsonPurchase)[_U("plateNo")]));
	//purchase->SetWasDownloaded(j(jsonPurchase)[_U("wasDownloaded")).as_bool());
	purchase->SetHerd(j(jsonPurchase)[_U("herdNo")].as_integer());

	json::array jsonInvoices = j(jsonPurchase)[_U("invoices")].as_array();

	for (int j = 0, invoiceCount = jsonInvoices.size(); j < invoiceCount; j++)
	{
		PurchaseInvoice* invoice = purchase->AddInvoice();
		SetPurchaseInvoice(jsonInvoices[j], invoice);
	}

	json::array jsonHents = j(jsonPurchase)[_U("purchaseHents")].as_array();

	for (int k = 0, hentsCount = jsonHents.size(); k < hentsCount; k++)
	{
		PurchaseHent* hent = purchase->AddHent();
		SetPurchaseHent(jsonHents[k], hent);
	}
}

void RepoClient::SetPurchaseHeader(const json::value& jsonPurchase, PurchaseHeader* purchaseHeader)
{
	purchaseHeader->SetAgentCode(json_nullToEmpty(j(jsonPurchase)[_U("agentCd")]));
	purchaseHeader->SetEndDate(json_DateTime(j(jsonPurchase)[_U("endDt")]));
	purchaseHeader->SetStartDate(json_DateTime(j(jsonPurchase)[_U("startDt")]));
	purchaseHeader->SetPlateNo(json_nullToEmpty(j(jsonPurchase)[_U("plateNo")]));
	purchaseHeader->SetWasDownloaded(json_bool(j(jsonPurchase)[_U("wasDownloaded")]));
	purchaseHeader->SetHerd(j(jsonPurchase)[_U("herdNo")].as_integer());
	purchaseHeader->SetCowCount(j(jsonPurchase)[_U("count")].as_integer());	
}

void RepoClient::SetPurchaseInvoice(const json::value& jsonInvoice, PurchaseInvoice* invoice)
{
	invoice->SetInvoiceDate(json_DayDate(j(jsonInvoice)[_U("invoiceDt")]));
	invoice->SetTransactionDate(json_DayDate(j(jsonInvoice)[_U("transactionDt")]));
	invoice->SetTransactionPlace(json_nullToEmpty(j(jsonInvoice)[_U("transactionPlace")]));
	invoice->SetVatRate(json_NullDecimal(j(jsonInvoice)[_U("vatRate")]).ToDecimal(Decimal(0L)));
	invoice->SetPayWay(PayWays::FromString(json_nullToEmpty(j(jsonInvoice)[_U("payWay")])));
	invoice->SetInvoiceType(InvoiceTypes::FromString(json_nullToEmpty(j(jsonInvoice)[_U("invoiceType")])));
	invoice->SetCustomNumber(json_nullToEmpty(j(jsonInvoice)[_U("customNumber")]));
	invoice->SetPayDueDays(j(jsonInvoice)[_U("payDueDays")].as_integer());
	
	PurchaseHent* hent = new PurchaseHent(L"");		
	SetPurchaseHent(j(jsonInvoice)[_U("hent")], hent);
	invoice->SetInvoiceHent(hent);
	delete hent;
	
	const json::array jsonCows = j(jsonInvoice)[_U("cows")].as_array();

	PurchaseCow* cow;
	for (int i = 0, size = jsonCows.size(); i < size; i++)
	{
		cow = invoice->AddCow();
		SetPurchaseCow(jsonCows.at(i),cow);
	}

	json::value jsonDeductions = j(jsonInvoice)[_U("deductions")];

	if (!jsonDeductions.is_null())
	{
		PurchaseInvoiceDeduction* deduction;
		for (int i = 0, size = jsonDeductions.size(); i < size; i++)
		{
			deduction = invoice->AddDeduction();
			SetPurchaseDeduction(jsonDeductions[i], deduction);

		}
	}
}


void RepoClient::SetPurchaseDeduction(const json::value& jsonDeduction, PurchaseInvoiceDeduction* deduction)
{
	std::wstring code = json_nullToEmpty(jsonDeduction[_U("code")]);
	std::wstring reason = json_nullToEmpty(jsonDeduction[_U("reason")]);
	Decimal fraction = json_Decimal(jsonDeduction[_U("fraction")]);
	bool enabled = jsonDeduction[_U("enabled")].as_bool();

	deduction->SetCode(code);
	deduction->SetReason(reason);
	deduction->SetFraction(fraction);
	deduction->SetIsEnabled(enabled);
}

void RepoClient::SetPurchaseCow(const json::value& jsonCow, PurchaseCow* cow)
{
	std::wstring cowNoStr = json_nullToEmpty(jsonCow[_U("cowNo")]);
	try
	{
		cow->SetCowNo(CowNoFactory::FromString(cowNoStr));
	}
	catch (CowNoException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(L"invalid cow no " + cowNoStr + L": " + e.message()));
	}

	json::value jsonFirstOwnerNo = jsonCow[_U("firstOwner")];
	if (!jsonFirstOwnerNo.is_null())
	{
		std::wstring firstOwnerNoStr = jsonFirstOwnerNo.as_string();
		try
		{
			cow->SetFirstOwner(HentNoFactory::FromString(firstOwnerNoStr));
		}
		catch (HentNoException& e)
		{
			BOOST_THROW_EXCEPTION(RepoException(L"cow " + cowNoStr + L" has invalid first owner no " + firstOwnerNoStr + L": " + e.message()));
		}
	}
	
	
	cow->SetSex(json_CowSex(jsonCow[_U("sex")]));

	cow->SetStockCd(json_nullToEmpty(jsonCow[_U("stockCd")]));
	cow->SetClassCd(json_nullToEmpty(jsonCow[_U("classCd")]));
	cow->SetWeight(json_NullDecimal(jsonCow[_U("weight")]).ToDecimal(Decimal(0L)));
	cow->SetPrice(json_NullDecimal(jsonCow[_U("price")]).ToDecimal(Decimal(0L)));
	cow->SetPassportNo(json_nullToEmpty(jsonCow[_U("passportNo")]));

	
	json::value jsonPassportDt = jsonCow[_U("passportIssueDt")];
	if (!jsonPassportDt.is_null())
		cow->SetPassportIssueDt(json_DayDate(jsonPassportDt));
	
	cow->SetHealthCertNo(json_nullToEmpty(jsonCow[_U("healthCertNo")]));

	json::value jsonMotherNo = jsonCow[_U("motherNo")];
	if (!jsonMotherNo.is_null())
		cow->SetMotherNo(CowNoFactory::FromString(jsonMotherNo.as_string()));

	cow->SetBirthPlace(json_nullToEmpty(jsonCow[_U("birthPlace")]));

	json::value jsonBirthDt = jsonCow[_U("birthDt")];
	if (!jsonBirthDt.is_null())
		cow->SetBirthDt(json_DayDate(jsonBirthDt));
	
	//Json::Value jsonLatitude = jsonCow["latitude"];
	//Json::Value jsonLongitude = jsonCow["longitude"];
}

void RepoClient::SetPurchaseHent(const json::value& jsonValue, PurchaseHent* hent)
{
	std::wstring hentNoStr = json_nullToEmpty(jsonValue[_U("hent_no")]);
	try
	{
		hent->SetHentNo(HentNoFactory::FromString(hentNoStr));
	}
	catch (HentNoException& e)
	{
		BOOST_THROW_EXCEPTION(RepoException(L"invalid hent no " + hentNoStr + L": " + e.message()));
	}

	hent->SetName(json_nullToEmpty(jsonValue[_U("hent_name")]));
	hent->SetStreet(json_nullToEmpty(jsonValue[_U("street")]));
	hent->SetPOBox(json_nullToEmpty(jsonValue[_U("pobox")]));
	hent->SetCity(json_nullToEmpty(jsonValue[_U("city")]));
	hent->SetZip(json_nullToEmpty(jsonValue[_U("zip_code")]));
	hent->SetPhoneNo(json_nullToEmpty(jsonValue[_U("phone_no")]));
	hent->SetFiscalNo(json_nullToEmpty(jsonValue[_U("fiscal_no")]));
	

	hent->SetWetNo(json_nullToEmpty(jsonValue[_U("wet_id_no")]));
	hent->SetPersonalNo(json_nullToEmpty(jsonValue[_U("personal_no")]));
	hent->SetStatsNo(json_nullToEmpty(jsonValue[_U("stats_no")]));
	hent->SetPersonalIdNo(json_nullToEmpty(jsonValue[_U("personal_id_no")]));

	json::value personalIdIssueDt = jsonValue[_U("personal_id_issue_dt")];
	if (!personalIdIssueDt.is_null())
	{
		hent->SetIssueDate(json_DayDate(personalIdIssueDt));
	}
	hent->SetIssuePost(json_nullToEmpty(jsonValue[_U("personal_id_issue_post")]));
	hent->SetAccountNo(AccountNoFactory::FromString(json_nullToEmpty(jsonValue[_U("account_no")])));
	hent->SetBankName(json_nullToEmpty(jsonValue[_U("bank_name")]));
	hent->SetWetLicenceNo(json_nullToEmpty(jsonValue[_U("wet_lic_no")]));
	hent->SetCellPhoneNo(json_nullToEmpty(jsonValue[_U("cellphone_no")]));
	hent->SetEmailAddress(json_nullToEmpty(jsonValue[_U("email_address")]));
	hent->SetAlias(json_nullToEmpty(jsonValue[_U("alias")]));
	hent->SetPlateNo(json_nullToEmpty(jsonValue[_U("plate_no")]));
	
	json::value jsonHentType = jsonValue[_U("hent_type")];
	if (!jsonHentType.is_null())
		hent->SetHentType(HentTypes::FromString(jsonHentType.as_string()));

	//hent->SetLatitude(TextUtils::FromUTF8(jsonValue["latitude"];
	//hent->SetLongitude(TextUtils::FromUTF8(jsonValue["longitude"];
}





}

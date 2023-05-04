#include "stdafx.h"
#include "ChangePublisher.h"
#include <utils/JsonUtils.h>
#include <data/IAgent.h>
#include <data/IHerd.h>
#include "../SeasonSession.h"
#include "../FarmSession.h"
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <boost/foreach.hpp>


ChangePublisherFactory::ChangePublisherFactory(const gcp::GoogleServiceAccount& account, FarmSession* farmSession) : m_account(account), m_farmSession(farmSession)
{
}

ChangePublisher* ChangePublisherFactory::CreatePublisher(SeasonSession* session)
{
	if(!m_account.IsNull())
	{
		return new ChangePublisher(m_account, session,m_farmSession);
	}
	return NULL;
}

//-------------------------------------------------------------------------------------------------

//json::value json_DateTimeZ(const DateTime& dt)
//{
//
//	if (dt.IsNull())
//		return json::value::null();
//
//	return json::value(dt.GetDateInFmt(DateTime::YYYYdashMMdashDDTHHcMIcSS) + L"Z");	
//}
//
//
//json::value json_NullDecimalAsFloat(const NullDecimal& d)
//{
//	if (d.IsNull())
//		return json::value::null();
//
//	std::wstring sDecimal = d.ToString(5);
//	return json::value(stod(sDecimal));
//}
//
//json::value json_DecimalAsFloat(const Decimal& d)
//{	
//	std::wstring sDecimal = d.ToString(5);
//	return json::value(stod(sDecimal));
//}



ChangePublisher::ChangePublisher(const gcp::GoogleServiceAccount& account, SeasonSession* session, FarmSession* farmSession) : m_fcmPublisher(account), m_session(session),m_farmSession(farmSession)
{
}


void ChangePublisher::DocDetailsToJson(const IDoc* doc,json::value& result)
{	
	static const wchar_t* docdate_attribute_name = L"docdate";
	static const wchar_t* loaddate_attribute_name = L"loaddate";
	static const wchar_t* extras_attribute_name = L"extras";
	static const wchar_t* plateno_attribute_name = L"plateno";
	static const wchar_t* motive_attribute_name = L"motive";
	static const wchar_t* loadstartdttm_attribute_name = L"loadstartdttm";
	static const wchar_t* loadenddttm_attribute_name = L"loadenddttm";
	static const wchar_t* agent_attribute_name = L"agent";


	result[docdate_attribute_name] = json_DateTimeZ(doc->GetDocDate());
	result[loaddate_attribute_name] = json_DateTimeZ(doc->GetLoadDate());
	result[extras_attribute_name] = json::value(doc->GetExtras());
	result[plateno_attribute_name] = json::value(doc->GetPlateNo());
	result[motive_attribute_name] = json_Motive(doc->GetMotive());
	result[loadstartdttm_attribute_name] = json_Time(doc->GetLoadStartDtTm());
	result[loadenddttm_attribute_name] = json_Time(doc->GetLoadEndDtTm());
	   
	result[agent_attribute_name] = AgentToJson(doc->GetAgentId());

}

json::value ChangePublisher::CowEntryToJson(const ICowEntry* entry)
{
	static const wchar_t* cowkey_attribute_name = L"cow_key";
	static const wchar_t* weight_attribute_name = L"weight";
	static const wchar_t* stock_attribute_name = L"stock";
	static const wchar_t* class_attribute_name = L"cow_class";
	static const wchar_t* group_attribute_name = L"group";
		
	json::value result;

	result[cowkey_attribute_name] = json::value(L"cow" + IntString(entry->GetCowId()));
	result[weight_attribute_name] = json_NullDecimalAsFloat(entry->GetWeight());
	result[stock_attribute_name] = StockToJson(entry->GetStockId());
	result[class_attribute_name] = ClassToJson(entry->GetClassId());
	result[group_attribute_name] = json_NullInt(entry->GetGroup());

	return result;
	
}


json::value ChangePublisher::DocContentToJson(const IDoc* doc)
{
	std::vector<json::value> jsonEntries;

	PtrEnumerator<ICowEntry> enumCows = doc->EnumCows();

	while(enumCows.hasNext())
	{
		jsonEntries.push_back(CowEntryToJson(*enumCows));
		enumCows.advance();
	}

	return json::value::array(jsonEntries);

}

json::value ChangePublisher::InvoiceContentToJson(const IInvoice* invoice)
{
	std::vector<json::value> jsonEntries;

	PtrEnumerator<ICowInvoiceEntry> enumEntries = invoice->EnumEntries();

	while (enumEntries.hasNext())
	{
		jsonEntries.push_back(CowInvoiceEntryToJson(*enumEntries));
		enumEntries.advance();
	}

	return json::value::array(jsonEntries);
}

json::value ChangePublisher::AgentToJson(uint32_t agentId)
{

	if (agentId != NULL_ID)
	{
		IAgent* agent = m_farmSession->FindAgent(agentId);

		return json::value(agent->GetAgentCode());
	}

	return json::value::null();
}

json::value ChangePublisher::HerdToJson(uint32_t herdId)
{	
	
	if (herdId != NULL_ID)
	{
		IHerd* herd = m_farmSession->FindHerdById(herdId);

		return json::value(herd->GetHerdIndex());
	}

	return json::value::null();
}

json::value ChangePublisher::StockToJson(uint32_t stockId)
{
	if (stockId != NULL_ID)
	{
		IStock* stock = m_session->FindStock(stockId);
		if (stock != NULL)
		{
			return json::value(stock->GetStockCode());
		}
	}

	return json::value::null();
}

json::value ChangePublisher::ClassToJson(uint32_t classId)
{
	if (classId != NULL_ID)
	{
		IClass* cowclass = m_session->FindClass(classId);
		if (cowclass != NULL)
		{
			return json::value(cowclass->GetClassCode());
		}
	}

	return json::value::null();
}

json::value ChangePublisher::HentToJson(uint32_t hentId)
{
	if (hentId != NULL_ID)
	{
		HentServerObject* hentSO = m_session->FindHent(hentId);
		Hent* hent = hentSO->Object();

		return json_HentNo(hent->GetHentNo());
	}

	return json::value::null();
}


json::value ChangePublisher::BuyDocToJson(const IBuyDoc* buydoc)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* herd_attribute_name = L"herd";
	static const wchar_t* hent_attribute_name = L"from_hent";
	static const wchar_t* cows_attribute_name = L"cows";

	json::value result;

	result[key_attribute_name] = json::value(L"buy" + IntString(buydoc->GetId()));
	result[herd_attribute_name] = HerdToJson(buydoc->GetHerdId());
	DocDetailsToJson(buydoc, result);
	result[cows_attribute_name] = DocContentToJson(buydoc);
	result[hent_attribute_name] = HentToJson(buydoc->GetHentId());	

	return result;
}

json::value ChangePublisher::InDocToJson(const IInDoc* indoc)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* herd_attribute_name = L"herd";	
	static const wchar_t* cows_attribute_name = L"cows";

	json::value result;

	result[key_attribute_name] = json::value(L"in" + IntString(indoc->GetId()));
	result[herd_attribute_name] = HerdToJson(indoc->GetHerdId());	
	DocDetailsToJson(indoc,result);
	result[cows_attribute_name] = DocContentToJson(indoc);
	return result;
}

json::value ChangePublisher::MoveDocToJson(const IMoveDoc* movedoc)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* herd_to_attribute_name = L"herd_to";
	static const wchar_t* herd_from_attribute_name = L"herd_from";
	static const wchar_t* details_attribute_name = L"details";
	static const wchar_t* cows_attribute_name = L"cows";

	json::value result;

	result[key_attribute_name] = json::value(L"move" + IntString(movedoc->GetId()));
	result[herd_from_attribute_name] = HerdToJson(movedoc->GetSrcHerdId());
	result[herd_to_attribute_name] = HerdToJson(movedoc->GetDstHerdId());
	DocDetailsToJson(movedoc, result);
	result[cows_attribute_name] = DocContentToJson(movedoc);

	return result;
}

json::value ChangePublisher::OutDocToJson(const IOutDoc* outdoc)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* herd_attribute_name = L"herd";
	static const wchar_t* details_attribute_name = L"details";
	static const wchar_t* cows_attribute_name = L"cows";

	json::value result;

	result[key_attribute_name] = json::value(L"out" + IntString(outdoc->GetId()));
	result[herd_attribute_name] = HerdToJson(outdoc->GetHerdId());

	DocDetailsToJson(outdoc, result);
	result[cows_attribute_name] = DocContentToJson(outdoc);

	return result;
}

json::value ChangePublisher::SellDocToJson(const ISellDoc* selldoc)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* hent_attribute_name = L"to_hent";
	static const wchar_t* herd_attribute_name = L"herd";
	//static const wchar_t* invoiceid_attribute_name = L"invoiceid";
	static const wchar_t* details_attribute_name = L"details";
	static const wchar_t* cows_attribute_name = L"cows";

	json::value result;

	result[key_attribute_name] = json::value(L"sell" + IntString(selldoc->GetId()));
	result[hent_attribute_name] = HentToJson(selldoc->GetHentId());
	result[herd_attribute_name] = HerdToJson(selldoc->GetHerdId());
	//result[invoiceid_attribute_name] = json::value(this->m_invoiceId);

	DocDetailsToJson(selldoc, result);
	result[cows_attribute_name] = DocContentToJson(selldoc);

	return result;
}


json::value ChangePublisher::CowSOsToJson(const std::vector<CowServerObject*>& cows)
{
	std::vector<json::value> jsonCows;
	
	BOOST_FOREACH(CowServerObject* cowSO,cows)
	{
		jsonCows.push_back(CowToJson(cowSO->Object()));
	}

	return json::value::array(jsonCows);
}

void ChangePublisher::CowDetailsToJson(const ICow* cow, json::value& result)
{
	static const wchar_t* weight_attribute_name = L"weight";
	static const wchar_t* stock_attribute_name = L"stock";
	static const wchar_t* class_attribute_name = L"class";
	static const wchar_t* cowno_attribute_name = L"cow_no";
	static const wchar_t* motherno_attribute_name = L"motherno";
	static const wchar_t* birthdate_attribute_name = L"birth_date";
	static const wchar_t* passportno_attribute_name = L"passport_no";
	static const wchar_t* healthcertno_attribute_name = L"health_cert_no";
	static const wchar_t* passportdate_attribute_name = L"passport_date";
	static const wchar_t* extras_attribute_name = L"extras";
	static const wchar_t* birthplace_attribute_name = L"birth_place";
	static const wchar_t* firtsowner_attribute_name = L"firstowner";
	static const wchar_t* sex_attribute_name = L"sex";
	static const wchar_t* termbuyprice_attribute_name = L"termbuy_price";
	static const wchar_t* termsellprice_attribute_name = L"termsell_price";
	static const wchar_t* termbuyweight_attribute_name = L"termbuy_weight";
	static const wchar_t* termsellweight_attribute_name = L"termsell_weight";
	static const wchar_t* termbuyclass_attribute_name = L"termbuy_class_id";
	static const wchar_t* termsellclass_attribute_name = L"termsell_class_id";

	result[weight_attribute_name] = json_DecimalAsFloat(cow->GetWeight());
	result[stock_attribute_name] = StockToJson(cow->GetStockId());
	result[class_attribute_name] = ClassToJson(cow->GetClassId());
	result[cowno_attribute_name] = json::value(cow->GetCowNo().ToString());
	result[motherno_attribute_name] = json::value(cow->GetMotherNo().ToString());
	result[birthdate_attribute_name] = json_DateTimeZ(cow->GetBirthDate());
	result[passportno_attribute_name] = json::value(cow->GetPassNo());
	result[healthcertno_attribute_name] = json::value(cow->GetHealthCertNo());
	result[passportdate_attribute_name] = json_DateTimeZ(cow->GetPassDate());
	result[extras_attribute_name] = json::value(cow->GetExtras());
	result[birthplace_attribute_name] = json::value(cow->GetBirthPlace());
	result[firtsowner_attribute_name] = HentToJson(cow->GetFirstOwnerId());
	result[sex_attribute_name] = json_CowSex(cow->GetSex());
	result[termbuyprice_attribute_name] = json_NullDecimalAsFloat(cow->GetTermBuyPrice());
	result[termsellprice_attribute_name] = json_NullDecimalAsFloat(cow->GetTermSellPrice());
	result[termbuyweight_attribute_name] = json_NullDecimalAsFloat(cow->GetTermBuyWeight());
	result[termsellweight_attribute_name] = json_NullDecimalAsFloat(cow->GetTermSellWeight());
	result[termbuyclass_attribute_name] = ClassToJson(cow->GetTermBuyClassId());
	result[termsellclass_attribute_name] = ClassToJson(cow->GetTermSellClassId());
}

json::value ChangePublisher::CowToJson(const ICow* cow)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* passimglabel_attribute_name = L"passimg_label";
	static const wchar_t* sellinvoiceid_attribute_name = L"sellinvoice_id";
	static const wchar_t* buyinvoiceid_attribute_name = L"buyinvoice_id";
	
	json::value result;

	result[key_attribute_name] = json::value(L"cow" + IntString(cow->GetId()));
	result[sellinvoiceid_attribute_name] = json::value(cow->GetSellInvoiceId());
	result[buyinvoiceid_attribute_name] = json::value(cow->GetBuyInvoiceId());
	CowDetailsToJson(cow,result);
	result[passimglabel_attribute_name] = json::value(cow->GetPassImgLabel());

	return result;
}

void ChangePublisher::PublishMessage(const std::wstring& changeType,const std::wstring& entityType, json::value& body)
{
	static const wchar_t* change_type_attribute_name = L"change_type";
	static const wchar_t* entity_type_attribute_name = L"entity_type";
	static const wchar_t* body_attribute_name = L"body";

	
	IFarm* farm = m_farmSession->GetFarm();
	FarmNo farmNo = farm->GetFarmNo();

	json::value message;
	message[change_type_attribute_name] = json::value(changeType);
	message[entity_type_attribute_name] = json::value(entityType);
	message[body_attribute_name] = body;

	//m_fcmPublisher.PublishToTopic(TextUtils::ToUTF8(message.serialize()), u(farmNo.ToString()) + ".changes");
}

void ChangePublisher::PublishCowUpdate(const ICow* cow)
{
	json::value cowJson = CowToJson(cow);
	
	json::value body;
	body[L"cow"] = cowJson;
	
	PublishMessage(L"update", L"cow", body);
}

void ChangePublisher::PublishInDocInsert(const IInDoc* indoc)
{	
	json::value indocJson = InDocToJson(indoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(indoc);
	json::value cowsJson = CowSOsToJson(cows);
	
	json::value body;
	body[L"plusdoc"] = indocJson;
	body[L"cows"] = cowsJson;
	
	PublishMessage(L"insert",L"plusdoc", body);
}

void ChangePublisher::PublishBuyDocInsert(const IBuyDoc* buydoc)
{
	json::value indocJson = BuyDocToJson(buydoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(buydoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"plusdoc"] = indocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"plusdoc", body);
}

void ChangePublisher::PublishInDocUpdate(const IInDoc* indoc)
{
	json::value indocJson = InDocToJson(indoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(indoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"plusdoc"] = indocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"plusdoc", body);
}

void ChangePublisher::PublishBuyDocUpdate(const IBuyDoc* buydoc)
{
	json::value indocJson = BuyDocToJson(buydoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(buydoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"plusdoc"] = indocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"plusdoc", body);
}

void ChangePublisher::PublishInDocDelete(uint32_t indocid)
{
	json::value body;
	body[L"key"] = json::value(L"in" + IntString(indocid));
	PublishMessage(L"delete", L"plusdoc", body);
}

void ChangePublisher::PublishBuyDocDelete(uint32_t buydocid)
{
	json::value body;
	body[L"key"] = json::value(L"buy" + IntString(buydocid));
	PublishMessage(L"delete", L"plusdoc", body);
}

void ChangePublisher::PublishBuyInvoiceInsert(const IBuyInvoice* buyinvoice)
{
	json::value buyinvoiceJson = BuyInvoiceToJson(buyinvoice);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForInvoice(buyinvoice);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"buyinvoice"] = buyinvoiceJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"buyinvoice", body);
}


void ChangePublisher::PublishMoveDocInsert(const IMoveDoc* movedoc)
{
	json::value movedocJson = MoveDocToJson(movedoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(movedoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"movedoc"] = movedocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"movedoc", body);
}

void ChangePublisher::PublishMoveDocUpdate(const IMoveDoc* buydoc)
{
	json::value movedocJson = MoveDocToJson(buydoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(buydoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"movedoc"] = movedocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"movedoc", body);
}

void ChangePublisher::PublishMoveDocDelete(uint32_t movedocid)
{
	json::value body;
	body[L"doc_id"] = json::value(movedocid);

	PublishMessage(L"delete", L"buydoc", body);
}

void ChangePublisher::PublishSellDocInsert(const ISellDoc* selldoc)
{
	json::value selldocJson = SellDocToJson(selldoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(selldoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"selldoc"] = selldocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"selldoc", body);
}

void ChangePublisher::PublishSellDocUpdate(const ISellDoc* selldoc)
{
	json::value selldocJson = SellDocToJson(selldoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(selldoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"selldoc"] = selldocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"selldoc", body);
}

void ChangePublisher::PublishSellDocDelete(uint32_t selldocid)
{
	json::value body;
	body[L"doc_id"] = json::value(selldocid);

	PublishMessage(L"delete", L"selldoc", body);
}

void ChangePublisher::PublishOutDocInsert(const IOutDoc* outdoc)
{
	json::value outdocJson = OutDocToJson(outdoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(outdoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"outdoc"] = outdocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"outdoc", body);
}

void ChangePublisher::PublishOutDocUpdate(const IOutDoc* outdoc)
{
	json::value outdocJson = OutDocToJson(outdoc);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForDoc(outdoc);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"outdoc"] = outdocJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"outdoc", body);
}

void ChangePublisher::PublishOutDocDelete(uint32_t outdocid)
{
	json::value body;
	body[L"doc_id"] = json::value(outdocid);

	PublishMessage(L"delete", L"outdoc", body);
}


void ChangePublisher::PublishSellInvoiceInsert(const ISellInvoice* sellinvoice)
{
	json::value sellinvoiceJson = SellInvoiceToJson(sellinvoice);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForInvoice(sellinvoice);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"sellinvoice"] = sellinvoiceJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"insert", L"sellinvoice", body);
}

void ChangePublisher::PublishSellInvoiceUpdate(const ISellInvoice* sellinvoice)
{
	json::value sellinvoiceJson = SellInvoiceToJson(sellinvoice);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForInvoice(sellinvoice);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"sellinvoice"] = sellinvoiceJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"sellinvoice", body);
}

void ChangePublisher::PublishSellInvoiceDelete(uint32_t sellinvoiceid)
{
	json::value body;
	body[L"invoice_id"] = json::value(sellinvoiceid);

	PublishMessage(L"delete", L"sellinvoice", body);
}


void ChangePublisher::InvoiceDetailsToJson(const IInvoice* invoice, json::value& result)
{
	static const wchar_t* extras_attribute_name = L"extras";
	static const wchar_t* invoicedate_attribute_name = L"invoice_date";
	static const wchar_t* paiddate_attribute_name = L"paid_date";
	static const wchar_t* vatrate_attribute_name = L"vat_rate";
	static const wchar_t* payway_attribute_name = L"payway";
	static const wchar_t* customnumber_attribute_name = L"custom_number";
	static const wchar_t* payduedays_attribute_name = L"payduedays";
	static const wchar_t* transactionplace_attribute_name = L"transactionplace";
	static const wchar_t* transactiondate_attribute_name = L"transactiondate";

	result[extras_attribute_name] = json::value(invoice->GetExtras());
	result[invoicedate_attribute_name] = json_DateTimeZ(invoice->GetInvoiceDate());
	result[paiddate_attribute_name] = json_DateTimeZ(invoice->GetPaidDate());
	result[vatrate_attribute_name] = json_DecimalAsFloat(invoice->GetVATRate());
	result[payway_attribute_name] = json_PayWay(invoice->GetPayWay());
	result[customnumber_attribute_name] = json::value(invoice->GetCustomNumber());
	result[payduedays_attribute_name] = json::value(invoice->GetPayDueDays());
	result[transactionplace_attribute_name] = json::value(invoice->GetTransactionPlace());
	result[transactiondate_attribute_name] = json_DateTimeZ(invoice->GetTransactionDate());
	
}

json::value ChangePublisher::BuyInvoiceToJson(const IBuyInvoice* buyinvoice)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* scope_attribute_name = L"scope";
	static const wchar_t* invoicetype_attribute_name = L"invoice_type";
	static const wchar_t* invoiceno_attribute_name = L"invoice_no";
	static const wchar_t* hent_attribute_name = L"hent";
	static const wchar_t* cows_attribute_name = L"cows";
	static const wchar_t* deductions_attribute_name = L"deductions";


	json::value result;

	result[key_attribute_name] = json::value(L"buy" + IntString(buyinvoice->GetId()));
	result[scope_attribute_name] = json_InvoiceScope(buyinvoice->GetInvoiceScope());
	result[invoicetype_attribute_name] = json_InvoiceType(buyinvoice->GetInvoiceType());
	result[invoiceno_attribute_name] = json::value(buyinvoice->GetInvoiceNo());

	result[hent_attribute_name] = InvoiceHentToJson(buyinvoice->GetInvoiceHent());
	result[deductions_attribute_name] = InvoiceDeductionsToJson(buyinvoice);
	InvoiceDetailsToJson(buyinvoice, result);
	result[cows_attribute_name] = InvoiceContentToJson(buyinvoice);

	return result;
}

json::value ChangePublisher::SellInvoiceToJson(const ISellInvoice* sellinvoice)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* scope_attribute_name = L"scope";
	static const wchar_t* invoicetype_attribute_name = L"invoice_type";
	static const wchar_t* invoiceno_attribute_name = L"invoice_no";
	static const wchar_t* hent_attribute_name = L"hent";
	static const wchar_t* details_attribute_name = L"details";
	static const wchar_t* cows_attribute_name = L"cows";
	static const wchar_t* deductions_attribute_name = L"deductions";

	json::value result;

	result[key_attribute_name] = json::value(L"sell" + IntString(sellinvoice->GetId()));
	result[scope_attribute_name] = json_InvoiceScope(sellinvoice->GetInvoiceScope());
	result[invoicetype_attribute_name] = json_InvoiceType(sellinvoice->GetInvoiceType());
	result[invoiceno_attribute_name] = json::value(sellinvoice->GetInvoiceNo());

	result[hent_attribute_name] = InvoiceHentToJson(sellinvoice->GetInvoiceHent());
	result[deductions_attribute_name] = InvoiceDeductionsToJson(sellinvoice);
	InvoiceDetailsToJson(sellinvoice, result);
	result[cows_attribute_name] = InvoiceContentToJson(sellinvoice);

	return result;
}

json::value ChangePublisher::CowInvoiceEntryToJson(const ICowInvoiceEntry* entry)
{
	static const wchar_t* cowkey_attribute_name = L"cow_key";
	static const wchar_t* weight_attribute_name = L"weight";
	static const wchar_t* price_attribute_name = L"price";
	static const wchar_t* class_attribute_name = L"cow_class";

	json::value result;

	result[cowkey_attribute_name] = json::value(L"cow" + IntString(entry->GetCowId()));
	result[weight_attribute_name] = json_DecimalAsFloat(entry->GetWeight());
	result[class_attribute_name] = ClassToJson(entry->GetClassId());
	result[price_attribute_name] = json_DecimalAsFloat(entry->GetPrice());


	return result;
}

json::value ChangePublisher::InvoiceDeductionToJson(const IInvoiceDeduction* deduction)
{
	static const wchar_t* enabled_attribute_name = L"enabled";
	static const wchar_t* code_attribute_name = L"code";
	static const wchar_t* reason_attribute_name = L"reason";
	static const wchar_t* fraction_attribute_name = L"fraction";


	json::value result;

	result[enabled_attribute_name] = json_bool(deduction->GetIsEnabled());
	result[fraction_attribute_name] = json_DecimalAsFloat(deduction->GetFraction());
	result[code_attribute_name] = json::value(deduction->GetCode());
	result[reason_attribute_name] = json::value(deduction->GetReason());


	return result;
}

json::value ChangePublisher::InvoiceDeductionsToJson(const IInvoice* invoice)
{
	std::vector<json::value> jsonDeductions;

	PtrEnumerator<IInvoiceDeduction> enumDeductions = invoice->EnumDeductions();

	while (enumDeductions.hasNext())
	{
		jsonDeductions.push_back(InvoiceDeductionToJson(*enumDeductions));
		enumDeductions.advance();
	}

	return json::value::array(jsonDeductions);
}



json::value ChangePublisher::InvoiceHentToJson(const IInvoiceHent* invoiceHent)
{
	static const wchar_t* hentname_attribute_name = L"hent_name";
	static const wchar_t* city_attribute_name = L"city";
	static const wchar_t* zipcode_attribute_name = L"zip_code";
	static const wchar_t* street_attribute_name = L"street";
	static const wchar_t* pobox_attribute_name = L"pobox";
	static const wchar_t* hentno_attribute_name = L"hent_no";
	static const wchar_t* fiscalno_attribute_name = L"fiscal_no";
	static const wchar_t* personalno_attribute_name = L"personal_no";
	static const wchar_t* personalidno_attribute_name = L"personal_id_no";
	static const wchar_t* personalidissuedt_attribute_name = L"personal_id_issue_dt";
	static const wchar_t* personalidissuepost_attribute_name = L"personal_id_issue_post";
	static const wchar_t* accountno_attribute_name = L"account_no";
	static const wchar_t* bankname_attribute_name = L"bank_name";
	static const wchar_t* parenthentid_attribute_name = L"parent_hent_id";


	json::value result;

	result[hentname_attribute_name] = json::value(invoiceHent->GetName());
	result[city_attribute_name] = json::value(invoiceHent->GetCity());
	result[zipcode_attribute_name] = json::value(invoiceHent->GetZip());
	result[street_attribute_name] = json::value(invoiceHent->GetStreet());
	result[pobox_attribute_name] = json::value(invoiceHent->GetPOBox());
	result[hentno_attribute_name] = json::value(invoiceHent->GetHentNo().ToString());
	result[fiscalno_attribute_name] = json::value(invoiceHent->GetFiscalNo());
	result[personalno_attribute_name] = json::value(invoiceHent->GetPESEL());
	result[personalidno_attribute_name] = json::value(invoiceHent->GetIdNo());
	result[personalidissuepost_attribute_name] = json::value(invoiceHent->GetIssuePost());
	result[accountno_attribute_name] = json::value(invoiceHent->GetAccountNo().ToString());
	result[bankname_attribute_name] = json::value(invoiceHent->GetBankName());
	result[parenthentid_attribute_name] = json::value(invoiceHent->GetParentHentId());
	result[personalidissuedt_attribute_name] = json_DateTimeZ(invoiceHent->GetIssueDate());
	return result;
}

void ChangePublisher::PublishBuyInvoiceUpdate(const IBuyInvoice* buyinvoice)
{
	json::value buyinvoiceJson = BuyInvoiceToJson(buyinvoice);

	std::vector<CowServerObject*> cows = m_session->GetCowSOsForInvoice(buyinvoice);
	json::value cowsJson = CowSOsToJson(cows);

	json::value body;
	body[L"buyinvoice"] = buyinvoiceJson;
	body[L"cows"] = cowsJson;

	PublishMessage(L"update", L"buyinvoice", body);
}

void ChangePublisher::PublishBuyInvoiceDelete(uint32_t buyinvoiceid)
{
	json::value body;
	body[L"invoice_id"] = json::value(buyinvoiceid);

	PublishMessage(L"delete", L"buyinvoice", body);
}

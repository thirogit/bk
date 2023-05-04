#include "stdafx.h"
#include "JsonExporter.h"

#include <boost/foreach.hpp>
#include "../../../invoice/calculator/InvoiceCalculator.h"
#include <string/TextUtils.h>
#include <utils\JsonUtils.h>
#include <data\IAgent.h>
#include <data\IHerd.h>
#include <string/TextUtils.h>
#include <string/IntString.h>


std::wstring JsonExporter::GetExporterName()
{
	return L"JSON";
}

std::wstring JsonExporter::GetFileExtension()
{
	return L"json";
}

std::string JsonExporter::GetDefaultEncoding()
{
	return "utf-8";
}



void JsonExporter::Export(IInvoiceRange invoices,
									 InvoiceExporterOptions& options,
									 std::wofstream& output,
									 SeasonSession* session,
									 SimpleProgress *progress)
{		
	static const wchar_t* invoice_attribute_name = L"invoice";
	static const wchar_t* cows_attribute_name = L"weight";

	JsonSerializer seriazlizer(session);
	std::vector<json::value> exportedElements;
	BOOST_FOREACH(IInvoice* invoice, invoices)
	{	
		json::value  jsonInvoice = seriazlizer.InvoiceToJson(invoice);
		

		PtrEnumerator<ICowInvoiceEntry> entries = invoice->EnumEntries();
		std::vector<CowClientObject*> cowCOs;
		while (entries.hasNext()) {
			ICowInvoiceEntry* entry = *entries;
			cowCOs.push_back(session->GetCow(entry->GetCowId()));
			entries.advance();
		}
		
		json::value element;

		element[invoice_attribute_name] = jsonInvoice;
		element[cows_attribute_name] = seriazlizer.CowCOsToJson(cowCOs);
		
		exportedElements.push_back(element);
	}	
	json::value::array(exportedElements).serialize(output);
	
}


JsonExporter::JsonSerializer::JsonSerializer(SeasonSession* session) : m_session(session) {

}

json::value JsonExporter::JsonSerializer::InvoiceToJson(const IInvoice* invoice)
{
	
	switch (invoice->GetInvoiceDirection())
	{
	case InvoiceDirection_Buy:
		return BuyInvoiceToJson((BuyInvoice*)invoice);
	case InvoiceDirection_Sell:
		return SellInvoiceToJson((SellInvoice*)invoice);
	}

	return json::value::null();
}

json::value JsonExporter::JsonSerializer::InvoiceContentToJson(const IInvoice* invoice)
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

json::value JsonExporter::JsonSerializer::AgentToJson(uint32_t agentId)
{

	if (agentId != NULL_ID)
	{
		IAgent* agent = m_session->GetAgent(agentId);

		return json::value(agent->GetAgentCode());
	}

	return json::value::null();
}

json::value JsonExporter::JsonSerializer::HerdToJson(uint32_t herdId)
{

	if (herdId != NULL_ID)
	{
		IHerd* herd = m_session->GetHerd(herdId);

		return json::value(herd->GetHerdIndex());
	}

	return json::value::null();
}

json::value JsonExporter::JsonSerializer::StockToJson(uint32_t stockId)
{
	if (stockId != NULL_ID)
	{
		IStock* stock = m_session->GetStock(stockId);
		if (stock != NULL)
		{
			return json::value(stock->GetStockCode());
		}
	}

	return json::value::null();
}

json::value JsonExporter::JsonSerializer::ClassToJson(uint32_t classId)
{
	if (classId != NULL_ID)
	{
		IClass* cowclass = m_session->GetClass(classId);
		if (cowclass != NULL)
		{
			return json::value(cowclass->GetClassCode());
		}
	}

	return json::value::null();
}

json::value JsonExporter::JsonSerializer::HentToJson(uint32_t hentId)
{
	if (hentId != NULL_ID)
	{
		HentClientObject* hentCO = m_session->GetHent(hentId);
		Hent* hent = hentCO->Object();
		return json_HentNo(hent->GetHentNo());
	}

	return json::value::null();
}


json::value JsonExporter::JsonSerializer::CowCOsToJson(const std::vector<CowClientObject*>& cows)
{
	std::vector<json::value> jsonCows;

	BOOST_FOREACH(CowClientObject* cowSO, cows)
	{
		jsonCows.push_back(CowToJson(cowSO->Object()));
	}

	return json::value::array(jsonCows);
}

void JsonExporter::JsonSerializer::CowDetailsToJson(const ICow* cow, json::value& result)
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
	result[birthdate_attribute_name] = json_DateTime(cow->GetBirthDate());
	result[passportno_attribute_name] = json::value(cow->GetPassNo());
	result[healthcertno_attribute_name] = json::value(cow->GetHealthCertNo());
	result[passportdate_attribute_name] = json_DateTime(cow->GetPassDate());
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

json::value JsonExporter::JsonSerializer::CowToJson(const ICow* cow)
{
	static const wchar_t* key_attribute_name = L"key";
	static const wchar_t* passimglabel_attribute_name = L"passimg_label";
	static const wchar_t* sellinvoiceid_attribute_name = L"sellinvoice_id";
	static const wchar_t* buyinvoiceid_attribute_name = L"buyinvoice_id";

	json::value result;

	result[key_attribute_name] = json::value(L"cow" + IntString(cow->GetId()));
	result[sellinvoiceid_attribute_name] = json::value(cow->GetSellInvoiceId());
	result[buyinvoiceid_attribute_name] = json::value(cow->GetBuyInvoiceId());
	CowDetailsToJson(cow, result);
	result[passimglabel_attribute_name] = json::value(cow->GetPassImgLabel());

	return result;
}


void JsonExporter::JsonSerializer::InvoiceDetailsToJson(const IInvoice* invoice, json::value& result)
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
	result[invoicedate_attribute_name] = json_DateTime(invoice->GetInvoiceDate());
	result[paiddate_attribute_name] = json_DateTime(invoice->GetPaidDate());
	result[vatrate_attribute_name] = json_DecimalAsFloat(invoice->GetVATRate());
	result[payway_attribute_name] = json_PayWay(invoice->GetPayWay());
	result[customnumber_attribute_name] = json::value(invoice->GetCustomNumber());
	result[payduedays_attribute_name] = json::value(invoice->GetPayDueDays());
	result[transactionplace_attribute_name] = json::value(invoice->GetTransactionPlace());
	result[transactiondate_attribute_name] = json_DateTime(invoice->GetTransactionDate());

}

json::value JsonExporter::JsonSerializer::BuyInvoiceToJson(const IBuyInvoice* buyinvoice)
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

json::value JsonExporter::JsonSerializer::SellInvoiceToJson(const ISellInvoice* sellinvoice)
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

json::value JsonExporter::JsonSerializer::CowInvoiceEntryToJson(const ICowInvoiceEntry* entry)
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

json::value JsonExporter::JsonSerializer::InvoiceDeductionToJson(const IInvoiceDeduction* deduction)
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

json::value JsonExporter::JsonSerializer::InvoiceDeductionsToJson(const IInvoice* invoice)
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



json::value JsonExporter::JsonSerializer::InvoiceHentToJson(const IInvoiceHent* invoiceHent)
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
	result[personalidissuedt_attribute_name] = json_DateTime(invoiceHent->GetIssueDate());
	return result;
}


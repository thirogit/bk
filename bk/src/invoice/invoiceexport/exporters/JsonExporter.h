#ifndef __JSONEXPORTER_H__
#define __JSONEXPORTER_H__

#include "InvoiceExporter.h"

class JsonExporter : public InvoiceExporter
{

	class JsonSerializer {

	public:
		JsonSerializer(SeasonSession* session);


		json::value InvoiceToJson(const IInvoice* invoice);
		json::value BuyInvoiceToJson(const IBuyInvoice* buyinvoice);
		json::value SellInvoiceToJson(const ISellInvoice* sellinvoice);
		json::value CowCOsToJson(const std::vector<CowClientObject*>& cows);
	private:

		void CowDetailsToJson(const ICow* cow, json::value& result);
		void InvoiceDetailsToJson(const IInvoice* invoice, json::value& result);

		json::value HerdToJson(uint32_t herdId);
		json::value HentToJson(uint32_t hentId);
		json::value AgentToJson(uint32_t agentId);
		json::value StockToJson(uint32_t stockId);
		json::value ClassToJson(uint32_t classId);
		json::value CowInvoiceEntryToJson(const ICowInvoiceEntry* entry);
		json::value CowToJson(const ICow* cow);
		json::value InvoiceContentToJson(const IInvoice* invoice);
		json::value InvoiceDeductionsToJson(const IInvoice* invoice);
		json::value InvoiceHentToJson(const IInvoiceHent* invoiceHent);
		json::value InvoiceDeductionToJson(const IInvoiceDeduction* deduction);

	private:
		SeasonSession* m_session;
	};


public:
	virtual std::wstring GetExporterName();
	virtual std::wstring GetFileExtension();
	virtual std::string GetDefaultEncoding();

	virtual void Export(  IInvoiceRange invoices,
										InvoiceExporterOptions& options,
										std::wofstream& output,
										SeasonSession* session,
										SimpleProgress *progress);
private:	
	json::value InvoiceToJson(const IInvoice* invoice);
	
	
};

#endif
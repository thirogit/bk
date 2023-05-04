#ifndef __JPKEXPORTER_H__
#define __JPKEXPORTER_H__

#include "InvoiceExporter.h"
#include "quickxml.h"

class JpkExporter : public InvoiceExporter
{		
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
	quickxml::element SerializeBuy(int index,const IBuyInvoice* invoice);
	quickxml::element SerializeSell(int index,const ISellInvoice* invoice);
	std::wstring GetInvoiceHentFiscalNo(const IInvoiceHent* hent);
	std::wstring GetHentAddress(const IInvoiceHent* hent);
	DateTime GetTransactionDate(const IInvoice* invoice);
	
};

#endif
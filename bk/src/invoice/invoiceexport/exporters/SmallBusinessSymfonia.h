#ifndef __SMALLBUSINESSSYMFONIA_H__
#define __SMALLBUSINESSSYMFONIA_H__

#include "InvoiceExporter.h"
#include "sagejson.h"

class SmallBusinessSymfonia : public InvoiceExporter
{	
	void WriteObject(std::wofstream& output, const sagejson::object& o);
	std::wstring FormatFiscalNo(const std::wstring& fiscalNo);
public:
	virtual std::wstring GetExporterName();
	virtual std::wstring GetFileExtension();
	virtual std::string GetDefaultEncoding();

	virtual void Export(  IInvoiceRange invoices,
										InvoiceExporterOptions& options,
										std::wofstream& output,
										SeasonSession* session,
										SimpleProgress *progress);
	
};

#endif
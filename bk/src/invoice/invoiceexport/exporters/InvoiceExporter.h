#ifndef __INVOICEEXPORTER_H__
#define __INVOICEEXPORTER_H__

#include <progress/SimpleProgress.h>
#include <boost\range\any_range.hpp>
#include "options\InvoiceExporterOptions.h"
#include <data\IInvoice.h>
#include "../../../context/SeasonSession.h"

typedef boost::any_range<IInvoice*, boost::random_access_traversal_tag, IInvoice*, std::ptrdiff_t> IInvoiceRange;

class InvoiceExporter
{
public:
	virtual std::wstring GetExporterName() = 0;
	virtual std::wstring GetFileExtension() = 0;
	virtual std::string GetDefaultEncoding() = 0;

	virtual void Export(IInvoiceRange invoices,
						InvoiceExporterOptions& options,
						std::wofstream& output,
						SeasonSession* session,
						SimpleProgress *progress) = 0;
	virtual ~InvoiceExporter() {};
};

#endif
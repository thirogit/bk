#ifndef __FATINVOICEVIEW_H__
#define __FATINVOICEVIEW_H__

#include <stdint.h>
#include "CowInvoiceEntryView.h"
#include <vector>
#include "InvoiceView.h"

class FatInvoiceView : public InvoiceView
{
public:
	FatInvoiceView(uint32_t invoiceId);
	FatInvoiceView(const FatInvoiceView& src);
	~FatInvoiceView();
		
	CowInvoiceEntryView* AddEntry(uint32_t cowId);	
	size_t GetEntryCount() const;
	const CowInvoiceEntryView* GetEntryAt(int index) const;	
private:
	
	std::vector<CowInvoiceEntryView*> m_entries;
	
};

#endif
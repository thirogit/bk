#pragma once

#include "InvoiceCowItem.h"
#include <boost/range/any_range.hpp>
#include <data/impl/InvoiceDetails.h>
#include <data/impl/InvoiceHent.h>
#include <data/impl/InvoiceDeduction.h>
#include <vector>

typedef boost::any_range<const InvoiceCowItem*,boost::forward_traversal_tag, InvoiceCowItem* const, std::ptrdiff_t> InvoiceSnapshotItemsRange;
typedef boost::any_range<const InvoiceDeduction*, boost::forward_traversal_tag, InvoiceDeduction* const, std::ptrdiff_t> InvoiceSnapshotDeductionsRange;

class InvoiceSnapshot
{
public:
	InvoiceSnapshot(uint32_t invoiceId);
	~InvoiceSnapshot();

	void SetDetails(const InvoiceDetails& details);
	const InvoiceDetails& GetDetails() const;

	void SetInvoiceType(InvoiceType type);
	InvoiceType GetInvoiceType() const;

	void SetInvoiceHent(const InvoiceHent& hent);
	const InvoiceHent& GetInvoiceHent() const;

	const InvoiceCowItem& GetItem(int index) const;
	void AddItem(const InvoiceCowItem& item);	
	InvoiceSnapshotItemsRange Items() const;


	const InvoiceDeduction& GetDeduction(int index) const;
	void AddDeduction(const InvoiceDeduction& item);
	InvoiceSnapshotDeductionsRange Deductions() const;

	uint32_t GetInvoiceId() const;
	
private:
	InvoiceType m_invoicetype;
	uint32_t m_invoiceId;
	InvoiceHent m_hent;
	InvoiceDetails m_details;
	std::vector<InvoiceCowItem*> m_items;
	std::vector<InvoiceDeduction*> m_deductions;
};

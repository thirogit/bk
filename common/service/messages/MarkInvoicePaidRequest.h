#ifndef __MARKINVOICEPAIDREQUEST_H__
#define __MARKINVOICEPAIDREQUEST_H__

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "../../types/DateTime.h"
#include "../../data/IIdable.h"

class MarkInvoicePaidRequest
{
public:
	MarkInvoicePaidRequest(uint32_t invoiceId = NULL_ID);
	MarkInvoicePaidRequest& WithPaidDate(const DateTime& paidDt);	
	const DateTime& GetPaidDt() const;
	void serialize(SF::Archive &archive);	
	uint32_t GetInvoiceId() const;
private:
	DateTime m_paidDt;
	uint32_t m_invoiceId;
};

typedef boost::shared_ptr<MarkInvoicePaidRequest> MarkInvoicePaidRequestObj;

#endif

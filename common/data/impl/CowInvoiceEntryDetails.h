#ifndef __COWINVOICEENTRYDETAILS_H__
#define __COWINVOICEENTRYDETAILS_H__

#include <types/Decimal.h>
#include "../ICowInvoiceEntry.h"
#include  <casajson/json.h>

class SerializableCowInvoiceEntryDetails;

class CowInvoiceEntryDetails
{
public:
	CowInvoiceEntryDetails();

	void CopyFrom(const CowInvoiceEntryDetails& src);
	void CopyFrom(const ICowInvoiceEntry* src);
	
	uint32_t GetClassId() const;
	const Decimal&	GetWeight() const;
	const Decimal&	GetPrice() const;
	
	void SetClassId(uint32_t classId);
	void SetWeight(const Decimal& weight);
	void SetPrice(const Decimal& price);

	static CowInvoiceEntryDetails FromJson(const json::value& value);
	json::value ToJson() const;

protected:		
	uint32_t	m_classId;
	Decimal		m_Weight;
	Decimal		m_Price;	

	friend class SerializableCowInvoiceEntryDetails;
};

#endif

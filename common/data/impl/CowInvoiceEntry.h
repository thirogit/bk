#ifndef __COWINVOICEENTRY_H__
#define __COWINVOICEENTRY_H__

#include "../ICowInvoiceEntry.h"
#include "CowInvoiceEntryDetails.h"
#include  <casajson/json.h>

class SerializableCowInvoiceEntry;

class CowInvoiceEntry : public ICowInvoiceEntry
{
public:
	CowInvoiceEntry(uint32_t cowId);
	CowInvoiceEntry(const CowInvoiceEntry& src);
	CowInvoiceEntry(const ICowInvoiceEntry* src);

	void CopyFrom(const ICowInvoiceEntry* src);

	virtual uint32_t		GetCowId() const;
	virtual uint32_t		GetClassId() const;
	virtual const Decimal&	GetWeight() const;
	virtual const Decimal&	GetPrice() const;
	
	void			SetClassId(uint32_t classId);
	void			SetWeight(const Decimal& weight);
	void			SetPrice(const Decimal& price);
	void			SetFrom(const CowInvoiceEntryDetails& details);
		

	static CowInvoiceEntry FromJson(const json::value& value);
	json::value ToJson() const;
protected:
	
	uint32_t	m_cowId;
	CowInvoiceEntryDetails m_details;

	friend class SerializableCowInvoiceEntry;
};

#endif
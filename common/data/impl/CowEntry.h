#ifndef __COWENTRY_H__
#define __COWENTRY_H__

#include "../ICowEntry.h"
#include "CowEntryDetails.h"

class SerializableCowEntry;

class CowEntry : public ICowEntry
{
public:		
	CowEntry(uint32_t cowId);

	void CopyFrom(ICowEntry* pSrc);

	void SetFrom(const CowEntryDetails& details);
	const CowEntryDetails& GetDetails() const;

	virtual const NullDecimal&	GetWeight() const;	
	virtual uint32_t			GetStockId() const;	
	virtual uint32_t			GetClassId() const;			
	virtual const NullInt&		GetGroup() const;			
	virtual uint32_t			GetCowId() const;
	
	void SetWeight(const NullDecimal& weight);	
	void SetStockId(uint32_t stockId);	
	void SetClassId(uint32_t classId);		
	void SetGroup(const NullInt& group);			
	
private:
	CowEntryDetails	m_details;		
	uint32_t 	m_cowId;

	friend class SerializableCowEntry;
};



#endif
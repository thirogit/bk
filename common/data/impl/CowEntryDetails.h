#ifndef __COWENTRYDETAILS_H__
#define __COWENTRYDETAILS_H__

#include "../../types/NullDecimal.h"
#include "../../types/NullInt.h"
#include "../ICowEntry.h"

class SerializableCowEntryDetails;

class CowEntryDetails
{
public:		
	CowEntryDetails();
	void CopyFrom(const CowEntryDetails& src);
	void CopyFrom(const ICowEntry* pSrc);

	const NullDecimal&	GetWeight() const;	
	uint32_t			GetStockId() const;	
	uint32_t			GetClassId() const;			
	const NullInt&		GetGroup() const;			
		
	void SetWeight(const NullDecimal& weight);	
	void SetStockId(uint32_t stockId);	
	void SetClassId(uint32_t classId);		
	void SetGroup(const NullInt& group);			
	
private:

	NullDecimal m_weight;
	uint32_t	m_stockId;	
	uint32_t 	m_classId;		
	NullInt 	m_group;			

	friend class SerializableCowEntryDetails;
};



#endif
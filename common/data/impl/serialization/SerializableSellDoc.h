#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../SellDoc.h"

class SerializableSellDoc
{
public:
	SerializableSellDoc(SellDoc* pDoc);	
	void serialize(SF::Archive &archive);
protected:
	DocDetails& Details();	
	DocContent& Content();	
	uint32_t& Id();
	uint32_t& HerdId();
	uint32_t& HentId();
	uint32_t& InvoiceId();

private:
	SellDoc* m_pDoc;
};

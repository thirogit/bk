#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../InvoiceContent.h"

class SerializableInvoiceContent
{
public:
	SerializableInvoiceContent(InvoiceContent& content);	
	void serialize(SF::Archive &archive);
private:
	InvoiceContent& m_Content;

	
};

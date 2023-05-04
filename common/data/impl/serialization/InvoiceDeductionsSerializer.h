#pragma once 

#include <SF/Archive.hpp>

#include "../InvoiceDeductions.h"

class InvoiceDeductionsSerializer
{
public:
	InvoiceDeductionsSerializer(InvoiceDeductions& deductions);
	void serialize(SF::Archive &archive);
private:
	InvoiceDeductions& m_Deductions;

	
};

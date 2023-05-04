#pragma once 

#include <SF/Archive.hpp>
#include <SF/string.hpp> 

#include "../InvoiceDeduction.h"

class InvoiceDeductionSerializer
{
public:
	InvoiceDeductionSerializer(InvoiceDeduction& deduction);
	void serialize(SF::Archive &archive);
private:
	InvoiceDeduction& m_Deduction;

	
};

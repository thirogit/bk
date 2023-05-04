#ifndef __INVOICEDEDUCTIONS_H__
#define __INVOICEDEDUCTIONS_H__

#include "InvoiceDeduction.h"
#include <configuration/deductions/IDeductionDefinition.h>
#include "../../CommonTypes.h"
#include "../../arrays/Enumerator.h"
#include <vector>
#include  <casajson/json.h>

class InvoiceDeductionsSerializer;

class InvoiceDeductions
{
public:
	InvoiceDeductions();
	~InvoiceDeductions();

	count_t GetCount() const;
	PtrEnumerator<IInvoiceDeduction> EnumDeductions() const;
	PtrEnumerator<InvoiceDeduction> EnumDeductions();
	
	InvoiceDeduction* GetDeductionAt(int index) const;
	void RemoveAt(int index);
	void RemoveAll();
	InvoiceDeduction* AddDeduction(const IDeductionDefinition* definition);
	InvoiceDeduction* AddDeduction(const IInvoiceDeduction* deduction);
	void AppendDeduction(InvoiceDeduction* definition);
	
	static InvoiceDeductions FromJson(const json::value& value);
	json::value ToJson() const;

protected:
	std::vector<InvoiceDeduction*> m_Deductions;

	friend class InvoiceDeductionsSerializer;
	
};


#endif
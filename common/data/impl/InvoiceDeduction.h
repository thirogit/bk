#ifndef __INVIOCEDEDUCTION_H__
#define __INVIOCEDEDUCTION_H__

#include "../IInvoiceDeduction.h"
#include "../../configuration/deductions/DeductionDetails.h"
#include "../../configuration/deductions/IDeductionDefinition.h"
#include  <casajson/json.h>

class InvoiceDeductionSerializer;

class InvoiceDeduction : public IInvoiceDeduction
{
public:
	InvoiceDeduction();
	InvoiceDeduction(const InvoiceDeduction& src);
	InvoiceDeduction(const IInvoiceDeduction* src);
	InvoiceDeduction(const IDeductionDefinition* src);
	void CopyFrom(const IInvoiceDeduction* src);
	InvoiceDeduction& operator=(const InvoiceDeduction& right);
	void CopyFrom(const DeductionDetails& details);

	virtual const std::wstring& GetCode() const;	
	virtual const std::wstring& GetReason() const;	
	virtual const Decimal& GetFraction() const;	
	virtual bool GetIsEnabled() const;
	virtual DeductionDetails GetDetails() const;

	void SetCode(const std::wstring& sCode);
	void SetReason(const std::wstring& sReason);
	void SetFraction(const Decimal&	fraction);
	void SetIsEnabled(bool bEnabled);

	static InvoiceDeduction FromJson(const json::value& value);
	json::value ToJson() const;

private:
	
	DeductionDetails m_details;
	bool m_enabled;

	friend class InvoiceDeductionSerializer;

};


#endif

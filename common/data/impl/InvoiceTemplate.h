#ifndef __INVOICETEMPLATE_H__
#define __INVOICETEMPLATE_H__

#include "../IInvoice.h"
#include "InvoiceDetails.h"
#include "InvoiceContent.h"
#include "InvoiceHent.h"
#include "InvoiceDeductions.h"

template<class INVOICEINTERFACE>
class InvoiceTemplate : public INVOICEINTERFACE
{
public:
	InvoiceTemplate(uint32_t id,InvoiceScope scope) : m_invoiceid(id),m_invoicescope(scope),m_correctfor(NULL_ID), m_invoicetype(InvoiceType_None)
	{
	}	
	
	virtual ~InvoiceTemplate() {}
	
	void CopyFrom(const IInvoice* pSrc)
	{
		m_invoicescope = pSrc->GetInvoiceScope();
		m_invoiceno = pSrc->GetInvoiceNo();
		m_correctfor = pSrc->GetCorrectFor();
		m_Details.CopyFrom(pSrc);
		m_Hent.CopyFrom(pSrc->GetInvoiceHent());
		m_invoicetype = pSrc->GetInvoiceType();

		m_Content.RemoveAll();

		const ICowInvoiceEntry* pSrcEntry = NULL;
		for(int i = 0,count = pSrc->GetCowQty(); i < count;i++)
		{
			pSrcEntry = pSrc->GetCowAt(i);
			m_Content.AddEntry(pSrcEntry->GetCowId())->CopyFrom(pSrcEntry);
		}	

		m_Deductions.RemoveAll();
	
		PtrEnumerator<IInvoiceDeduction> deductionEnum = pSrc->EnumDeductions();
		while (deductionEnum.hasNext())
		{
			m_Deductions.AddDeduction(*deductionEnum);
			deductionEnum.advance();
		}
	}

	virtual uint32_t GetId() const
	{
		return m_invoiceid;
	}
	
	virtual uint32_t GetInvoiceNo() const
	{
		return m_invoiceno;	
	}
	
	virtual uint32_t GetCorrectFor() const
	{
		return m_correctfor;
	}
	
	virtual InvoiceType GetInvoiceType() const
	{
		return m_invoicetype;
	}
		
	virtual const std::wstring& GetExtras() const
	{
		return m_Details.GetExtras();
	}
	virtual const DateTime& GetInvoiceDate() const
	{
		return m_Details.GetInvoiceDate();
	}
	
	virtual const DateTime& GetPaidDate() const
	{
		return m_Details.GetPaidDate();
	}
	
	virtual const Decimal& GetVATRate() const
	{
		return m_Details.GetVATRate();
	}
	
	virtual PayWay GetPayWay() const
	{
		return m_Details.GetPayWay();
	}
	
	virtual const std::wstring& GetCustomNumber() const
	{
		return m_Details.GetCustomNumber();
	}
	
	virtual const std::wstring& GetTransactionPlace() const
	{
		return m_Details.GetTransactionPlace();
	}
	
	virtual const DateTime& GetTransactionDate() const
	{
		return m_Details.GetTransactionDate();
	}
	
	virtual int GetPayDueDays() const
	{
		return m_Details.GetPayDueDays();
	}
		
	virtual int GetCowQty() const
	{
		return m_Content.GetCount();
	}

	virtual InvoiceScope GetInvoiceScope() const
	{
		return m_invoicescope;
	}
	
	virtual ICowInvoiceEntry* GetCow(uint32_t cowId) const
	{
		return m_Content.GetEntry(cowId);
	}

	virtual ICowInvoiceEntry* GetCowAt(uint32_t index) const
	{
		return m_Content.GetEntryAt(index);
	}

	virtual const IInvoiceHent* GetInvoiceHent() const
	{
		return &m_Hent;
	}

	void SetInvoiceHent(const InvoiceHent& hent) 
	{
		m_Hent.CopyFrom(&hent);
	}

	CowInvoiceEntry* AddEntry(uint32_t cowId)
	{
		return m_Content.AddEntry(cowId);
	}

	void AddEntry(CowInvoiceEntry* entry)
	{
		return m_Content.AddEntry(entry);
	}
	
	void SetInvoiceNo(uint32_t invoiceNo)
	{
		m_invoiceno = invoiceNo;	
	}
	
	void SetCorrectFor(uint32_t invoiceId)
	{
		m_correctfor = invoiceId;	
	}
	
	void SetInvoiceType(InvoiceType invoiceType)
	{
		m_invoicetype = invoiceType;
	}
	
	void SetExtras(const std::wstring& sExtras)
	{
		m_Details.SetExtras(sExtras);
	}
	
	void SetInvoiceDate(const DateTime& invoiceDt)
	{
		m_Details.SetInvoiceDate(invoiceDt);
	}
	
	void SetPaidDate(const DateTime& paidDt)
	{
		m_Details.SetPaidDate(paidDt);
	}
	
	void SetVATRate(const Decimal& vatRate)
	{
		m_Details.SetVATRate(vatRate);
	}
	
	void SetPayWay(PayWay payWay)
	{
		m_Details.SetPayWay(payWay);
	}
	
	void SetCustomNumber(const std::wstring& sCustomNumber)
	{
		m_Details.SetCustomNumber(sCustomNumber);
	}
	
	void SetTransactionPlace(const std::wstring& sTransactionPlace)
	{
		m_Details.SetTransactionPlace(sTransactionPlace);
	}
	
	void SetTransactionDate(const DateTime& transactionDt)
	{
		m_Details.SetTransactionDate(transactionDt);
	}
	
	void SetPayDueDays(int payDueDays)
	{
		m_Details.SetPayDueDays(payDueDays);
	}

	void SetFrom(const InvoiceDetails& details)
	{
		m_Details.CopyFrom(details);
	}

	void SetFrom(const IInvoice* pInvoice)
	{
		m_invoiceno = pInvoice->GetInvoiceNo();	
		m_Hent.CopyFrom(pInvoice->GetInvoiceHent());
		m_Details.CopyFrom(pInvoice);
	}

	PtrEnumerator<ICowInvoiceEntry> EnumEntries() const
	{
		return m_Content.EnumEntries();
	}

	CowInvoiceEntry* GetEntry(uint32_t cowId)
	{
		return m_Content.GetEntry(cowId);
	}
	
	void RemoveEntry(uint32_t cowId)
	{
		m_Content.RemoveEntry(cowId);
	}
	
	virtual PtrEnumerator<IInvoiceDeduction> EnumDeductions() const 
	{
		return m_Deductions.EnumDeductions();
	}

	InvoiceDeduction* AddDeduction(const IDeductionDefinition* definition)
	{
		return m_Deductions.AddDeduction(definition);
	}

	void AddDeduction(const IInvoiceDeduction* deduction)
	{
		m_Deductions.AddDeduction(deduction);
	}

	void AppendDeduction(InvoiceDeduction* definition)
	{
		m_Deductions.AppendDeduction(definition);
	}

	void RemoveAllDeductions()
	{
		m_Deductions.RemoveAll();
	}

protected:
	uint32_t 	m_invoiceno;
	uint32_t 	m_invoiceid;
	uint32_t 	m_correctfor;
	InvoiceType m_invoicetype;
	
	InvoiceDeductions m_Deductions;
	InvoiceScope m_invoicescope;
	InvoiceDetails m_Details;
	InvoiceContent m_Content;
	InvoiceHent m_Hent;	
	
};

#endif
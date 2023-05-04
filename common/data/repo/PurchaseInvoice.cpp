#include "stdafx.h"
#include "PurchaseInvoice.h"
#include <arrays/DeepDelete.h>
#include <string/TextUtils.h>

PurchaseInvoice::PurchaseInvoice(const std::wstring& id) : m_PayWay(PayWay_None),
												m_PayDueDays(0), 
												m_InvoiceType(InvoiceType_None), 
												m_id(id),
												m_InvoiceHent(L"")
{
}

void PurchaseInvoice::CopyFrom(const IPurchaseInvoice* src) 
{
	DeepDelete(m_Cows);
	m_Cows.clear();
	DeepDelete(m_Deductions);
	m_Deductions.clear();

	m_InvoiceDate = src->GetInvoiceDate();
	m_TransactionDate = src->GetTransactionDate();
	m_TransactionPlace = src->GetTransactionPlace();
	m_VatRate = src->GetVatRate();
	m_PayWay = src->GetPayWay();
	m_CustomNumber = src->GetCustomNumber();
	m_PayDueDays = src->GetPayDueDays();
	m_InvoiceHent.CopyFrom(src->GetInvoiceHent());
	m_InvoiceType = src->GetInvoiceType();

	PtrEnumerator<IPurchaseCow> cowEnum = src->EnumCows();

	while (cowEnum.hasNext())
	{
		AddCow(*(cowEnum));
		cowEnum.advance();
	}

	PtrEnumerator<IPurchaseInvoiceDeduction> deductionsEnum = src->EnumDeductions();

	while (deductionsEnum.hasNext())
	{
		AddDeduction(*(deductionsEnum));
		deductionsEnum.advance();
	}
}

PurchaseInvoice::~PurchaseInvoice()
{
	DeepDelete(m_Cows);
	DeepDelete(m_Deductions);
}


const std::wstring& PurchaseInvoice::GetId() const
{
	return m_id;
}

const DateTime& PurchaseInvoice::GetInvoiceDate() const
{
	return m_InvoiceDate;
}

const DateTime& PurchaseInvoice::GetTransactionDate() const
{
	return m_TransactionDate;
}
	
const std::wstring& PurchaseInvoice::GetTransactionPlace() const
{
	return m_TransactionPlace;
}
	
const Decimal& PurchaseInvoice::GetVatRate() const
{
	return m_VatRate;
}

PayWay PurchaseInvoice::GetPayWay() const
{
	return m_PayWay;
}

const std::wstring& PurchaseInvoice::GetCustomNumber() const
{
	return m_CustomNumber;
}
	
int PurchaseInvoice::GetPayDueDays() const
{
	return m_PayDueDays;
}
	
IPurchaseHent* PurchaseInvoice::GetInvoiceHent() const
{
	PurchaseInvoice* nonconst_this = const_cast<PurchaseInvoice*>(this);
	return &nonconst_this->m_InvoiceHent;
}

void PurchaseInvoice::SetInvoiceType(InvoiceType invoiceType)
{
	m_InvoiceType = invoiceType;
}

void PurchaseInvoice::SetInvoiceDate(const DateTime& invoiceDt)
{
	m_InvoiceDate = invoiceDt;
}

void PurchaseInvoice::SetTransactionDate(const DateTime& transactionDt)
{
	m_TransactionDate = transactionDt;
}
	
void PurchaseInvoice::SetTransactionPlace(const std::wstring& sTransactionPlace)
{
	m_TransactionPlace = sTransactionPlace;
}
	
void PurchaseInvoice::SetVatRate(const Decimal& vatRate)
{
	m_VatRate = vatRate;
}

void PurchaseInvoice::SetPayWay(PayWay payWay)
{
	m_PayWay = payWay;
}

void PurchaseInvoice::SetCustomNumber(const std::wstring& sCustomNumber)
{
	m_CustomNumber = sCustomNumber;
}
	
void PurchaseInvoice::SetPayDueDays(int payDueDays)
{
	m_PayDueDays = payDueDays;
}
	
void PurchaseInvoice::SetInvoiceHent(IPurchaseHent* pInvoiceHent)
{
	m_InvoiceHent.CopyFrom(pInvoiceHent);
}

int PurchaseInvoice::GetCowCount() const
{
	return m_Cows.size();
}

PurchaseCow* PurchaseInvoice::GetCowAt(int iCow)
{
	return m_Cows.at(iCow);
}

const IPurchaseCow* PurchaseInvoice::GetCowAt(int iCow) const
{
	return const_cast<PurchaseInvoice*>(this)->GetCowAt(iCow);
}

std::wstring PurchaseInvoice::ObtainNewCowId() const
{
	std::wstring nextId = m_idGenerator.Generate();
	while (std::find_if(m_Cows.begin(), m_Cows.end(), [nextId](PurchaseCow* cow) -> bool {	return cow->GetId() == nextId;  }) != m_Cows.end())
	{
		nextId = m_idGenerator.Generate();
	}
	return nextId;
	
}

std::wstring PurchaseInvoice::ObtainNewDeductionId() const
{
	std::wstring nextId = m_idGenerator.Generate();
	while (std::find_if(m_Deductions.begin(), m_Deductions.end(), [nextId](PurchaseInvoiceDeduction* deduction) -> bool {	return deduction->GetId() == nextId;  }) != m_Deductions.end())
	{
		nextId = m_idGenerator.Generate();
	}
	return nextId;
}

PurchaseCow* PurchaseInvoice::AddCow(IPurchaseCow* pCow)
{
	PurchaseCow* cow = new PurchaseCow(ObtainNewCowId());
	cow->CopyFrom(pCow);
	m_Cows.add(cow);
	return cow;
}

PurchaseCow* PurchaseInvoice::AddCow()
{
	PurchaseCow* cow = new PurchaseCow(ObtainNewCowId());	
	m_Cows.add(cow);
	return cow;
}

InvoiceType PurchaseInvoice::GetInvoiceType() const
{
	return m_InvoiceType;
}

PtrEnumerator<IPurchaseCow> PurchaseInvoice::EnumCows() const
{
	return PtrEnumerator<IPurchaseCow>(m_Cows);
}


PtrEnumerator<IPurchaseInvoiceDeduction> PurchaseInvoice::EnumDeductions() const
{
	return PtrEnumerator<IPurchaseInvoiceDeduction>(m_Deductions);
}

int PurchaseInvoice::GetDeductionsCount() const
{
	return m_Deductions.size();
}

PurchaseInvoiceDeduction* PurchaseInvoice::GetDeductionAt(int index)
{
	return m_Deductions.at(index);
}

const IPurchaseInvoiceDeduction* PurchaseInvoice::GetDeductionAt(int index) const
{
	return const_cast<PurchaseInvoice*>(this)->m_Deductions.at(index);
}

PurchaseInvoiceDeduction* PurchaseInvoice::AddDeduction(IPurchaseInvoiceDeduction* pDeduction)
{
	PurchaseInvoiceDeduction* newDeduction = new PurchaseInvoiceDeduction(ObtainNewDeductionId());
	newDeduction->CopyFrom(pDeduction);
	m_Deductions.add(newDeduction);
	return newDeduction;
}

PurchaseInvoiceDeduction* PurchaseInvoice::AddDeduction()
{
	PurchaseInvoiceDeduction* newDeduction = new PurchaseInvoiceDeduction(ObtainNewDeductionId());
	m_Deductions.add(newDeduction);
	return newDeduction;
}

PurchaseCow* PurchaseInvoice::GetCow(const std::wstring& id)
{
	return m_Cows.find(id);
}

IPurchaseCow* PurchaseInvoice::GetCow(const std::wstring& id) const
{
	return const_cast<PurchaseInvoice*>(this)->m_Cows.find(id);
}

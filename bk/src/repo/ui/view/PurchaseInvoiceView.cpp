#include "stdafx.h"
#include "PurchaseInvoiceView.h"
#include <data\IIdable.h>

PurchaseInvoiceView::PurchaseInvoiceView(const std::wstring& id) :
			m_PayWay(PayWay_None),
			m_InvoiceType(InvoiceType_None),
			m_CowCount(0),
			m_invoiceId(id),
			m_InvoiceHent(L"")
{
}
	
const std::wstring& PurchaseInvoiceView::GetInvoiceId() const
{
	return m_invoiceId;
}

const DateTime& PurchaseInvoiceView::GetInvoiceDate() const
{
	return m_InvoiceDate;
}

const Decimal& PurchaseInvoiceView::GetVatRate() const
{
	return m_VatRate;
}

PayWay PurchaseInvoiceView::GetPayWay() const
{
	return m_PayWay;
}

const std::wstring& PurchaseInvoiceView::GetCustomNumber() const
{
	return m_CustomNumber;
}

const PurchaseHentView* PurchaseInvoiceView::GetInvoiceHent() const
{
	return &m_InvoiceHent;
}

InvoiceType PurchaseInvoiceView::GetInvoiceType() const
{
	return m_InvoiceType;
}

int PurchaseInvoiceView::GetCowCount() const 
{
	return m_CowCount;
}

const Decimal& PurchaseInvoiceView::GetTotalNet() const
{
	return m_TotalNet;
}

void PurchaseInvoiceView::SetInvoiceDate(const DateTime& invoiceDt)
{
	m_InvoiceDate = invoiceDt;
}

void PurchaseInvoiceView::SetVatRate(const Decimal& vatRate)
{
	m_VatRate = vatRate;
}

void PurchaseInvoiceView::SetPayWay(PayWay payWay)
{
	m_PayWay = payWay;
}

void PurchaseInvoiceView::SetCustomNumber(const std::wstring& customNumber)
{
	m_CustomNumber = customNumber;
}

void PurchaseInvoiceView::SetInvoiceHent(const PurchaseHentView* invoiceHent)
{
	m_InvoiceHent.CopyFrom(*invoiceHent);
}

void PurchaseInvoiceView::SetInvoiceType(InvoiceType invoiceType)
{
	m_InvoiceType = invoiceType;
}

void PurchaseInvoiceView::SetCowCount(int cowCount)
{
	m_CowCount	= cowCount;
}

void PurchaseInvoiceView::SetTotalNet(const Decimal& totalNet)
{
	m_TotalNet = totalNet;
}

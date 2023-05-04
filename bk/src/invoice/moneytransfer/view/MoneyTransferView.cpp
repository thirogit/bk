#include "stdafx.h"
#include "MoneyTransferView.h"

MoneyTransferView::MoneyTransferView(uint32_t invoiceId) : m_invoiceId(invoiceId)
{
}
	
uint32_t MoneyTransferView::GetInvoiceId() const
{
	return m_invoiceId;
}
	
void MoneyTransferView::SetHentAlias(const std::wstring& hentAlias)
{
	m_hentAlias = hentAlias;
}

void MoneyTransferView::SetInvoiceNo(const std::wstring& invoiceNo)
{
	m_invoiceNo = invoiceNo;
}

void MoneyTransferView::SetCustomInvoiceNo(const std::wstring& customInvoiceNo)
{
	m_customInvoiceNo = customInvoiceNo;
}

void MoneyTransferView::SetTransferValue(const Decimal& transferValue)
{
	m_transferValue = transferValue;
}

void MoneyTransferView::SetStatusDesc(const ColoredObject<std::wstring>& statusDesc)
{
	m_statusDesc = statusDesc;
}

const std::wstring& MoneyTransferView::GetHentAlias() const
{
	return m_hentAlias;
}

const std::wstring& MoneyTransferView::GetInvoiceNo() const
{
	return m_invoiceNo;
}

const std::wstring& MoneyTransferView::GetCustomInvoiceNo()  const
{
	return m_customInvoiceNo;
}

const Decimal& MoneyTransferView::GetTransferValue()  const
{
	return m_transferValue;
}

const ColoredObject<std::wstring>& MoneyTransferView::GetStatusDesc()  const
{
	return m_statusDesc;
}

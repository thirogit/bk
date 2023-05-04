#include "stdafx.h"
#include "HentsDocView.h"

HentsDocView::HentsDocView(uint32_t docId) : DocView(docId)
{
}

const std::wstring& HentsDocView::GetHerdCd() const
{
	return m_HerdCd;
}

void HentsDocView::SetHerdCd(const std::wstring& herdCd)
{
	m_HerdCd = herdCd;
}

const std::wstring& HentsDocView::GetHentAlias() const
{
	return m_HentAlias;
}

void HentsDocView::SetHentAlias(const std::wstring& sHentAlias)
{
	m_HentAlias = sHentAlias;
}

const std::wstring& HentsDocView::GetInvoiceNo() const
{
	return m_InvoiceNo;
}

void HentsDocView::SetInvoiceNo(const std::wstring& sInvoiceNo)
{
	m_InvoiceNo = sInvoiceNo;
}
	

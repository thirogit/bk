#include "stdafx.h"
#include "OwnDocView.h"

OwnDocView::OwnDocView(uint32_t docId) : DocView(docId)
{
}


const std::wstring& OwnDocView::GetHerdCd() const
{
	return m_HerdCd;
}

void OwnDocView::SetHerdCd(const std::wstring& herdCd)
{
	m_HerdCd = herdCd;
}
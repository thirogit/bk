#include "stdafx.h"
#include "MoveDocView.h"

MoveDocView::MoveDocView(uint32_t docId) : DocView(docId)
{
}

const std::wstring&	MoveDocView::GetSrcHerdCd() const
{
	return m_SrcHerdCd;
}

void MoveDocView::SetSrcHerdCd(const std::wstring& srcHerdCd)
{
	m_SrcHerdCd = srcHerdCd;
}

const std::wstring&	MoveDocView::GetDstHerdCd() const
{
	return m_DstHerdCd;
}

void MoveDocView::SetDstHerdCd(const std::wstring& dstHerdCd)
{
	m_DstHerdCd = dstHerdCd;
}


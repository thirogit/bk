#include "stdafx.h"
#include "CursorDescriptor.h"

namespace repo 
{

CursorDescriptor::CursorDescriptor(const std::wstring& cursorId) : m_CursorId(cursorId)
{
	m_Count = 0;
	m_MinFetch = 0;
	m_MaxFetch = 0;
}

CursorDescriptor::CursorDescriptor(const CursorDescriptor& src) : m_CursorId(src.m_CursorId)
{
	CopyFrom(src);
}

CursorDescriptor& CursorDescriptor::operator=(const CursorDescriptor& right)
{
	m_CursorId = right.m_CursorId;
	CopyFrom(right);
	return *this;
}

void CursorDescriptor::CopyFrom(const CursorDescriptor& src)
{
	m_Count = src.m_Count;
	m_MinFetch = src.m_MinFetch;
	m_MaxFetch = src.m_MaxFetch;
}

const std::wstring& CursorDescriptor::GetCursorId() const
{
	return m_CursorId;
}

int CursorDescriptor::GetCount() const
{
	return m_Count;
}

int CursorDescriptor::GetMinFetch() const
{
	return m_MinFetch;
}

int CursorDescriptor::GetMaxFetch() const
{
	return m_MaxFetch;
}

void CursorDescriptor::SetCount(int count)
{
	m_Count = count;
}

void CursorDescriptor::SetMinFetch(int minFetch)
{
	m_MinFetch = minFetch;
}

void CursorDescriptor::SetMaxFetch(int maxFetch)
{
	m_MaxFetch = maxFetch;
}

}
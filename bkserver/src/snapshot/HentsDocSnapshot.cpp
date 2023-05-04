#include "stdafx.h"
#include "HentsDocSnapshot.h"
#include <sstream>

HentsDocSnapshot::HentsDocSnapshot(uint32_t docId) : DocSnapshot(docId),m_herdId(NULL_ID),m_hentId(NULL_ID)
{
}
	
void HentsDocSnapshot::SetHerdId(uint32_t herdId)
{
	m_herdId = herdId;
}

uint32_t HentsDocSnapshot::GetHerdId() const
{
	return m_herdId;
}

void HentsDocSnapshot::SetHentId(uint32_t hentId)
{
	m_hentId = hentId;
}

uint32_t HentsDocSnapshot::GetHentId() const
{
	return m_hentId;
}
	

std::wstring HentsDocSnapshot::ToString() const
{
	std::wstringstream s;

	s << L"herdId=" << this->GetHerdId() << L", ";
	s << L"hentId=" << this->GetHentId() << L", ";
	s << DocSnapshot::ToString();

	return s.str();
}

#include "stdafx.h"
#include "OwnDocSnapshot.h"
#include <sstream>

OwnDocSnapshot::OwnDocSnapshot(uint32_t docId) : DocSnapshot(docId),m_herdId(NULL_ID)
{
}
	
void OwnDocSnapshot::SetHerdId(uint32_t herdId)
{
	m_herdId = herdId;
}

uint32_t OwnDocSnapshot::GetHerdId() const
{
	return m_herdId;
}

std::wstring OwnDocSnapshot::ToString() const
{
	std::wstringstream s;
	
	s << L"herdId=" << this->GetHerdId() << L", ";
	s << DocSnapshot::ToString();
	
	return s.str();
}


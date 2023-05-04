#include "stdafx.h"
#include "CowPresenceKey.h"

CowPresenceKey::CowPresenceKey(uint32_t cowId,DocType docType,uint32_t docId) : m_cowId(cowId),m_docId(docId),m_docType(docType)
{
}

CowPresenceKey::CowPresenceKey(const CowPresenceKey& src)
{
	operator=(src);
}

uint32_t CowPresenceKey::GetCowId() const
{
	return m_cowId;
}

DocType CowPresenceKey::GetDocType() const
{
	return m_docType;
}

uint32_t CowPresenceKey::GetDocId() const
{
	return m_docId;
}

CowPresenceKey& CowPresenceKey::operator=(const CowPresenceKey& right)
{
	m_cowId = right.m_cowId;
	m_docId = right.m_docId;
	m_docType = right.m_docType;
	return *this;
}
	
bool operator==(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	return left.m_cowId == right.m_cowId && left.m_docType == right.m_docType && left.m_docId == right.m_docId;
}

bool operator!=(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	return !operator==(left,right);
}

bool operator>(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	if(left.m_cowId == right.m_cowId)
	{
		if(left.m_docType == right.m_docType)
		{
			return left.m_docId > right.m_docId;
		}
		return left.m_docType > right.m_docType;
	}
	return left.m_cowId > right.m_cowId;
}

bool operator<(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	return right > left;
}

bool operator>=(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	if(left == right)
		return true;

	return left > right;
}

bool operator<=(const CowPresenceKey &left ,const CowPresenceKey &right)
{
	if(left == right)
		return true;

	return left < right;
}
	

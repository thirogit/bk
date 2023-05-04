#pragma once

#include <data/DocType.h>
#include <cstdint>

class CowPresenceKey
{
public:	
	CowPresenceKey(uint32_t cowId,DocType docType,uint32_t docId);
	CowPresenceKey(const CowPresenceKey& src);
	CowPresenceKey& operator=(const CowPresenceKey& right);

	uint32_t GetCowId() const;
	DocType GetDocType() const;
	uint32_t GetDocId() const;
private:
	uint32_t m_cowId;
	DocType m_docType;
	uint32_t m_docId;
	
	friend bool operator==(const CowPresenceKey &left ,const CowPresenceKey &right);
	friend bool operator!=(const CowPresenceKey &left ,const CowPresenceKey &right);
	friend bool operator>(const CowPresenceKey &left ,const CowPresenceKey &right);
	friend bool operator<(const CowPresenceKey &left ,const CowPresenceKey &right);
	friend bool operator>=(const CowPresenceKey &left ,const CowPresenceKey &right);
	friend bool operator<=(const CowPresenceKey &left ,const CowPresenceKey &right);
	
	
};

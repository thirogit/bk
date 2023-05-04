#pragma once

#include <data/impl/CowDetails.h>
#include <data/impl/CowEntryDetails.h>

class DocCowItem
{
public:
	DocCowItem(uint32_t cowId);
	DocCowItem(const DocCowItem& src);
	uint32_t GetCowId() const;
			
	const CowDetails& GetCow() const;
	void SetCow(const CowDetails& cow);
	
	const CowEntryDetails& GetEntry() const;
	void SetEntry(const CowEntryDetails& entry);

	void CopyFrom(const DocCowItem& src);
	
	std::wstring ToString() const;

private:
	uint32_t m_cowId;
	CowDetails m_cowDetails;
	CowEntryDetails m_cowEntryDetails;
};

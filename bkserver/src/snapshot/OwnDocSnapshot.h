#pragma once

#include "DocSnapshot.h"

class OwnDocSnapshot : public DocSnapshot
{
public:
	OwnDocSnapshot(uint32_t docId);
	
	void SetHerdId(uint32_t herdId);
	uint32_t GetHerdId() const;
	
	virtual std::wstring ToString() const;

private:
	uint32_t m_herdId;
};
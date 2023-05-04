#pragma once

#include "DocSnapshot.h"

class HentsDocSnapshot : public DocSnapshot
{
public:
	HentsDocSnapshot(uint32_t docId);
	
	void SetHerdId(uint32_t herdId);
	uint32_t GetHerdId() const;

	void SetHentId(uint32_t hentId);
	uint32_t GetHentId() const;

	virtual std::wstring ToString() const;
	
private:
	uint32_t m_herdId;
	uint32_t m_hentId;
};
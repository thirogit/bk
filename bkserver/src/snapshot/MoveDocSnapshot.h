#pragma once

#include "DocSnapshot.h"

class MoveDocSnapshot : public DocSnapshot
{
public:
	MoveDocSnapshot(uint32_t docId);
	
	void SetSrcHerdId(uint32_t herdId);
	uint32_t GetSrcHerdId() const;
	
	void SetDstHerdId(uint32_t herdId);
	uint32_t GetDstHerdId() const;

private:
	uint32_t m_srcHerdId;
	uint32_t m_dstHerdId;
};
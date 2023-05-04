#include "stdafx.h"
#include "MoveDocSnapshot.h"


MoveDocSnapshot::MoveDocSnapshot(uint32_t docId) : DocSnapshot(docId),m_srcHerdId(NULL_ID),m_dstHerdId(NULL_ID)
{
}
	
void MoveDocSnapshot::SetSrcHerdId(uint32_t herdId)
{
	m_srcHerdId = herdId;
}

uint32_t MoveDocSnapshot::GetSrcHerdId() const
{
	return m_srcHerdId;
}
	
void MoveDocSnapshot::SetDstHerdId(uint32_t herdId)
{
	m_dstHerdId = herdId;
}

uint32_t MoveDocSnapshot::GetDstHerdId() const
{
	return m_dstHerdId;
}


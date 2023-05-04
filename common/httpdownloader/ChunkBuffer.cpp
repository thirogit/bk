#include "stdafx.h"
#include "ChunkBuffer.h"


CChunkBuffer::CChunkBuffer(UINT size) : m_size(size)
{
	ASSERT(size > 0);
	m_pBuffer = new BYTE[m_size];
}
BYTE* CChunkBuffer::GetBuffer()
{
	return m_pBuffer;
}
UINT CChunkBuffer::GetSize()
{
	return m_size;
}

CChunkBuffer::~CChunkBuffer()
{
	delete m_pBuffer;
}
	

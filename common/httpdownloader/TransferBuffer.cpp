#include "stdafx.h"
#include "TransferBuffer.h"

CTransferBuffer::CTransferBuffer(BYTE* pBuffer,UINT uSize) : m_pBuffer(pBuffer),m_uSize(uSize),m_bufferPos(0)
{
	ASSERT(m_pBuffer);
}
void CTransferBuffer::OnReceived(CChunkBuffer &chunkBuffer,UINT bytesReceived)
{
	UINT bytesToWrite = bytesReceived;
	if(m_bufferPos+bytesToWrite >= m_uSize)
	{
		bytesToWrite = m_uSize-m_bufferPos;
	}
	memcpy(m_pBuffer+m_bufferPos,chunkBuffer.GetBuffer(), bytesToWrite);
	m_bufferPos+=bytesToWrite;
}

UINT CTransferBuffer::OnSend(CChunkBuffer &chunkBuffer)
{
	UINT bytesToSend = 0;
	if(m_bufferPos < m_uSize)
	{
		UINT bytesLeftToSend = (m_uSize-m_bufferPos);
		UINT bufferSize = chunkBuffer.GetSize();
		bytesToSend = bytesLeftToSend > bufferSize ? bufferSize : bytesLeftToSend;
		memcpy(chunkBuffer.GetBuffer(),m_pBuffer + m_bufferPos, bytesToSend);
		m_bufferPos += bytesToSend;
	}
	return bytesToSend;
}

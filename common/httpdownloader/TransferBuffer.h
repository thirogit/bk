#ifndef __TRANSFERBUFFER_H__
#define __TRANSFERBUFFER_H__

#include "ChunkBuffer.h"
#include "TransferStream.h"

class CTransferBuffer : public CTransferStream
{
public:
	CTransferBuffer(BYTE* pBuffer,UINT uSize);
	virtual void OnReceived(CChunkBuffer &chunkBuffer,UINT bytesReceived);
	virtual UINT OnSend(CChunkBuffer &chunkBuffer);
private:
	BYTE* m_pBuffer;
	UINT m_uSize;
	UINT m_bufferPos;
};


#endif //__TRANSFERBUFFER_H__
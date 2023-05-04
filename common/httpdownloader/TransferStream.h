#ifndef __TRANSFERSTREAM_H__
#define __TRANSFERSTREAM_H__

#include "ChunkBuffer.h"

class CTransferStream
{
public:
	virtual void OnReceived(CChunkBuffer &chunkBuffer,UINT bytesReceived) = 0;
	virtual UINT OnSend(CChunkBuffer &chunkBuffer) = 0;
};


#endif //__TRANSFERSTREAM_H__
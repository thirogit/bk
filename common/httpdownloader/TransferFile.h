#ifndef __TRANSFERFILE_H__
#define __TRANSFERFILE_H__

#include "ChunkBuffer.h"
#include "TransferStream.h"

class CTransferFile : public CTransferStream
{
public:
	~CTransferFile();
	bool Open(const CString& filePath);
	void Close();
	virtual void OnReceived(CChunkBuffer &chunkBuffer,UINT bytesReceived);
	virtual UINT OnSend(CChunkBuffer &chunkBuffer);
private:
	CFile m_file;
};


#endif //__TRANSFERSTREAM_H__
#include "stdafx.h"
#include "TransferFile.h"


CTransferFile::~CTransferFile()
{
	Close();
}

	
void CTransferFile::Close()
{
	if(m_file.m_hFile != INVALID_HANDLE_VALUE)
		m_file.Close();
}

bool CTransferFile::Open(const CString& filePath)
{
	return m_file.Open(filePath,CFile::modeCreate | CFile::modeWrite) == TRUE;

}
void CTransferFile::OnReceived(CChunkBuffer &chunkBuffer,UINT bytesReceived)
{
	if(m_file.m_hFile)
		m_file.Write(chunkBuffer.GetBuffer(),bytesReceived);
}

UINT CTransferFile::OnSend(CChunkBuffer &chunkBuffer)
{
	if(m_file.m_hFile)
		return m_file.Read(chunkBuffer.GetBuffer(),chunkBuffer.GetSize());

	return 0;
}

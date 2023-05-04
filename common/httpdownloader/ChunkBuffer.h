#ifndef __CHUNKBUFFER_H__
#define __CHUNKBUFFER_H__

class CChunkBuffer
{
public:
	CChunkBuffer(UINT size);
	~CChunkBuffer();
	BYTE* GetBuffer();
	UINT GetSize();
private:
	BYTE* m_pBuffer;
	UINT m_size;
};


#endif
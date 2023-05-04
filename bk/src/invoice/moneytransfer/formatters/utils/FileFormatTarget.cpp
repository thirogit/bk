#include "stdafx.h"
#include "FileFormatTarget.h"

FileFormatTarget::FileFormatTarget( CFile* pFile) : m_pTargetFile(pFile)
{
   
}


FileFormatTarget::~FileFormatTarget()
{
   
}

void FileFormatTarget::flush()
{
	m_pTargetFile->Flush();
}

void FileFormatTarget::writeChars(const XMLByte* const toWrite
                                     , const XMLSize_t count
                                     , xercesc::XMLFormatter * const)
{
	m_pTargetFile->Write(toWrite,sizeof(XMLByte)*count);
}


#include "stdafx.h"
#include "VersionTmpArchiveFileCache.h"
#include <httpdownloader/HttpFileGetter.h>
#include <httpdownloader/TransferFile.h>
#include "log.h"
#include <filesystem\paths.h>

bool CVersionTmpArchiveFileCache::CCachedZipArchive::OpenArchive(LPCSTR szArchiveFilePath)
{
	if(!m_archiveZip.IsClosed())
	{
		LOG("ARCHIVE IS ALREADY OPENED.");
		return false;
	}
	m_archiveFilePath = szArchiveFilePath;
	return m_archiveZip.Open(szArchiveFilePath);
}
bool CVersionTmpArchiveFileCache::CCachedZipArchive::ExtractFile(LPCSTR szFilePathInArchive,LPCSTR szAbsoluteDestPath)
{
	if(m_archiveZip.IsClosed())
	{
		LOG("ARCHIVE IS CLOSED.");
		return false;
	}
	ZIP_INDEX_TYPE fileIndex = m_archiveZip.FindFile(szFilePathInArchive);
	if(fileIndex == ZIP_FILE_INDEX_NOT_FOUND)
	{
		LOG("FILE %s CAN NOT BE FOUND IN ARCHIVE %s.",szFilePathInArchive,(LPCSTR)m_archiveFilePath);
		return false;
	}
	
	return m_archiveZip.ExtractFile(fileIndex,szAbsoluteDestPath);
}

void CVersionTmpArchiveFileCache::CCachedZipArchive::CloseArchive()
{
	if(!m_archiveZip.IsClosed())
	{
		m_archiveZip.Close();
	}
}
	

CVersionTmpArchiveFileCache::CVersionTmpArchiveFileCache(const CString &sUpdateURL)
{
	m_tempDirPath = paths_GetTempPath(NULL);
	m_sUpdateURL = sUpdateURL;
}

bool CVersionTmpArchiveFileCache::ExtractFile(CCachedZipArchive* pCachedArchive,const CString& fileToExtractRelPath,
											  const CString& absoluteDestPath)
{
	return pCachedArchive->ExtractFile(fileToExtractRelPath,absoluteDestPath);
}

bool CVersionTmpArchiveFileCache::ExtractFile(CVersionInfo *pVI,
											  const CString& fileToExtractRelPath,
											  const CString& absoluteDestPath)
{
	CCachedZipArchive* pCachedArchive = NULL;
	if(m_tmpArchiveLocationMap.Lookup(pVI->GetVerNo(),pCachedArchive))
	{
		return ExtractFile(pCachedArchive,fileToExtractRelPath,absoluteDestPath);		
	}
	else
	{
		CString sArchiveLocation = m_sUpdateURL + "/zips/" + pVI->GetArchiveName();

		CHttpFileGetter verGetter(sArchiveLocation,NULL);	
		CTransferFile transferFile;
		CString tempArchiveFilePath = paths_ConcatPath(m_tempDirPath,pVI->GetArchiveName());

		if(!transferFile.Open(tempArchiveFilePath))
		{
			LOG("FAILD TO CREATE TEMP ARCHIVE FILE IN %s.",(LPCSTR)tempArchiveFilePath);
			return false;
		}

		if(verGetter.Download(transferFile))
		{
			transferFile.Close();
			pCachedArchive = new CCachedZipArchive();
			if(!pCachedArchive->OpenArchive(tempArchiveFilePath))
			{
				LOG("FAILD TO OPEN TMP ARCHIVE IN %s.",(LPCSTR)tempArchiveFilePath);
				delete pCachedArchive;
				DeleteFile(tempArchiveFilePath);
				return false;
			}
			m_tmpArchiveLocationMap.SetAt(pVI->GetVerNo(),pCachedArchive);
			m_tmpFileDeleter.AddFile(tempArchiveFilePath);		
			return ExtractFile(pCachedArchive,fileToExtractRelPath,absoluteDestPath);
		}
		else
		{
			LOG("FAILD TO DOWNLOAD ARCHIVE FOR VERSION %d.",pVI->GetVerNo());
		}
	}
	return false;
}

CVersionTmpArchiveFileCache::~CVersionTmpArchiveFileCache()
{
	POSITION pos = m_tmpArchiveLocationMap.GetStartPosition();
	CCachedZipArchive* pCachedArchive;
	UINT versionNo;
	while(pos)
	{
		pCachedArchive = NULL;
		versionNo = 0;
		m_tmpArchiveLocationMap.GetNextAssoc(pos,versionNo,pCachedArchive);
		pCachedArchive->CloseArchive();
		delete pCachedArchive;
	}
}
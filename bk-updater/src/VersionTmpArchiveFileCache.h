#ifndef __VERSIONTMPARCHIVEFILECACHE_H__
#define __VERSIONTMPARCHIVEFILECACHE_H__

#include <map>
#include <versioning/VersionInfo.h>
#include <filesystem\FileDeleterFinalizer.h>
#include <zip/ziparchive/ZipArchive.h>
#include <afxtempl.h>

class CVersionTmpArchiveFileCache
{
private:
	class CCachedZipArchive
	{
	public:
		bool OpenArchive(LPCSTR szArchiveFilePath);
		void CloseArchive();
		bool ExtractFile(LPCSTR szFilePathInArchive,LPCSTR szAbsoluteDestPath);
	private:
		CString			m_archiveFilePath;
		CZipArchive		m_archiveZip;
	};
public:
	CVersionTmpArchiveFileCache(const CString &sUpdateURL);
	~CVersionTmpArchiveFileCache();
	bool ExtractFile(CVersionInfo *pVI,const CString& fileToExtractRelPath,const CString& absoluteDestPath);
	bool ExtractFile(CCachedZipArchive* pCachedArchive,const CString& fileToExtractRelPath,const CString& absoluteDestPath);
private:
	CString m_tempDirPath;
	CFileDeleterFinalizer m_tmpFileDeleter;
	CMap<UINT,UINT,CCachedZipArchive*,CCachedZipArchive*> m_tmpArchiveLocationMap;
	CString m_sUpdateURL;
};

#endif
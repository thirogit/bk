#ifndef __VERSIONFILE_H__
#define __VERSIONFILE_H__

#include <versioning\VersionInfo.h>
#include "VersionTmpArchiveFileCache.h"

class CVersionFile 
{
public:
	CVersionFile(const CString& basePath,const CString& relativeFilePath,CVersionInfo *pOwningVersion);
	bool Equals(const CString& relativeFilePath);
	bool UpdateFile(CVersionTmpArchiveFileCache *pArchiveCache,bool bCheck);
	bool BackupOldFile(const CString& oldFileBackupDirPath);
	CString GetFilePath();
	bool RolbackUpdate();
protected:
	bool GetCanonicalFileFullPath(CString &canonicalFilePath);
private:
	CString			m_relativeFilePath;
	CVersionInfo*	m_owningVersion;
	CString			m_tmpZipPath;
	CString			m_basePath;
	CString			m_oldFileBackupPath;
	

	
};


#endif
#ifndef __VERSIONINSTALLER_H__
#define __VERSIONINSTALLER_H__

#include <progress\SimpleProgress.h>
#include "VersionTmpArchiveFileCache.h"
#include <versioning\VersionInfo.h>
#include "VersionInfoList.h"
#include "VersionFileList.h"

class VersionInstaller 
{
public:
	VersionInstaller(CVersionTmpArchiveFileCache* pCache);	
	~VersionInstaller();
	void AddVersion(const CVersionInfo& versionToInstall);
	void Install(SimpleProgress* pProgress);
private:
	bool CheckCanUpdate(CVersionFileList& verFileList);
	bool BackupFiles(CVersionFileList& verFileList);
private:
	CVersionInfoList m_VersionsToInstall;
	CVersionTmpArchiveFileCache* m_pArchiveCache;
	
};

#endif
#include "stdafx.h"
#include "VersionInstaller.h"
#include <filesystem\paths.h>
#include "log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


VersionInstaller::VersionInstaller(CVersionTmpArchiveFileCache* pCache) : m_pArchiveCache(pCache)
{
}
VersionInstaller::~VersionInstaller()
{
	m_VersionsToInstall.FreeAndFlush();
}

void VersionInstaller::AddVersion(const CVersionInfo& versionToInstall)
{
	m_VersionsToInstall.Add(new CVersionInfo(versionToInstall));
}

void VersionInstaller::Install(SimpleProgress* pProgress)
{
	m_VersionsToInstall.Sort(false);

	CVersionFileList versionFiles;
	CVersionFileList::AutoArray autoDeleteVersionFiles(versionFiles);

	CString basePath = paths_GetApplicationDir();

	CStringArray files;
	CVersionInfoList::ArrayIterator vIt = m_VersionsToInstall.iterator();
	while(vIt.hasNext())
	{
		CVersionInfo *pVI = *(vIt++);		
		files.Copy(pVI->GetVerFiles());
		for(int iFile = 0;iFile < files.GetCount();iFile++)
		{
			CString file = files[iFile];
			if(!versionFiles.HasThisFile(file))
				versionFiles.Add(new CVersionFile(basePath,file,pVI));
		}
	}

	if(!CheckCanUpdate(versionFiles))
	{
		AfxMessageBox(IDS_ERR_FILES_ARE_IN_USE);
		LOG("UPDATE CHECK FAILD.");
		return;
	}

	if(!BackupFiles(versionFiles))
	{
		AfxMessageBox(IDS_ERR_FAILD_TO_BACKUP_FILES);
		LOG("FAILD TO BACKUP FILES TO UPDATE.");
		return;
	}

	pProgress->SetSteps(versionFiles.GetCount());
	
	CVersionFileList::ArrayIterator vfIt = versionFiles.iterator();


	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);
		LOG("UPDAING %s",(LPCSTR)pVersionFile->GetFilePath());
		bool bUpdated = pVersionFile->UpdateFile(m_pArchiveCache,false);
		pProgress->Step();
		if(!bUpdated)
		{
			while(!vfIt.isFirst())
				(*(vfIt--))->RolbackUpdate();
			break;
		}
	}

	return;


}

bool VersionInstaller::BackupFiles(CVersionFileList& verFileList)
{
	CString backupBasePath = paths_ConcatPath(paths_GetApplicationDir(),CString("update_backup"));
	CString thisBackupPath = paths_ConcatPath(backupBasePath,
											  CString(CDateTime::now().GetDateInFmt(CDateTime::YYYYMMDDHHMISS)));

	CVersionFileList::ArrayIterator vfIt = verFileList.iterator();
	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);
		LOG("BACKING UP %s.",(LPCSTR)pVersionFile->GetFilePath());
		bool bBackedup = pVersionFile->BackupOldFile(thisBackupPath);
		if(!bBackedup)
		{
			LOG("BACKUP FAILD FOR FILE: %s",(LPCSTR)pVersionFile->GetFilePath());
			return false;
		}
	}

	return true;
}

bool VersionInstaller::CheckCanUpdate(CVersionFileList& verFileList)
{
	CVersionFileList::ArrayIterator vfIt = verFileList.iterator();
	while(vfIt.hasNext())
	{
		CVersionFile* pVersionFile = *(vfIt++);

		LOG("CHECKING IF GOOD FOR UPDATE: %s.",(LPCSTR)pVersionFile->GetFilePath());
		bool bChecked = pVersionFile->UpdateFile(NULL,true);
		if(!bChecked)
		{
			LOG("UPDATE CHECK FAILD FOR FILE: %s",(LPCSTR)pVersionFile->GetFilePath());
			return false;
		}
	}
	return true;
}
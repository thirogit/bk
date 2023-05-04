#include "stdafx.h"
#include "VersionFile.h"
#include <filesystem\paths.h>
#include <filesystem\files.h>

CVersionFile::CVersionFile(const CString& basePath,const CString& relativeFilePath,CVersionInfo *pOwningVersion) :
				m_relativeFilePath(relativeFilePath),m_owningVersion(pOwningVersion),m_basePath(basePath)
{
	ASSERT(m_owningVersion);

}
bool CVersionFile::Equals(const CString& relativeFilePath)
{
	return m_relativeFilePath.CompareNoCase(relativeFilePath) == 0;
}
CString CVersionFile::GetFilePath()
{
	return paths_ConcatPath(m_basePath,m_relativeFilePath);
}

bool CVersionFile::UpdateFile(CVersionTmpArchiveFileCache *pArchiveCache,bool bCheck)
{
	if(bCheck)
	{
		CString filePath = paths_ConcatPath(m_basePath,m_relativeFilePath);
		CFile file;	
		if(file.Open(filePath,CFile::shareExclusive|CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite))
		{	
			file.Close();
			return true;
		}		
		return false;
	}
	else
	{
		return pArchiveCache->ExtractFile(m_owningVersion,m_relativeFilePath,m_basePath);
	}
	return false;
}
bool CVersionFile::GetCanonicalFileFullPath(CString &canonicalFilePath)
{
	CString filePath = paths_ConcatPath(m_basePath,m_relativeFilePath);
	CFile file;	
	if(file.Open(filePath,CFile::modeReadWrite))
	{	
		file.Close();
		canonicalFilePath = file.GetFilePath();
		return true;
	}
	return false;	
}

bool CVersionFile::BackupOldFile(const CString& oldFileBackupDirPath)
{
	CString oldFilePath = paths_ConcatPath(oldFileBackupDirPath,m_relativeFilePath);
	CString backupLocation = paths_GetParentPath(oldFilePath);
	CString fileToBackupPath = paths_ConcatPath(m_basePath,m_relativeFilePath);
	paths_EnsurePathExist(backupLocation);

	if(IsFileExists(fileToBackupPath))
	{
		if(CopyFile(fileToBackupPath,oldFilePath,FALSE))
		{
			m_oldFileBackupPath = oldFilePath;
			return true;
		}
	}
	else
		return true; //new file

	return false;
}
bool CVersionFile::RolbackUpdate()
{
	CString destFilePath = paths_ConcatPath(m_basePath,m_relativeFilePath);
	CString srcFilePath = m_oldFileBackupPath;

	return true;
}
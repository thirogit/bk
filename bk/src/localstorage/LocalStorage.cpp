#include "stdafx.h"
#include "LocalStorage.h"
#include <filesystem\paths.h>

std::wstring LocalStorage::GetStorageFile(const std::wstring& file) const
{			 
    std::wstring storageDir = GetStorageDirectory();
	return paths_ConcatPath(storageDir, file);
}

std::wstring LocalStorage::GetStorageDirectory() const
{
	std::wstring homePath = paths_GetHomePath();	
	std::wstring appHome = paths_ConcatPath(homePath,L".bk");	
	return appHome;
}
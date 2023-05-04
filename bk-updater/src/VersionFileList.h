#ifndef __VERSIONFILELIST_H__
#define __VERSIONFILELIST_H__

#include <arrays\ptrflusharray.h>
#include "VersionFile.h"

class CVersionFileList : public PtrFlushArray<CVersionFile>
{
public:
	bool HasThisFile(const CString& relativeFilePath); 
	bool DownloadArchives();

private:

//	CFileDeleter m_tmpZipsDeleter;
};




#endif
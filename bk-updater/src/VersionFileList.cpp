#include "stdafx.h"
#include "VersionFileList.h"

bool CVersionFileList::HasThisFile(const CString& relativeFilePath)
{
	ArrayIterator it = iterator();
	while(it.hasNext())
	{
		CVersionFile *verFile = *(it++);
		if(verFile->Equals(relativeFilePath))
			return true;
	}
	return false;
}



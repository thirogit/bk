#include "stdafx.h"
#include "DownloadException.h"

IMPLEMENT_DYNAMIC(DownloadException, CBaseException)


DownloadException::DownloadException(const CString &errorMsg) 
{
	SetError(errorMsg);
}

DownloadException::DownloadException(const DownloadException &src)
{
	SetError(src.GetError());
}
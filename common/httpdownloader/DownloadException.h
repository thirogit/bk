#ifndef __DOWNLOADEXCEPTION_H__
#define __DOWNLOADEXCEPTION_H__

#include "../exception/BaseException.h" 

class DownloadException : public CBaseException
{
	DECLARE_DYNAMIC(DownloadException)
public:
	DownloadException(const CString &errorMsg);
	DownloadException(const DownloadException &src);
};

#endif
#ifndef __DOWNLOADPROGRESS_H__
#define  __DOWNLOADPROGRESS_H__

#include "HttpFileGetter.h"
#include "../progress/SimpleProgress.h"

class DownloadProgress : public CHttpFileGetterNotification
{
public:
	DownloadProgress(SimpleProgress* pProgress);
	
	virtual void OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, 
							LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	virtual void OnDownloadProgress(UINT bytesTotal,UINT bytesReceived);
private:
	SimpleProgress* m_pProgress;
	int m_ProgressPercent;
};

#endif
#include "stdafx.h"
#include "DownloadProgress.h"


DownloadProgress::DownloadProgress(SimpleProgress* pProgress) : m_pProgress(pProgress)
{
	if(m_pProgress)
		m_pProgress->SetSteps(100);
	m_ProgressPercent = 0;		
}

void DownloadProgress::OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
}
void DownloadProgress::OnDownloadProgress(UINT bytesTotal,UINT bytesReceived)
{
	if(m_pProgress)
	{
		int currentProgressPercent = (bytesReceived*100)/bytesTotal;
		if(currentProgressPercent > m_ProgressPercent)
		while(currentProgressPercent < m_ProgressPercent)
		{
			m_pProgress->Step();
			currentProgressPercent++;			
		}		
		m_ProgressPercent = currentProgressPercent;
	}
}
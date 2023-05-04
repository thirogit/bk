#include "stdafx.h"
#include "HttpFileGetter.h"
#include "DownloadException.h"

#define READBUFFER_SIZE 1024
/////////////////////////////////////////////////////////////////////////////////////////
CHttpFileGetter::HandleFinalizer::HandleFinalizer()
{
	Set(NULL);
}
CHttpFileGetter::HandleFinalizer::HandleFinalizer(HINTERNET hHandle)
{
	Set(hHandle);
}
void CHttpFileGetter::HandleFinalizer::Set(HINTERNET hHandle)
{
	m_hHandle = hHandle;
}
CHttpFileGetter::HandleFinalizer::~HandleFinalizer()
{
	if(m_hHandle)
		::InternetCloseHandle(m_hHandle);

}
/////////////////////////////////////////////////////////////////////////////////////////
CHttpFileGetter::CHttpFileGetter(const CString& URL, CHttpFileGetterNotification* pNotification)
{
	m_pNotification = pNotification;
	m_URL = URL;
}
void CHttpFileGetter::SetUsername(const CString& username)
{
	m_userName = username;
}
void CHttpFileGetter::SetPassword(const CString& password)
{
	m_password = password;
}
const CString& CHttpFileGetter::GetPassword()
{
	return m_password;
}
const CString& CHttpFileGetter::GetUsername()
{
	return m_userName;
}
const CString& CHttpFileGetter::GetURL()
{
	return m_URL;
}

void CHttpFileGetter::OnError(LPCSTR szErrorMessage)
{
	throw new DownloadException(szErrorMessage);
}
void CHttpFileGetter::OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	if(m_pNotification)
		m_pNotification->OnStatus(hInternet,dwInternetStatus,lpvStatusInformation,dwStatusInformationLength);
}
void CHttpFileGetter::OnDownloadProgress(UINT bytesTotal,UINT bytesReceived)
{
	if(m_pNotification)
		m_pNotification->OnDownloadProgress(bytesTotal,bytesReceived);
}
 

HINTERNET  CHttpFileGetter::OpenSession()
{
	HINTERNET     hInternetSession = ::InternetOpen(AfxGetAppName(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternetSession == NULL)
	{
		OnError("Failed in call to InternetOpen");
		return NULL;
	}

	if (::InternetSetStatusCallback(hInternetSession, _OnStatusCallBack) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		OnError("Failed in call to InternetSetStatusCallback");
		return NULL;
	}
	return hInternetSession;
}

HINTERNET  CHttpFileGetter::Connect(HINTERNET hSession)
{
	HINTERNET     hHttpConnection = NULL;
	CString       sServer; 
	CString       sObject; 
	CString       sFilename;
	INTERNET_PORT nPort;
	DWORD         dwServiceType;

	if (!AfxParseURL(GetURL(), dwServiceType, sServer, sObject, nPort))
	{
		OnError("Faild to parse version info URL on connect");
		return NULL;
	}

	hHttpConnection = ::InternetConnect(hSession, sServer, nPort, "","", dwServiceType, 0, (DWORD_PTR) this);
		
	if (hHttpConnection == NULL)
	{
		OnError("Failed in call to InternetConnect");
		return NULL;
	}

	return hHttpConnection;
}

HINTERNET  CHttpFileGetter::RequestURL(HINTERNET hConnection)
{	
	HINTERNET     hHttpFile = NULL;
	//Issue the request to read the file
	LPCTSTR ppszAcceptTypes[2];
	ppszAcceptTypes[0] = _T("*/*");  //We support accepting any mime file type since this is a simple download of a file
	ppszAcceptTypes[1] = NULL;
	
	CString       sServer; 
	CString       sObject; 
	CString       sFilename;
	INTERNET_PORT nPort;
	DWORD         dwServiceType;

	if (!AfxParseURL(GetURL(), dwServiceType, sServer, sObject, nPort))
	{
		OnError("Faild to parse version info URL on request");
		return NULL;
	}

	hHttpFile = HttpOpenRequest(hConnection, NULL, sObject, NULL, NULL, ppszAcceptTypes, INTERNET_FLAG_RELOAD | 
						INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION, (DWORD_PTR) this);
	if (hHttpFile == NULL)
	{
		OnError("Failed in call to HttpOpenRequest");
		return NULL;
	}

		
		
	//Issue the request
	BOOL bSend = ::HttpSendRequest(hHttpFile, NULL, 0, NULL, 0);
	if (!bSend)
	{
		OnError("Failed in call to HttpSendRequest");
		return NULL;
	}

	//Check the HTTP status code
	TCHAR szStatusCode[32];
	DWORD dwInfoSize = sizeof(szStatusCode);
	if (!HttpQueryInfo(hHttpFile, HTTP_QUERY_STATUS_CODE, szStatusCode, &dwInfoSize, NULL))
	{
		OnError("Failed in call to HttpQueryInfo for HTTP query status code");
		return NULL;
	}
	else
	{
		long nStatusCode = _ttol(szStatusCode);

		//Handle any authentication errors
		if (nStatusCode == HTTP_STATUS_PROXY_AUTH_REQ || nStatusCode == HTTP_STATUS_DENIED)
		{
			// We have to read all outstanding data on the Internet handle
			// before we can resubmit request. Just discard the data.
			char szData[51];
			DWORD dwSize;
			do
			{
				::InternetReadFile(hHttpFile, (LPVOID)szData, 50, &dwSize);
			}
			while (dwSize != 0);

			OnError("Authentication error");
		}
		else if (nStatusCode != HTTP_STATUS_OK)
		{
			OnError("Failed to retrieve a HTTP 200 status");
			return NULL;
		}
	}
	return hHttpFile;
}

long CHttpFileGetter::GetContentLength(HINTERNET hHttpFile)
{
	// Get the length of the file.            
	TCHAR szContentLength[32];
	DWORD dwInfoSize = sizeof(szContentLength);
	DWORD dwFileSize = 0;
	if (::HttpQueryInfo(hHttpFile, HTTP_QUERY_CONTENT_LENGTH, szContentLength, &dwInfoSize, NULL))
	{
		return _ttol(szContentLength);		
	}
	return -1;
}

long CHttpFileGetter::GetContentLength()
{
	HINTERNET     hInternetSession = OpenSession();
	HandleFinalizer autoCloseSessionHandle(hInternetSession);
	if (hInternetSession == NULL) return -1;

	HINTERNET     hHttpConnection = Connect(hInternetSession);
	HandleFinalizer autoCloseConnectionHandle(hHttpConnection);
	if (hHttpConnection == NULL) return -1;


	HINTERNET     hHttpFile = RequestURL(hHttpConnection);;
	HandleFinalizer autoCloseFileHandle(hHttpFile);
	if(hHttpFile == NULL) return -1;

	return GetContentLength(hHttpFile);	
}

bool CHttpFileGetter::Download(CTransferStream& downStream)
{
	HINTERNET     hInternetSession = NULL;
	HandleFinalizer autoCloseSessionHandle;
	HINTERNET     hHttpConnection = NULL;
	HandleFinalizer autoCloseConnectionHandle;
	HINTERNET     hHttpFile = NULL;
	HandleFinalizer autoCloseFileHandle;

	do
	{
		hInternetSession = OpenSession();
		if (hInternetSession == NULL) break;
		autoCloseSessionHandle.Set(hInternetSession);
	 
		hHttpConnection = Connect(hInternetSession);
		if (hHttpConnection == NULL) break;
		autoCloseConnectionHandle.Set(hHttpConnection);

		hHttpFile = RequestURL(hHttpConnection);
		if(hHttpFile == NULL) break;
		autoCloseFileHandle.Set(hHttpFile);
		
		long contentLength = GetContentLength(hHttpFile);	
		if(contentLength < 0)
		{
			OnError("Failed to get content length.");
		}
				
		DWORD dwBytesRead = 0;
		CChunkBuffer readBuffer(READBUFFER_SIZE);
		DWORD dwBytesToRead = READBUFFER_SIZE;
		DWORD dwTotalBytesRead = 0;
		
		do
		{
			if (!::InternetReadFile(hHttpFile, readBuffer.GetBuffer(), dwBytesToRead, &dwBytesRead))
			{
				OnError("Failed in call to InternetReadFile");			
				return false;
			}
			else if (dwBytesRead)
			{
				downStream.OnReceived(readBuffer,dwBytesRead);
				dwTotalBytesRead += dwBytesRead;
				OnDownloadProgress((UINT)contentLength,dwTotalBytesRead);
			}
		} 
		while (dwBytesRead);


	} while(FALSE);

	return true;
}


void CALLBACK CHttpFileGetter::_OnStatusCallBack(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, 
                                                  LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
  
  CHttpFileGetter* pGetter = (CHttpFileGetter*) dwContext;
  ASSERT(pGetter);
  pGetter->OnStatusCallBack(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}

void CHttpFileGetter::OnStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus, 
                                         LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
{
	OnStatus(hInternet, dwInternetStatus, lpvStatusInformation, dwStatusInformationLength);
}


#ifndef __HTTPFILEGET_H__
#define __HTTPFILEGET_H__

#include <afxinet.h>
#include "TransferStream.h"

class CHttpFileGetterNotification
{
public:
	virtual void OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength) = 0;
	virtual void OnDownloadProgress(UINT bytesTotal,UINT bytesReceived) = 0;
};

class CHttpFileGetter 
{
	class HandleFinalizer
	{	
	public:
		HandleFinalizer();
		HandleFinalizer(HINTERNET hHandle);
		void Set(HINTERNET hHandle);
		void Release();
		~HandleFinalizer();
	private:
		HINTERNET m_hHandle;
	};

public:
	CHttpFileGetter(const CString& URL, CHttpFileGetterNotification* pNotification);
	void SetUsername(const CString& username);
	void SetPassword(const CString& password);
	const CString& GetPassword();
	const CString& GetUsername();
	const CString& GetURL();
	bool Download(CTransferStream& downStream);
	long GetContentLength();

private:
	static void CALLBACK _OnStatusCallBack(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, 
                                        LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void OnStatusCallBack(HINTERNET hInternet, DWORD dwInternetStatus, 
                        LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);

	HINTERNET  OpenSession();
	HINTERNET  Connect(HINTERNET hSession);
	HINTERNET  RequestURL(HINTERNET hConnection);
	long		GetContentLength(HINTERNET hHttpFile);

	void OnError(LPCSTR szErrorMessage);
	void OnStatus(HINTERNET hInternet, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength);
	void OnDownloadProgress(UINT bytesTotal,UINT bytesReceived);
 
private:
	CString m_userName;
	CString m_password;
	CString m_URL;
	CHttpFileGetterNotification* m_pNotification;
};

#endif //__HTTPFILEGET_H__
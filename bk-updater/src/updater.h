#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		
#include <afxmt.h>
#include <versioning\VersionInfoStruct.h>
#include "VersionInfoList.h"
#include "VersionFileList.h"
#include <progress\SimpleProgress.h>

class CUpdaterApp : public CWinApp
{
public:
	CUpdaterApp();
	
	~CUpdaterApp();
public:
	virtual BOOL InitInstance();
private:
	UINT GetCattlebaseExeVersion();
	void CreateVersionList(VERSION_INFO_HEADER* pVIHeader,CVersionInfoList &versionInfoList);
	CString GetUpdateURL();


	CMutex * m_pSIMutex;

	DECLARE_MESSAGE_MAP()
};

extern CUpdaterApp theApp;
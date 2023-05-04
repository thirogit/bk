#include "stdafx.h"
#include "updater.h"
#include "log.h"
#include <filesystem\paths.h>
#include <wait\WaitDialog.h>

#include <versioning\generated\UpdateLocation.hxx>

#include <httpdownloader/HttpFileGetter.h>
#include <httpdownloader/TransferBuffer.h>

#include <string/TempStringLoader.h>
#include <memory>

#include <httpdownloader\DownloadProgress.h>

#include <progress\ui\SimpleProgressDialog.h>

#include "CmdLineParser.h"
#include "VersionFile.h"
#include "VersionInstaller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MUTEX_NAME "__CATTLEBASE_APP_SIMUTEX__"

BEGIN_MESSAGE_MAP(CUpdaterApp, CWinApp)
	
END_MESSAGE_MAP()

CUpdaterApp::CUpdaterApp() : m_pSIMutex(NULL)
{
	
}

CUpdaterApp::~CUpdaterApp() 
{
	if (m_pSIMutex != NULL)
	{	
		m_pSIMutex->Unlock();
		delete m_pSIMutex;
		m_pSIMutex = NULL;
	}
}

CUpdaterApp theApp;

UINT CUpdaterApp::GetCattlebaseExeVersion()
{
	CString cattlebaseExePath = paths_ConcatPath(paths_GetApplicationDir(),CString("cattlebase.exe"));

	DWORD   dwVerHnd = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSize (cattlebaseExePath, &dwVerHnd);
    if (!dwVerInfoSize)    
        return 0;

    LPSTR lpstrVffInfo =  new CHAR[dwVerInfoSize+1];

    if (!GetFileVersionInfo(cattlebaseExePath, dwVerHnd, dwVerInfoSize, lpstrVffInfo)) 
    {
        delete (lpstrVffInfo);
        return 0;  
	}

	UINT    uBuildNoLen = 0;
    LPVOID lpBuildNo = NULL;
    BOOL retVal = VerQueryValue ( lpstrVffInfo, "\\BuildInformation\\BuildNo", &lpBuildNo, (UINT *)&uBuildNoLen);
	LPCSTR szBuildNo = (LPCSTR)lpBuildNo;

	UINT versionExtend = atoi(szBuildNo);
	delete lpstrVffInfo;
	return versionExtend;
}



BOOL CUpdaterApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("Cattlebase Updater"));

	m_pSIMutex = new CMutex(FALSE,MUTEX_NAME);
	if(!m_pSIMutex->Lock(1))
	{
		LOG("ANOTHER CATTLEBASE OR UPDATER INSTANCE IS RUNNING.");
		AfxMessageBox(IDS_ERRMUTEXLOCK,MB_OK,0);
		return FALSE;
	}

	CString logPath = paths_ConcatPath(paths_GetApplicationDir(),_T("UPDATERLOG.log"));

	AMLOG_SETFILENAME((LPCSTR)logPath);
	AMLOG_SETLOGLEVEL_INFO;


	CSimpleProgressDialog progress(CTempStringLoader(IDS_DOWNLOADINGUPDATES));
	progress.Create(NULL);

	UINT cattlebaseVersion = 0;
	
	CCmdLineParser cmdLineParser(::GetCommandLine());
	if(cmdLineParser.HasVal("version"))
	{
		cattlebaseVersion =  atoi(cmdLineParser.GetVal("version"));
		LOG("CMD LINE PARAMETER VERSION %d",cattlebaseVersion);
	}
	else
	{	
		cattlebaseVersion = GetCattlebaseExeVersion();
		LOG("EXISTING CATTLEBASE.EXE VERSION %d",cattlebaseVersion);
	}

	if(cattlebaseVersion == 0)
	{
		LOG("CANT FIND CATTLEBASE.EXE OR INVALID VERSION PASSED IN CMD LINE PARAMETER");		
		return FALSE;
	}
	
	CString sUpdateURL = GetUpdateURL();	
	if(sUpdateURL.IsEmpty())
	{
		LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		return FALSE;
	}
	
	DownloadProgress downloadProgress(&progress);
	
	CString sVersionInfoFileURL = sUpdateURL + "/version.info";

	CHttpFileGetter verGetter(sVersionInfoFileURL,&downloadProgress);

	LOG("DOWNLOADING VERSION INFO FILE.");
	long lContentLength = verGetter.GetContentLength();
	if(lContentLength > 0)
	{
		UINT uContentLength = (UINT)lContentLength;
		BYTE *pBuffer = new BYTE[uContentLength];
		std::auto_ptr<BYTE> autoBuffer(pBuffer);
		CTransferBuffer transferBuffer(pBuffer,uContentLength);
		
		if(verGetter.Download(transferBuffer))
		{
			VERSION_INFO_HEADER* pVIHeader = (VERSION_INFO_HEADER*)pBuffer;
			if(!strcmp(VERSION_INFO_HEADER_ID,pVIHeader->id))
			{
				if(pVIHeader->size == uContentLength)
				{
					if(cattlebaseVersion < pVIHeader->max_version_no)
					{
						LOG("UPDATING. OLD VERSION %d,  NEW %d.",cattlebaseVersion,pVIHeader->max_version_no);
						CVersionInfoList versionInfoList;
						CreateVersionList(pVIHeader,versionInfoList);
						
						CVersionTmpArchiveFileCache cache(sUpdateURL);
						VersionInstaller installer(&cache);						
						CVersionInfoList::ArrayIterator viIt = versionInfoList.iterator();
						while(viIt.hasNext())
						{
							CVersionInfo *pVI = *(viIt++);
							if(pVI->GetVerNo() > cattlebaseVersion)
							{
								installer.AddVersion(*pVI);
							}
						}

						progress.SetTaskName(CTempStringLoader(IDS_INSTALINGVERUPDATES));

						installer.Install(&progress);						
											
						versionInfoList.FreeAndFlush();				
					}
					else
					{
						LOG("NO NEED FOR UPDATE, CURRENT CATTLEBASE.EXE VERSION %d, MAX UPDATE VERSION %d.",cattlebaseVersion,pVIHeader->max_version_no);
					}
				}	
				else
				{
					LOG("VERSION INFO FILE IS CORUPTED. (SIZE)");
				}
			}
			else
			{
				LOG("VERSION INFO FILE IS CORUPTED. (HEADER)");
			}
		}
		else
		{
			LOG("FAILD TO GET VERSION INFO CONFIGURATION!");
		}
	}
	else
	{
		LOG("FAILD TO GET VERSION INFO FILE SIZE.");
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


CString CUpdaterApp::GetUpdateURL()
{
	return "";
}

void CUpdaterApp::CreateVersionList(VERSION_INFO_HEADER* pVIHeader,CVersionInfoList &versionInfoList)
{

	VERSION_INFO *pVI = (VERSION_INFO*)(pVIHeader+1);
	for(UINT v = 0; v < pVIHeader->version_info_count;v++)
	{
		versionInfoList.Add(new CVersionInfo(pVI));
		pVI = (VERSION_INFO*)(((BYTE*)pVI) + pVI->size);
	}
}

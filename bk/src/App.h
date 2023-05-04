#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       
#include "context\XContext.h"
#ifdef _DEBUG
//#include <vld.h>
#endif

#include "services\ServiceRegistryImpl.h"


class CBKApp : public CWinAppEx
{
public:
	CBKApp();
	XContext* GetContext();
	ServiceRegistry* GetServiceRegistry();
	
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
protected:
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;
	ULONG_PTR m_gdiplusToken;
	XContext m_Context;

	ServiceRegistryImpl m_Registry;

	RCF::RcfInitDeinit m_RCFInit;
public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
};

extern CBKApp theApp;

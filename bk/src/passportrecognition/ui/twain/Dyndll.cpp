///////////////////////////////////////////////////////////////////////////////
//
//		CDynDll
//		----------
//		wrapper base class for a dynamically loaded DLL
//
////Holger Kloos, 1999-2001////////////////////////////////////////////////////


#include "stdafx.h"
#include "DynDLL.h"


//  CDynDLL
//	-------


typedef int (*FGetVersion)(void);

//  CDynDll
//	-------

CDynDLL::CDynDLL()
{
	m_hInst = 0;	
}

CDynDLL::~CDynDLL()
{
	Free();
}

BOOL CDynDLL::Create(LPCTSTR pName)
{
	m_hInst = LoadLibrary(pName);

    if (!m_hInst)
        TRACE("!!! ERROR loading DLL %s!\n", pName);

	return m_hInst != NULL;
}

BOOL CDynDLL::Free()
{
	BOOL bRes = m_hInst && FreeLibrary(m_hInst);
	m_hInst = NULL;

	return bRes;
}

BOOL CDynDLL::GetAddress(LPVOID* fct, const char* name) const
{
	ASSERT(m_hInst);

	*fct = GetProcAddress(m_hInst, name);

	if (!*fct)
		TRACE("! Function not found in DLL: %s",name);

	return *fct != NULL;
}




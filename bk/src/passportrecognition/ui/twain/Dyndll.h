///////////////////////////////////////////////////////////////////////////////
//
//		CDynDll
//		----------
//		wrapper base class for a dynamically loaded DLL
//
////Holger Kloos, 1999-2001////////////////////////////////////////////////////


#ifndef __DYNDLL_H
#define __DYNDLL_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynDLL  
{
public:
	CDynDLL();
	virtual ~CDynDLL();

	BOOL Free();

	HINSTANCE GetInstance() const	{ return m_hInst; };
    BOOL IsValid() const			{ return m_hInst > 0; };

protected:
	BOOL Create(LPCTSTR pName);
	BOOL GetAddress(LPVOID* fct, const char* name) const;

private:
	HINSTANCE m_hInst;
};

#endif

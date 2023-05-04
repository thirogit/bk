#ifndef __OWNCLASSWND_H__
#define __OWNCLASSWND_H__
	
#include <afxwin.h>
#include <string>

class COwnClassWnd : public CWnd
{
public:
	COwnClassWnd(const std::wstring& sClassName);
	virtual ~COwnClassWnd();	
	virtual BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL);

	
private:
	bool RegisterWindowClass();
	void UnregisterWindowClass();

private:
	std::wstring m_sClassName;
	
};


#endif
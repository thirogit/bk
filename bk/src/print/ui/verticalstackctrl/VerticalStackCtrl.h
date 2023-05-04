#ifndef __VERTIALSTACKCTRL_H__
#define __VERTIALSTACKCTRL_H__

#include <afxwin.h>
#include "../../../ctrls/ownclasswnd/OwnClassWnd.h"
#include "../../../ctrls/xcolorstatic/XColorStatic.h"
#include <arrays/Enumerator.h>
#include <vector>

class VertialStackCtrl : public CStatic
{

	struct Control
	{
		CWnd* window;
		CXColorStatic *header;
	};

public:
	VertialStackCtrl();	
	void AddControl(CWnd* pCtrl);
	void AddControl(const std::wstring& sHeaderText,CWnd* pCtrl);
	//PtrEnumerator<CWnd> EnumControls();
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
private:
	int ComputeHeight();
	int GetWindowH(CWnd* pWnd);

	void AddControl(CXColorStatic *header,CWnd* pCtrl);

private:
	std::vector<Control*> m_controls;
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};


#endif


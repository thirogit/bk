///////////////////////////////////////////////////////////////////////////////
//
//		CTwainEventHandler
//		------------------
//		Subclassing (main) window for Twain messages
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#ifndef _TWAINEVENTHANDLER_H__
#define _TWAINEVENTHANDLER_H__


interface IMsgLink
{
	virtual BOOL OnWinMsg(const MSG* pMsg) = 0;
};


class CTwainEventHandler
{
public:
	CTwainEventHandler();
	virtual ~CTwainEventHandler();

	BOOL Connect(IMsgLink*	pMsgLink);
	BOOL DisConnect();

	void SetWindowHandle(HWND hWnd)		{ m_hWnd = hWnd; }
	HWND GetWindowHandle() const		{ return m_hWnd; }
	HWND* GetWindowHandlePtr()			{ return &m_hWnd; }

private:
	LRESULT OnWndMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	IMsgLink*	m_pMsgLink;
 	WNDPROC 	m_OrgWndProc;
	HWND		m_hWnd;
};

#endif

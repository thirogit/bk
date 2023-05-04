///////////////////////////////////////////////////////////////////////////////
//
//		CTwainEventHandler
//		------------------
//		Subclassing (main) window for Twain messages
//
////Holger Kloos, 2003/////////////////////////////////////////////////////////


#include "stdafx.h"
#include "TwainEventHandler.h"


//  CTwainEventHandler
//	------------------


CTwainEventHandler::CTwainEventHandler()
{
	m_pMsgLink = NULL;
	m_OrgWndProc = NULL;
	m_hWnd = NULL;
}

CTwainEventHandler::~CTwainEventHandler()
{
}	

BOOL CTwainEventHandler::Connect(IMsgLink*	pMsgLink)
{
	ASSERT(m_OrgWndProc == NULL);
	ASSERT(m_hWnd);

	m_pMsgLink = pMsgLink;

	if (GetWindowLong(m_hWnd, GWL_USERDATA) == 0)
		SetWindowLong(m_hWnd, GWL_USERDATA, (DWORD)this);
	else
		return FALSE;

	m_OrgWndProc = (WNDPROC)SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD)WindowProc);

	return m_OrgWndProc != NULL;
}
	
BOOL CTwainEventHandler::DisConnect()
{
	if (!m_OrgWndProc)
		return FALSE;

	BOOL bRes = SetWindowLong(m_hWnd, GWL_WNDPROC, (DWORD)m_OrgWndProc) != NULL;
	SetWindowLong(m_hWnd, GWL_USERDATA, 0);
	m_OrgWndProc = NULL;

	return bRes;
}

LRESULT CTwainEventHandler::OnWndMsg(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) const
{
	if (m_pMsgLink)
	{
		MSG msg;

		msg.time = GetMessageTime();

		DWORD nPts = GetMessagePos();
		POINTS pts = MAKEPOINTS(nPts);
		msg.pt.x = pts.x;	msg.pt.y = pts.y;
		
		msg.hwnd = hwnd;
		msg.message = uMsg;
		msg.wParam = wParam;
		msg.lParam = lParam;
		if (m_pMsgLink->OnWinMsg(&msg))
			return 0;
	}
	return CallWindowProc(m_OrgWndProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CTwainEventHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CTwainEventHandler* pThis = reinterpret_cast <CTwainEventHandler*>((void*)GetWindowLong(hwnd, GWL_USERDATA));
	if (pThis)
		return pThis->OnWndMsg(hwnd, uMsg, wParam, lParam);

	ASSERT(0);

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

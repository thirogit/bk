#include "stdafx.h"
#include "UIBridge.h"

#define UIBRIDGE_WINDOW_CLASSNAME  _T( "UIBRIDGE" )
#define WM_UIBRIDGE_MSG (WM_APP+7001)

UIBridge::UIBridge() : m_hBridgeWnd(NULL)
{
	
}

bool UIBridge::RegisterWindowClass()
{
	WNDCLASS wndcls;

    HINSTANCE hInst = AfxGetResourceHandle();

    if ( !( ::GetClassInfo( hInst, UIBRIDGE_WINDOW_CLASSNAME , &wndcls ) ) )
    {		 
       wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
       wndcls.lpfnWndProc      = &UIBridge::BridgeWindowProc;
	   //wndcls.lpfnWndProc =     ::DefWindowProc;
       wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
       wndcls.hInstance        = hInst;
       wndcls.hIcon            = NULL;
       wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
       wndcls.hbrBackground    = (HBRUSH)( COLOR_3DFACE + 1 );
       wndcls.lpszMenuName     = NULL;
       wndcls.lpszClassName    = UIBRIDGE_WINDOW_CLASSNAME;
        
       if( !AfxRegisterClass( &wndcls ) )
       {
          AfxThrowResourceException();
          return FALSE;
       }
    }
    return TRUE;
}

void UIBridge::UnregisterWindowClass()
{
	WNDCLASS wndcls = { 0 };
   if( ( ::GetClassInfo( AfxGetResourceHandle() , UIBRIDGE_WINDOW_CLASSNAME , &wndcls ) ) ) 
   {
      ::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );
   }
}

LRESULT WINAPI UIBridge::BridgeWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_UIBRIDGE_MSG)
	{
		UIBridge* uiBridge = (UIBridge*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		uiBridge->OnUIBridgeMsg(wParam, lParam);
	}
	return ::DefWindowProc(hWnd,Msg,wParam,lParam);
}

UIBridge::~UIBridge()
{
	Destroy();
}

void UIBridge::Invoke(std::function<void ()>& fn)
{
	InvokeMessage* msg = new InvokeMessage();
	msg->fn = fn;
	Post(0,(LPARAM)msg);
}

void UIBridge::Post(WPARAM wParam, LPARAM lParam)
{
	ASSERT(::IsWindow(m_hBridgeWnd));
	::PostMessage(m_hBridgeWnd,WM_UIBRIDGE_MSG, wParam, lParam);
}

LRESULT UIBridge::OnUIBridgeMsg(WPARAM wParam,LPARAM lParam)
{
	InvokeMessage* msg = (InvokeMessage*)lParam;
	msg->fn();
	delete msg;
	return (LRESULT)0;	
}

void UIBridge::Create()
{	
	RegisterWindowClass();

	m_hBridgeWnd = CreateWindowEx(0, UIBRIDGE_WINDOW_CLASSNAME, L"",
		WS_CHILD,0,0,0,0,::GetDesktopWindow(), (HMENU)(UINT_PTR)-1, AfxGetInstanceHandle(),(LPVOID)NULL);
		
	::SetWindowLongPtr(m_hBridgeWnd, GWLP_USERDATA,(LONG)this);
}

void UIBridge::Destroy()
{
	if (::IsWindow(m_hBridgeWnd))
	{
		::DestroyWindow(m_hBridgeWnd);
		m_hBridgeWnd = NULL;
		UnregisterWindowClass();
	}
}

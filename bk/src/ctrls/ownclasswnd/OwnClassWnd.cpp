#include "stdafx.h"
#include "OwnClassWnd.h"


COwnClassWnd::COwnClassWnd(const std::wstring& sClassName) : m_sClassName(sClassName)
{
	RegisterWindowClass();
}

bool COwnClassWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;

    HINSTANCE hInst = AfxGetResourceHandle();

    if ( !( ::GetClassInfo( hInst, m_sClassName.c_str() , &wndcls ) ) )
    {		 
       wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
       wndcls.lpfnWndProc      = ::DefWindowProc;
       wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
       wndcls.hInstance        = hInst;
       wndcls.hIcon            = NULL;
       wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
       wndcls.hbrBackground    = (HBRUSH)( COLOR_3DFACE + 1 );
       wndcls.lpszMenuName     = NULL;
       wndcls.lpszClassName    = m_sClassName.c_str();
        
       if( !AfxRegisterClass( &wndcls ) )
       {
          AfxThrowResourceException();
          return FALSE;
       }
    }
    return TRUE;
}

void COwnClassWnd::UnregisterWindowClass()
{
	WNDCLASS wndcls = { 0 };
   if( ( ::GetClassInfo( AfxGetResourceHandle() , m_sClassName.c_str() , &wndcls ) ) ) 
   {
      ::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );
   }
}

BOOL COwnClassWnd::Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext/* = NULL*/)
{
	return CWnd::Create(m_sClassName.c_str(),lpszWindowName,dwStyle,rect,pParentWnd,nID,pContext);
}

COwnClassWnd::~COwnClassWnd()
{
	UnregisterWindowClass();
}

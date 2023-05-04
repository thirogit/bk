#include "stdafx.h"

#include "DayWeekMonthBtn.h"


#define BTNS_CTRL_ID 1001

#define BTN_DAY_ID 101
#define BTN_WEEK_ID 102
#define BTN_MONTH_ID 103

CDayWeekMonthBtn::CDayWeekMonthBtn() : m_unit(CalendarUnit_None)
{
	
}

CDayWeekMonthBtn::~CDayWeekMonthBtn() 
{
	
}


BEGIN_MESSAGE_MAP(CDayWeekMonthBtn, CStatic)
	ON_BN_CLICKED(BTN_DAY_ID,OnDay)
	ON_BN_CLICKED(BTN_WEEK_ID,OnWeek)
	ON_BN_CLICKED(BTN_MONTH_ID, OnMonth)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CDayWeekMonthBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1) return -1;
	return 0;
}


void CDayWeekMonthBtn::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(),  WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	
}

void CDayWeekMonthBtn::CheckButtons()
{
	TBBUTTONINFO btnInfo;
	memset(&btnInfo, 0, sizeof(TBBUTTONINFO));
	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_STATE;
	
	btnInfo.fsState  =  TBSTATE_ENABLED | (m_unit == CalendarUnit_Day ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_DAY_ID,&btnInfo);

	btnInfo.fsState  =  TBSTATE_ENABLED | (m_unit == CalendarUnit_Week ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_WEEK_ID,&btnInfo);

	btnInfo.fsState = TBSTATE_ENABLED | (m_unit == CalendarUnit_Month ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_MONTH_ID, &btnInfo);
	
}

BOOL CDayWeekMonthBtn::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (CStatic::Create(L"", dwStyle | SS_NOPREFIX, rect, pParentWnd, nID))
	{
		CreateButtons();
		return TRUE;
	}
	return FALSE;
}


void CDayWeekMonthBtn::SetUnit(CalendarUnit unit)
{
	m_unit = unit;
	if(IsWindow(GetSafeHwnd()))
		CheckButtons();
}

CalendarUnit CDayWeekMonthBtn::GetUnit() const
{
	return m_unit;
}

void CDayWeekMonthBtn::CreateButtons()
{

	RECT rect;
	GetClientRect(&rect);

		
	m_Btns.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS //| 	TBSTYLE_FLAT 
		| CCS_TOP | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,rect,this,BTNS_CTRL_ID);

	
	TBBUTTON dayBtn,weekBtn,monthBtn;
	memset(&dayBtn,0,sizeof(dayBtn));
	memset(&weekBtn,0,sizeof(weekBtn));
	memset(&monthBtn, 0, sizeof(monthBtn));

	
	dayBtn.iBitmap =  I_IMAGENONE;
	dayBtn.idCommand = BTN_DAY_ID;
	dayBtn.fsState = TBSTATE_ENABLED;
	dayBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	dayBtn.iString = (int)"";

	weekBtn.iBitmap =  I_IMAGENONE;
	weekBtn.idCommand = BTN_WEEK_ID;
	weekBtn.fsState = TBSTATE_ENABLED;
	weekBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	weekBtn.iString = (int)"";

	monthBtn.iBitmap = I_IMAGENONE;
	monthBtn.idCommand = BTN_MONTH_ID;
	monthBtn.fsState = TBSTATE_ENABLED;
	monthBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	monthBtn.iString = (int)"";
			
	m_Btns.InsertButton(0,&dayBtn);
	m_Btns.InsertButton(1,&weekBtn);
	m_Btns.InsertButton(2, &monthBtn);

	TBBUTTONINFO btnInfo;
	memset(&btnInfo, 0, sizeof(btnInfo));
	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_TEXT;

	btnInfo.pszText = (LPTSTR)L"D";
	m_Btns.SetButtonInfo(BTN_DAY_ID, &btnInfo);	

	btnInfo.pszText = (LPTSTR)L"T";
	m_Btns.SetButtonInfo(BTN_WEEK_ID, &btnInfo);

	btnInfo.pszText = (LPTSTR)L"M";
	m_Btns.SetButtonInfo(BTN_MONTH_ID, &btnInfo);

	//m_Btns.AutoSize();

	ResizeButtons();
	CheckButtons();
		
}


void CDayWeekMonthBtn::OnDay()
{
	if (m_unit != CalendarUnit_Day || m_unit == CalendarUnit_None)
		m_unit = CalendarUnit_Day;
	else
		m_unit = CalendarUnit_None;

	CheckButtons();
}

void CDayWeekMonthBtn::OnWeek()
{
	if (m_unit != CalendarUnit_Week || m_unit == CalendarUnit_None)
		m_unit = CalendarUnit_Week;
	else
		m_unit = CalendarUnit_None;

	CheckButtons();
}

void CDayWeekMonthBtn::OnMonth()
{
	if (m_unit != CalendarUnit_Month || m_unit == CalendarUnit_None)
		m_unit = CalendarUnit_Month;
	else
		m_unit = CalendarUnit_None;

	CheckButtons();
}
void CDayWeekMonthBtn::ResizeButtons()
{
	RECT rect;
	GetClientRect(&rect);
	int width = rect.right - rect.left;
	int halfWidth = width / 3;

	m_Btns.SetBitmapSize(CSize(0, 0));
	m_Btns.SetButtonWidth(halfWidth, halfWidth);
}
void CDayWeekMonthBtn::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	if (::IsWindow(m_Btns.m_hWnd))
	{
		m_Btns.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);

		ResizeButtons();


	}
}
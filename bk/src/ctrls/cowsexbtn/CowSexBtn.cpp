#include "stdafx.h"

#include "CowSexBtn.h"
#include "../../text/CowSexText.h"

#define BTNS_CTRL_ID 1001

#define BTN_XX_ID 101
#define BTN_XY_ID 102


CCowSexBtn::CCowSexBtn()
{
	
}

CCowSexBtn::~CCowSexBtn()
{
	
}


BEGIN_MESSAGE_MAP(CCowSexBtn, CStatic)
	ON_BN_CLICKED(BTN_XX_ID,OnXX)
	ON_BN_CLICKED(BTN_XY_ID,OnXY)
END_MESSAGE_MAP()



void CCowSexBtn::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(),  WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	CreateButtons();
}

void CCowSexBtn::CheckButtons()
{
	TBBUTTONINFO btnInfo;

	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_STATE;
	
	btnInfo.fsState  =  TBSTATE_ENABLED | (m_sex.IsXX() ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_XX_ID,&btnInfo);

	btnInfo.fsState  =  TBSTATE_ENABLED | (m_sex.IsXY() ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_XY_ID,&btnInfo);
	
}

void CCowSexBtn::SetSex(const CowSex& sex)
{
	m_sex = sex;
	if(IsWindow(GetSafeHwnd()))
		CheckButtons();
}

const CowSex& CCowSexBtn::GetSex() const
{
	return m_sex;
}

void CCowSexBtn::CreateButtons()
{
	RECT rect;
	GetClientRect(&rect);
		
	m_Btns.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS //| 	TBSTYLE_FLAT 
		| CCS_TOP | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,rect,this,BTNS_CTRL_ID);

	
	TBBUTTON xxBtn,xyBtn;
	memset(&xxBtn,0,sizeof(xxBtn));
	memset(&xyBtn,0,sizeof(xyBtn));

	std::wstring xxText = CowSexText(CowSex(SEX_XX)).ToString();
	std::wstring xyText = CowSexText(CowSex(SEX_XY)).ToString();

	xxBtn.iBitmap =  I_IMAGENONE;
	xxBtn.idCommand = BTN_XX_ID;
    xxBtn.fsState = TBSTATE_ENABLED;
    xxBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	xxBtn.iString = (int)"";

	xyBtn.iBitmap =  I_IMAGENONE;
	xyBtn.idCommand = BTN_XY_ID;
    xyBtn.fsState = TBSTATE_ENABLED;
    xyBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	xyBtn.iString = (int)"";
			
	m_Btns.InsertButton(0,&xyBtn);
	m_Btns.InsertButton(0,&xxBtn);

	TBBUTTONINFO btnInfo;
	memset(&btnInfo, 0, sizeof(btnInfo));
	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_TEXT;

	btnInfo.pszText = (LPTSTR)xxText.c_str();
	m_Btns.SetButtonInfo(BTN_XX_ID, &btnInfo);	

	btnInfo.pszText = (LPTSTR)xyText.c_str();
	m_Btns.SetButtonInfo(BTN_XY_ID, &btnInfo);

	//m_Btns.AutoSize();

	int width = rect.right - rect.left;
	int halfWidth = width / 2 - (width & 1 ? 1 : 0);

	m_Btns.SetBitmapSize(CSize(0, 0));
	m_Btns.SetButtonWidth(halfWidth, halfWidth);


	CheckButtons();
		
}


void CCowSexBtn::OnXX()
{
	if(m_sex.IsXY() || m_sex.IsUNK()) 
		m_sex.MakeXX();
	else
		m_sex.MakeUNK();

	CheckButtons();
}

void CCowSexBtn::OnXY()
{
	if(m_sex.IsXX() || m_sex.IsUNK()) 
		m_sex.MakeXY();
	else
		m_sex.MakeUNK();	
	CheckButtons();
}

#include "stdafx.h"

#include "InvoiceTypeBtn.h"
#include "../../text/InvoiceTypeString.h"


#define BTNS_CTRL_ID 1001
#define BTN_REGULAR_ID 101
#define BTN_LUMP_ID 102


CInvoiceTypeBtn::CInvoiceTypeBtn()
{
	
}

CInvoiceTypeBtn::~CInvoiceTypeBtn()
{
	
}


BEGIN_MESSAGE_MAP(CInvoiceTypeBtn, CStatic)
	ON_BN_CLICKED(BTN_REGULAR_ID,OnRegular)
	ON_BN_CLICKED(BTN_LUMP_ID,OnLump)
END_MESSAGE_MAP()



void CInvoiceTypeBtn::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyleEx(GetExStyle(),  WS_EX_CONTROLPARENT);
	ModifyStyle(GetStyle() & ~WS_VISIBLE, WS_CHILD | WS_TABSTOP | WS_GROUP );
	CreateButtons();
}

void CInvoiceTypeBtn::CheckButtons()
{
	TBBUTTONINFO btnInfo;

	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_STATE;
	
	btnInfo.fsState  =  TBSTATE_ENABLED | (m_type == InvoiceType_Regular ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_REGULAR_ID,&btnInfo);

	btnInfo.fsState  =  TBSTATE_ENABLED | (m_type == InvoiceType_Lump ? TBSTATE_CHECKED : 0);
	m_Btns.SetButtonInfo(BTN_LUMP_ID,&btnInfo);
	
}

void CInvoiceTypeBtn::SetInvoiceType(InvoiceType type)
{
	m_type = type;
	if(IsWindow(GetSafeHwnd()))
		CheckButtons();
}

InvoiceType CInvoiceTypeBtn::GetInvoiceType() const
{
	return m_type;
}

void CInvoiceTypeBtn::CreateButtons()
{
	RECT rect;
	GetClientRect(&rect);
		
	m_Btns.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS //| 	TBSTYLE_FLAT 
		| CCS_TOP | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER,rect,this,BTNS_CTRL_ID);

	
	TBBUTTON regularBtn,lumpBtn;
	memset(&regularBtn,0,sizeof(regularBtn));
	memset(&lumpBtn,0,sizeof(lumpBtn));

	std::wstring regularText = InvoiceTypeString(InvoiceType_Regular).ToString();
	std::wstring lumpText = InvoiceTypeString(InvoiceType_Lump).ToString();

	regularBtn.iBitmap =  I_IMAGENONE;
	regularBtn.idCommand = BTN_REGULAR_ID;
	regularBtn.fsState = TBSTATE_ENABLED;
	regularBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	regularBtn.iString = (int)"";

	lumpBtn.iBitmap =  I_IMAGENONE;
	lumpBtn.idCommand = BTN_LUMP_ID;
	lumpBtn.fsState = TBSTATE_ENABLED;
	lumpBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	lumpBtn.iString = (int)"";
			
	m_Btns.InsertButton(0,&regularBtn);
	m_Btns.InsertButton(0,&lumpBtn);

	TBBUTTONINFO btnInfo;
	memset(&btnInfo, 0, sizeof(btnInfo));
	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_TEXT;

	btnInfo.pszText = (LPTSTR)regularText.c_str();
	m_Btns.SetButtonInfo(BTN_REGULAR_ID, &btnInfo);

	btnInfo.pszText = (LPTSTR)lumpText.c_str();
	m_Btns.SetButtonInfo(BTN_LUMP_ID, &btnInfo);

	//m_Btns.AutoSize();

	int width = rect.right - rect.left;
	int halfWidth = width / 2 - (width & 1 ? 1 : 0);

	m_Btns.SetBitmapSize(CSize(0, 0));
	m_Btns.SetButtonWidth(halfWidth, halfWidth);


	CheckButtons();
		
}


void CInvoiceTypeBtn::OnRegular()
{
	if (m_type == InvoiceType_None || m_type == InvoiceType_Lump)
		m_type = InvoiceType_Regular;
	else
		m_type = InvoiceType_None;

	CheckButtons();
}

void CInvoiceTypeBtn::OnLump()
{
	if (m_type == InvoiceType_None || m_type == InvoiceType_Regular)
		m_type = InvoiceType_Lump;
	else
		m_type = InvoiceType_None;

	CheckButtons();
}

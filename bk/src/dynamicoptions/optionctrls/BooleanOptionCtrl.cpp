#include "stdafx.h"
#include "BooleanOptionCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../../text/YesNoText.h"
#include "../../options/PrintOptionSerializer.h"

#define ID_TOOLBAR 1001
#define BTN_Y_ID ((int)'Y')
#define BTN_N_ID ((int)'N')

BEGIN_MESSAGE_MAP(BooleanOptionCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
	
	ON_BN_CLICKED(BTN_Y_ID, OnY)
	ON_BN_CLICKED(BTN_N_ID, OnN)

	ON_WM_SIZE()
	
END_MESSAGE_MAP()

BooleanOptionCtrl::BooleanOptionCtrl()
{	
}

BooleanOptionCtrl::~BooleanOptionCtrl()
{
}

PrintOption BooleanOptionCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	PrintOptionSerializer serializer;
	serializer.SetValue(option, GetBoolean());	
	return option;
}

void BooleanOptionCtrl::SetOptionValue(const PrintOption& option)
{
	PrintOptionSerializer serializer;
	bool b = serializer.GetBooleanValue(option);
	SetBoolean(b);
}

void BooleanOptionCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void BooleanOptionCtrl::CreateContent()
{

	m_YNBtns.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS //| 	TBSTYLE_FLAT 
		| CCS_TOP | CCS_NOPARENTALIGN | CCS_NORESIZE | CCS_NODIVIDER, CRect(0,0,0,0), this, ID_TOOLBAR);


	TBBUTTON yBtn, nBtn;
	memset(&yBtn, 0, sizeof(yBtn));
	memset(&nBtn, 0, sizeof(nBtn));

	std::wstring yText = YesNoText(true).ToString();
	std::wstring nText = YesNoText(false).ToString();

	yBtn.iBitmap = I_IMAGENONE;
	yBtn.idCommand = BTN_Y_ID;
	yBtn.fsState = TBSTATE_ENABLED;
	yBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	yBtn.iString = (int)yText.c_str();

	nBtn.iBitmap = I_IMAGENONE;
	nBtn.idCommand = BTN_N_ID;
	nBtn.fsState = TBSTATE_ENABLED;
	nBtn.fsStyle = BTNS_CHECK | BTNS_SHOWTEXT;// | TBSTYLE_LIST;
	nBtn.iString = (int)nText.c_str();


	//int width = rect.right - rect.left;
	//int halfWidth = width / 2 - (width & 1 ? 1 : 0);

	m_YNBtns.SetBitmapSize(CSize(0, 0));
	//m_YNBtns.SetButtonWidth(halfWidth, halfWidth);

	m_YNBtns.InsertButton(0, &nBtn);
	m_YNBtns.InsertButton(0, &yBtn);
	

	m_Value = false;
	CheckButtons();

	SetWindowPos(NULL, 0, 0, 0, 25, SWP_NOMOVE);
}

void BooleanOptionCtrl::OnEnable(BOOL bEnable)
{
	CStatic::OnEnable(bEnable);
}

void BooleanOptionCtrl::CheckButtons()
{
	TBBUTTONINFO btnInfo;

	btnInfo.cbSize = sizeof(TBBUTTONINFO);
	btnInfo.dwMask = TBIF_STATE;

	btnInfo.fsState = TBSTATE_ENABLED | (m_Value ? TBSTATE_CHECKED : 0);
	m_YNBtns.SetButtonInfo(BTN_Y_ID, &btnInfo);

	btnInfo.fsState = TBSTATE_ENABLED | (!m_Value ? TBSTATE_CHECKED : 0);
	m_YNBtns.SetButtonInfo(BTN_N_ID, &btnInfo);

}

bool BooleanOptionCtrl::GetBoolean() const
{
	return m_Value;
}

void BooleanOptionCtrl::SetBoolean(bool b)
{
	m_Value = b;
	if (::IsWindow(m_YNBtns.m_hWnd))
		CheckButtons();
}


void BooleanOptionCtrl::OnY()
{
	m_Value = true;
	CheckButtons();
}

void BooleanOptionCtrl::OnN()
{
	m_Value = false;
	CheckButtons();
}

void BooleanOptionCtrl::OnSize(UINT nType, int cx, int cy)
{
	DocumentOptionCtrl::OnSize(nType, cx, cy);

	if (::IsWindow(m_YNBtns.m_hWnd))
	{
		m_YNBtns.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
		int width = cx;
		int halfWidth = width / 2 - (width & 1 ? 1 : 0);
		m_YNBtns.SetButtonWidth(halfWidth, halfWidth);
	}


}
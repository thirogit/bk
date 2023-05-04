#include "stdafx.h"
#include "IntValueSliderCtrl.h"

#define ID_SLIDERVALSLIDERID 0x1001
#define ID_SLIDERVALVALUEID 0x1002
#define ID_SLIDERVALPLUSID 0x1003
#define ID_SLIDERVALMINUSID 0x1004
#define SLIDER_HEIGHT 20

CIntValueSliderCtrl::CIntValueSliderCtrl()
{
	m_Slider = new CSliderCtrl;
	m_SliderVal = new CStatic;
	
	m_plusLabel = new CStatic;
	m_minusLabel = new CStatic;
}

CIntValueSliderCtrl::~CIntValueSliderCtrl()
{
	delete m_Slider;
	delete m_SliderVal;
	delete m_plusLabel;
	delete m_minusLabel;
	
}


BEGIN_MESSAGE_MAP(CIntValueSliderCtrl, CButton)
	ON_WM_HSCROLL()
	ON_WM_ENABLE()
END_MESSAGE_MAP()


void CIntValueSliderCtrl::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	CreateControls();
}


BOOL CIntValueSliderCtrl::CreateControls()
{

	RECT rect;
	CDC *pDC = NULL;	
	GetClientRect(&rect);

	UINT width = rect.right-rect.left,
		height = rect.bottom-rect.top,
		nextCtrlPos = 0,
		sliderRight = 0,
		startTopPos = 15;
    
	ASSERT((rect.right-rect.left) >= 80 && (rect.bottom-rect.top) >= 60);

	sliderRight = ((width)-4);
	nextCtrlPos = startTopPos + SLIDER_HEIGHT;
	if(!m_Slider->Create(WS_CHILD  | WS_VISIBLE  |  WS_TABSTOP, 
		CRect(4, startTopPos,sliderRight ,nextCtrlPos),
		this, ID_SLIDERVALSLIDERID))
		  return FALSE;

	pDC = GetDC();

	nextCtrlPos += 2;
	CSize plusminusSize = pDC->GetTextExtent(L"+",1);

	if(!m_plusLabel->Create(L"+",WS_CHILD | WS_VISIBLE, 
		CRect(4,nextCtrlPos ,4 + plusminusSize.cx, nextCtrlPos + plusminusSize.cy), 
		this, 	ID_SLIDERVALPLUSID))
		return FALSE;
	m_plusLabel->SetFont(GetFont());

	if(!m_minusLabel->Create(L"-",WS_CHILD | WS_VISIBLE, 
		CRect(sliderRight-plusminusSize.cx,nextCtrlPos ,sliderRight, nextCtrlPos + plusminusSize.cy), 
		this, 	ID_SLIDERVALMINUSID))
		return FALSE;
	m_minusLabel->SetFont(GetFont());

	CSize valSize = pDC->GetTextExtent(L"000",3);
	
	nextCtrlPos += 2;

	if(!m_SliderVal->Create(L"",WS_CHILD | WS_VISIBLE | ES_CENTER , 
		CRect((width-valSize.cx)/2,nextCtrlPos ,(width+valSize.cx)/2 , nextCtrlPos+valSize.cy), 
		this, 	ID_SLIDERVALVALUEID))
		return FALSE;

	m_SliderVal->SetFont(GetFont());
 	
	ReleaseDC(pDC);

	CWnd * enableshow[4] = 
	{
			m_Slider,
			m_SliderVal,
			m_plusLabel,
			m_minusLabel,
	};

	for(int i = 0;i < 4;i++)
	{
		enableshow[i]->EnableWindow(TRUE);
		enableshow[i]->ShowWindow(SW_SHOW);
	}
	
	return TRUE;
}
void CIntValueSliderCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateValueLabel(GetPos());	
}

int CIntValueSliderCtrl::MapSliderPos(int nPos)
{
	return nPos;
}

void CIntValueSliderCtrl::UpdateValueLabel(int nPos)
{
	int nMappedPos = MapSliderPos(nPos);
	wchar_t szLabelText[17];
	_snwprintf_s(szLabelText,sizeof(szLabelText),L"%d",nMappedPos);
	m_SliderVal->SetWindowText(szLabelText);
}

void CIntValueSliderCtrl::SetBounds(int nMin,int nMax)
{
	m_Slider->SetRange(nMin,nMax);
}

void CIntValueSliderCtrl::SetTic(int nTic)
{
	m_Slider->SetTicFreq(nTic);
}

void CIntValueSliderCtrl::SetPage(int nPage)
{
	m_Slider->SetPageSize(nPage);
}

int CIntValueSliderCtrl::GetPos()
{
	return m_Slider->GetPos();
}

void CIntValueSliderCtrl::SetPos(int nPos)
{
	m_Slider->SetPos(nPos);
	UpdateValueLabel(GetPos());

}

void CIntValueSliderCtrl::OnEnable(BOOL bEnable)
{
	m_Slider->EnableWindow(bEnable);
	m_SliderVal->EnableWindow(bEnable);
	m_plusLabel->EnableWindow(bEnable);
	m_minusLabel->EnableWindow(bEnable);
	CWnd::OnEnable(bEnable);

}

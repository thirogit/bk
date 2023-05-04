#include "stdafx.h"
#include "DecimalFormatCtrl.h"
#include "../../../resource.h"
#include "../../../utils/StringLoader.h"
#include <arrays\DeepDelete.h>
#include <boost/foreach.hpp>
#include "../../options/PrintOptionSerializer.h"
#include <string\IntString.h>

#define ID_DECIMALPT_CB_ID 1001
#define ID_PRECISION_CB_ID 1002

BEGIN_MESSAGE_MAP(DecimalFormatCtrl, DocumentOptionCtrl)
	ON_WM_DESTROY()
	ON_WM_ENABLE()
END_MESSAGE_MAP()

DecimalFormatCtrl::DecimalFormatCtrl()
{	
	m_pDecimalPtLabel = new CStatic;
	m_pPrecisionLabel = new CStatic;
	m_pPrecisionCb = new CComboBox;
	m_pDecimalPtCb = new CComboBox;
	
}

DecimalFormatCtrl::~DecimalFormatCtrl()
{
	delete m_pDecimalPtLabel;
	delete m_pPrecisionLabel;
	delete m_pPrecisionCb;
	delete m_pDecimalPtCb;
}

PrintOption DecimalFormatCtrl::GetOptionValue()
{
	PrintOption option(GetOptionCode());
	DecimalFormat fmt = GetFormat();
	PrintOptionSerializer seriazlizer;
	seriazlizer.SetValue(option, fmt);
	return option;
}

void DecimalFormatCtrl::SetOptionValue(const PrintOption& option)
{
	DecimalFormat fmt = PrintOptionSerializer().GetDecimalFormatValue(option);
	SetFormat(fmt);
}

void DecimalFormatCtrl::OnDestroy()
{
	DocumentOptionCtrl::OnDestroy();	
}

void DecimalFormatCtrl::CreateContent()
{
	std::wstring sDecimalPtLblTxt = StringLoader(IDS_DECIMALPOINTLBL);
	CClientDC dc(this);

	int decimalPtLblLeft = 3;
	int decimalPtLblRight = decimalPtLblLeft+70;
	int decimalPtCbLeft = decimalPtLblRight+5;
	int decimalPtCbRight = decimalPtCbLeft+45;
	int decimalPtCbTop = 0;
	int decimalPtCbBottom = decimalPtCbTop+200;
	CRect decimalPtCbRect(decimalPtCbLeft,decimalPtCbTop,decimalPtCbRight,decimalPtCbBottom);
	m_pDecimalPtCb->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,decimalPtCbRect,this,ID_DECIMALPT_CB_ID);
	m_pDecimalPtCb->EnableWindow(TRUE);
	m_pDecimalPtCb->ShowWindow(SW_SHOW);
	m_pDecimalPtCb->SetFont(GetFont());

	CRect cbRect;
	m_pDecimalPtCb->GetClientRect(&cbRect);
	int cbHeight = cbRect.Height();
	CSize decimalPtTxtSize = dc.GetTextExtent(sDecimalPtLblTxt.c_str());

	int middleOffset = (cbHeight-decimalPtTxtSize.cy)/2;
	int decimalPtLblTop = middleOffset;

	CRect decimalPtLblRect(decimalPtLblLeft,decimalPtLblTop,decimalPtLblRight,decimalPtLblTop+decimalPtTxtSize.cy);

	m_pDecimalPtLabel->Create(sDecimalPtLblTxt.c_str(),WS_CHILD | WS_VISIBLE,decimalPtLblRect,this);
	m_pDecimalPtLabel->EnableWindow(TRUE);
	m_pDecimalPtLabel->ShowWindow(SW_SHOW);
	m_pDecimalPtLabel->SetFont(GetFont());
	
	std::wstring sPrecisionLblText = StringLoader(IDS_PRECISIONLBL);
	CSize precisionTxtSize = dc.GetTextExtent(sPrecisionLblText.c_str());
	
	int precLblLeft = decimalPtCbRect.right+10;
	int precLblRight = precLblLeft+precisionTxtSize.cx;
	
	int precisionCbLeft = precLblRight+5;
	int precisionCbRight = precisionCbLeft+45;
	int precisionCbTop = 0;
	int precisionCbBottom = precisionCbTop+200;
	CRect precisionCbRect(precisionCbLeft,precisionCbTop,precisionCbRight,precisionCbBottom);
	m_pPrecisionCb->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP,precisionCbRect,this,ID_PRECISION_CB_ID);
	m_pPrecisionCb->EnableWindow(TRUE);
	m_pPrecisionCb->ShowWindow(SW_SHOW);
	m_pPrecisionCb->SetFont(GetFont());
	
	m_pPrecisionCb->GetClientRect(&cbRect);
	cbHeight = cbRect.Height();

	middleOffset = (cbHeight-precisionTxtSize.cy)/2;
	int precLblTop = middleOffset;
	int precLblBottom = precLblTop+precisionTxtSize.cy;
	CRect precisionLblRect(precLblLeft,precLblTop,precLblRight,precLblBottom);
	m_pPrecisionLabel->Create(sPrecisionLblText.c_str(),WS_CHILD | WS_VISIBLE,precisionLblRect,this);
	m_pPrecisionLabel->EnableWindow(TRUE);
	m_pPrecisionLabel->ShowWindow(SW_SHOW);
	m_pPrecisionLabel->SetFont(GetFont());
	InitContent(DecimalFormat());
	SetWindowPos(NULL, 0, 0, precisionCbRight, 25, SWP_NOMOVE);

}

void DecimalFormatCtrl::InitContent(const DecimalFormat& format)
{
	const char decimalPointCharacters[] = {'.',',',0};
	const int precisions[] = {2,3,4,5,6,0};
	
	m_pDecimalPtCb->ResetContent();
	CString sItemText;
	int iItem,iSelectedItem = -1;
	const char* pDecimalPoint = decimalPointCharacters;
	while(*pDecimalPoint)
	{
		sItemText.Empty();
		sItemText += *pDecimalPoint;
		iItem = m_pDecimalPtCb->AddString(sItemText);
		m_pDecimalPtCb->SetItemData(iItem,*pDecimalPoint);

		if(*pDecimalPoint == format.GetDecimalPoint() && format.GetDecimalPoint() != '\0')
		{
			iSelectedItem = iItem;
		}

		pDecimalPoint++;
	}

	if(iSelectedItem < 0)
	{
		sItemText.Empty();
		sItemText += format.GetDecimalPoint();
		iSelectedItem = m_pDecimalPtCb->AddString(sItemText);
		m_pDecimalPtCb->SetItemData(iSelectedItem,format.GetDecimalPoint());
	}
	m_pDecimalPtCb->SetCurSel(iSelectedItem);


	m_pPrecisionCb->ResetContent();
	iSelectedItem = -1;
	const int* pPresision = precisions;
	while(*pPresision)
	{
		iItem = m_pPrecisionCb->AddString(IntString(*pPresision).c_str());
		m_pPrecisionCb->SetItemData(iItem,*pPresision);

		if(*pPresision == format.GetPrecision() && format.GetPrecision() > 0)
		{
			iSelectedItem = iItem;
		}

		pPresision++;
	}

	if(iSelectedItem < 0)
	{
		iSelectedItem = m_pPrecisionCb->AddString(IntString(format.GetPrecision()).c_str());
		m_pPrecisionCb->SetItemData(iItem,format.GetPrecision());
	}
	m_pPrecisionCb->SetCurSel(iSelectedItem);
}

void DecimalFormatCtrl::OnEnable(BOOL bEnable)
{
	m_pDecimalPtLabel->EnableWindow(bEnable);
	m_pPrecisionLabel->EnableWindow(bEnable);
	m_pPrecisionCb->EnableWindow(bEnable);
	m_pDecimalPtCb->EnableWindow(bEnable);

	CStatic::OnEnable(bEnable);
}

DecimalFormat DecimalFormatCtrl::GetFormat() const
{
	char cDecimalPoint = '\0';
	int iPrecision = 0;

	int iSelectedPrecision = m_pPrecisionCb->GetCurSel();
	int iSelectedDecimalPoint = m_pDecimalPtCb->GetCurSel();
	if(iSelectedDecimalPoint >= 0)
	{
		cDecimalPoint = (char)m_pDecimalPtCb->GetItemData(iSelectedDecimalPoint);
	}

	if(iSelectedPrecision >= 0)
	{
		iPrecision = (int)m_pPrecisionCb->GetItemData(iSelectedPrecision);
	}

	return DecimalFormat(iPrecision,cDecimalPoint);
}

void DecimalFormatCtrl::SetFormat(const DecimalFormat& fmt)
{
	InitContent(fmt);
}

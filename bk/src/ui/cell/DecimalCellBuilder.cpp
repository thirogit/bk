#include "stdafx.h"
#include "DecimalCellBuilder.h"
#include <format\StringFormat.h>
#include <functional>

#define IDC_DECIMALEDITCELL 9005



BEGIN_MESSAGE_MAP(DecimalCellBuilder::CDecimalEditCell, CEditCell)
	
	ON_WM_CHAR()	

END_MESSAGE_MAP()

void DecimalCellBuilder::CDecimalEditCell::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (nChar == ',')
	{
		this->SendMessage(WM_CHAR,'.');
	}
	else
	{
		CEditCell::OnChar(nChar, nRepCnt, nFlags);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////

DecimalCellBuilder::DecimalCellBuilder() : m_precision(2)
{
}

DecimalCellBuilder& DecimalCellBuilder::precision(int prec)
{
	m_precision = max(1,prec);
	return *this;
}

DecimalCellBuilder& DecimalCellBuilder::value(const NullDecimal& decimal)
{
	m_value = decimal;
	return *this;
}

DecimalCellBuilder& DecimalCellBuilder::receiver(DecimalReceiver& receiver)
{
	m_receiver = receiver;
	return *this;
}

void DecimalCellBuilder::OnText(DecimalReceiver fwdRecv,const std::wstring& text)
{
	NullDecimal decimal;

	if(!text.empty())
		decimal = Decimal::FromString(text);

	if(fwdRecv)
	{
		fwdRecv(decimal);
	}
}

void DecimalCellBuilder::show(CWnd* pParent,const CRect& rect)
{

	CDecimalEditCell *pEdit = new CDecimalEditCell();
	pEdit->SetRegex(StringFormat(L"[0-9]{1,6}([,.][0-9]{0,%d})?", m_precision));
	TextReceiver txtRecv = std::bind(&DecimalCellBuilder::OnText, this, m_receiver, std::placeholders::_1);
	pEdit->SetTextReceiver(txtRecv);
	pEdit->Create(rect, pParent, IDC_DECIMALEDITCELL);

	if (!m_value.IsNull())
	{
		pEdit->SetText(m_value.ToString(m_precision));
	}
	
	pEdit->SetSel(0, -1);

	
	
}




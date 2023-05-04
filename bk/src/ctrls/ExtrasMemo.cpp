#include "stdafx.h"
#include "ExtrasMemo.h"
#include <string>
#include "../utils/StringLoader.h"
#include <boost\format.hpp>
#include "../resource.h"

BEGIN_MESSAGE_MAP(CExtrasMemo,CDimEditCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()


CExtrasMemo::CExtrasMemo(int charsLimit) : m_CharsLimit(charsLimit)
{
}

void CExtrasMemo::PreSubclassWindow()
{
	CDimEditCtrl::PreSubclassWindow();
	SetExtrasMemo();	
}

void CExtrasMemo::SetExtrasMemo()
{
	LimitText( m_CharsLimit );

	std::wstring sDimText = (boost::wformat(StringLoader(IDS_EXTRASMEMO_FMT)) % m_CharsLimit).str();
	SetDimText( sDimText.c_str() );
	SetDimColor( DIMCOLOR);
}

int CExtrasMemo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CDimEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetExtrasMemo();
	return 0;
}
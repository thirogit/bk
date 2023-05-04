#include "stdafx.h"
#include "GeoCoordinateEdit.h"
#include <stdio.h>
#include "../resource.h"
#include "../utils/StringLoader.h"

const TCHAR* szGEOCOORD_FORMAT = _T("%03d\xB0 %02d' %02d\" %c");

BEGIN_MESSAGE_MAP(CGeoCoordinateEdit, CTipEditWrapper<COXMaskedEdit>)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


CGeoCoordinateEdit::CGeoCoordinateEdit() 
{
	SetMask(L"###\xB0 ##' ##\" >");
}

bool CGeoCoordinateEdit::IsValidGeoCoordinate()
{
	if(IsInputEmpty())
		return true;

	unsigned short degrees;
	unsigned short minutes;
	unsigned short seconds;
	Hemisphere hemisphere = NullHemisphere();
	return 	GetGeoCoordinate(degrees,minutes,seconds,hemisphere);
}
void CGeoCoordinateEdit::SetGeoCoordinate(const GeoCoordinate& geoCoord)
{
	CString sGeoCoord;
	sGeoCoord.Format(szGEOCOORD_FORMAT,geoCoord.GetDegrees(),geoCoord.GetMinutes(),geoCoord.GetSeconds(),geoCoord.GetHemisphere().GetSuffix());
	SetWindowText(sGeoCoord);
}

bool CGeoCoordinateEdit::GetGeoCoordinate(unsigned short& degrees,unsigned short& minutes,unsigned short& seconds,Hemisphere& hemisphere) const
{
	if(IsInputEmpty())
		return false;

	if(!IsFullPrompt())
		return false;

	int degs = 181,mins = 61,secs = 61;
	int cHemisphere = 0;

	std::wstring sGeoCoord = GetText();

	if (_stscanf_s(sGeoCoord.c_str(), szGEOCOORD_FORMAT, &degs, &mins, &secs, &cHemisphere) != 4)
		return false;
	
	degrees = degs;
	minutes = mins;
	seconds = secs;
	hemisphere = HemisphereFactory::GetHemisphere(cHemisphere);

	return true;
}


BOOL CGeoCoordinateEdit::OnValidate()
{
	if(!CTipEditWrapper<COXMaskedEdit>::OnValidate()) return FALSE;

	if(!IsValidGeoCoordinate())
	{
		SetToolTipText(GetValidationErrorMessage().c_str());
		SetToolTipTitle(TTI_ERROR,StringLoader(IDS_ERRINVALID_GEOCOORDINATE).c_str());
		ShowToolTip();
		return FALSE;
	}
	return TRUE;
}

void	CGeoCoordinateEdit::OnPaint() 
{
	Default();
	DrawGetCoordHint();	
}


void CGeoCoordinateEdit::DrawGetCoordHint()
{
	std::wstring geoCoordHint = GetGeoCoordHint();
	if( geoCoordHint.empty() )
	{
		return;					
	}
	CClientDC	dcDraw( this );
	CRect		rRect;
	int			iState = dcDraw.SaveDC();					// Save The DC State
	
	GetClientRect( &rRect );								// Get Drawing Area
//	rRect.OffsetRect( 1, 1 );								// Add Sanity Space
	
	dcDraw.SelectObject( (*GetFont()) );					// Use The Control's Current Font
	dcDraw.SetTextColor( RGB(192,192,192) );				// Set The Text Color
	dcDraw.SetBkColor( GetSysColor( COLOR_WINDOW ) );		// Set The Bk Color
	dcDraw.DrawText( geoCoordHint.c_str(), geoCoordHint.size(), &rRect, 
			( DT_RIGHT | DT_VCENTER ) );					// Draw The Dim Text
	
	dcDraw.RestoreDC( iState );								// Restore The DC State

	return;													// Done!
}


BOOL CGeoCoordinateEdit::OnEraseBkgnd(CDC* pDC) 
{
	BOOL	bStatus = CEdit::OnEraseBkgnd(pDC);

	if( bStatus )
	{
		DrawGetCoordHint();									
	}
	return( bStatus );										
}

void CGeoCoordinateEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CTipEditWrapper<COXMaskedEdit>::OnSetFocus(pOldWnd);
	DrawGetCoordHint();
}

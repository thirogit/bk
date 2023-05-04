#ifndef __GEOCOORDINATEEDIT_H__
#define __GEOCOORDINATEEDIT_H__

#include "tipedit/tipedit.h"
#include <types/GeoCoordinate.h>
#include "maskededit/OXMaskedEdit.h"

class CGeoCoordinateEdit : public CTipEditWrapper<COXMaskedEdit>
{
protected:
	CGeoCoordinateEdit();
	virtual bool IsValidGeoCoordinate();
	void SetGeoCoordinate(const GeoCoordinate& geoCoord);
	bool GetGeoCoordinate(unsigned short& degrees,unsigned short& minutes,unsigned short& seconds,Hemisphere& hemisphere) const;

protected:	
	virtual BOOL OnValidate();	
	virtual std::wstring GetValidationErrorMessage() const = 0;
	virtual std::wstring GetGeoCoordHint() const = 0;
	void DrawGetCoordHint();
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd) ;

	DECLARE_MESSAGE_MAP()

};


#endif
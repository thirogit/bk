#ifndef __COUNTRYBUTTON_H__
#define __COUNTRYBUTTON_H__

#include <afxwin.h>
#include <data\types\countries\Country.h>
#include "../countryflags/CountryFlagsImageList.h"

class CCountryButton : public CButton
{
public:
	CCountryButton();
	virtual ~CCountryButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetCountry(const Country& country);
	const Country& GetCountry() const;

protected:
	virtual void PreSubclassWindow();
	void DestroyFlagIcon();
protected:
	
protected:
	Country m_Country;
	CountryFlagsImageList m_flagsIL;
	CBitmap m_flagIcon;
	
protected:
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};


#endif 

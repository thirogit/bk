#pragma once

#include <afxwin.h>

class CSectionHeader : public CStatic
{
public:	
	CSectionHeader();
	virtual ~CSectionHeader();
	
protected:
	virtual void PreSubclassWindow();
	void DrawEtchedLine(CDC *pDC, const CRect& rect);
	int DrawText(CDC *pDC, const CRect& rect);
	CFont * GetSafeFont();

	afx_msg void OnPaint();
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	DECLARE_MESSAGE_MAP()

private:
	
};
#pragma once

class CHerdNumbers : public CStatic, public HerdObserver
{
public:
	CHerdNumbers(uint32_t herdId);	
	void SetNumbers(size_t total,size_t instock);
	
protected:
	virtual void PreSubclassWindow();
	
	void Register();
	void Unregister();

	CFont		m_font;
	COLORREF	m_rgbText;
	COLORREF	m_rgbBackground;
	uint32_t	m_herdId;
			
protected:
	
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	DECLARE_MESSAGE_MAP()
};
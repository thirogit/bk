#ifndef __ARROWBUTTON_H__
#define __ARROWBUTTON_H__

#define ABS_ARROW_UP 0x0001
#define ABS_ARROW_DOWN 0x0002
#define ABS_ARROW_LEFT 0x0004
#define ABS_ARROW_RIGHT 0x0008


class CArrowButton : public CButton
{
public:
	CArrowButton();
	virtual ~CArrowButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void SetArrowStyle(WORD style);
	WORD GetArrowStyle();

protected:
	virtual void PreSubclassWindow();
protected:
	
protected:
	
	COLORREF m_crEnabledColor;
	CRect    m_ArrowRect;
	WORD	 m_arrowStyle;
protected:
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	

	DECLARE_MESSAGE_MAP()
};


#endif 

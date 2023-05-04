#ifndef __INTVALUESLIDERCTRL_H__
#define __INTVALUESLIDERCTRL_H__

class CIntValueSliderCtrl : public CButton
{
public:
	CIntValueSliderCtrl();
	virtual ~CIntValueSliderCtrl();

	void SetBounds(int nMin,int nMax);
	void SetTic(int nTic);
	void SetPage(int nPage);

	int GetPos();
	void SetPos(int nPos);

protected:

	virtual void PreSubclassWindow();
	void UpdateValueLabel(int nPos);
	CSliderCtrl *m_Slider;
	CStatic *m_SliderVal;
	CStatic *m_plusLabel;
	CStatic *m_minusLabel;
	
	
	BOOL CreateControls();
	
	virtual int MapSliderPos(int nPos);
	
protected:
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
};


#endif


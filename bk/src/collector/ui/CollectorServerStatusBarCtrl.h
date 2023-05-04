#if !defined(AFX_EXTLABEL_H__698A45B4_27B1_4AE6_B651_0DA439882A9B__INCLUDED_)
#define AFX_EXTLABEL_H__698A45B4_27B1_4AE6_B651_0DA439882A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CCollectorServerStatusBarCtrl : public CStatic
{
	static LOGFONT				lf_default;
public:
	CCollectorServerStatusBarCtrl();
	virtual ~CCollectorServerStatusBarCtrl();
private:

	int m_NumberOfReadyForSync;
	int m_NumberOfBusy;
	int m_NumberOfSyncing;

	CFont m_font;
	
protected:
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_EXTLABEL_H__698A45B4_27B1_4AE6_B651_0DA439882A9B__INCLUDED_)

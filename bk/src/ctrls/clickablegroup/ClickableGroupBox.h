#ifndef __CLICKABLEGROUPBOX_H__
#define __CLICKABLEGROUPBOX_H__

class CClickableGroupBox : public CButton
{
	DECLARE_DYNAMIC(CClickableGroupBox)
public:
	CClickableGroupBox();
	virtual ~CClickableGroupBox();
protected:
	
	CButton	 m_TitleBtn;
protected:
	virtual void PreSubclassWindow();


	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClicked() ;

	DECLARE_MESSAGE_MAP()
	
};

#endif 

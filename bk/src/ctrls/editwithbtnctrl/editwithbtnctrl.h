#pragma once

#include "afxcontrolbarutil.h"
#include "../maskededit/OXMaskedEdit.h"

class CEditWithBtnCtrl : public COXMaskedEdit
{
	DECLARE_DYNAMIC(CEditWithBtnCtrl)

// Construction
public:
	CEditWithBtnCtrl();

// Attributes
public:
	
	enum ButtonState
	{
		ButtonsIsRegular,
		ButtonsIsPressed,
		ButtonsIsHighlighted,
	};
	
protected:
	int  m_nBrowseButtonWidth;
	BOOL m_bIsButtonPressed;
	BOOL m_bIsButtonHighlighted;
	BOOL m_bIsButtonCaptured;
	
	CRect   m_rectBtn;
	CSize   m_sizeImage;

	CImageList m_ImageBrowse;

// Operations
public:
	void EnableButton();
	

	void SetButtonImage(HICON hIcon, BOOL bAutoDestroy = TRUE);
	void SetButtonImage(HBITMAP hBitmap, BOOL bAutoDestroy = TRUE);
	void SetButtonImage(UINT uiBmpResId);
	BOOL OnDrawBrowseButton(CDC* pDC, CRect rect, ButtonState state, COLORREF& /*clrText*/);
// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnBtnClick();
	virtual void OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot);
	virtual void OnChangeLayout();
	virtual void OnAfterUpdate();

// Implementation
public:
	virtual ~CEditWithBtnCtrl();

protected:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
	

	DECLARE_MESSAGE_MAP()

	
};



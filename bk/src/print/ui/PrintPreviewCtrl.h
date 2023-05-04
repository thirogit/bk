#pragma once

#include "../../bkprint/renderer/PrintOut.h"

class CPreviewViewCtrl : public CWnd
{
public:
	CPreviewViewCtrl();
	~CPreviewViewCtrl();
	
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT nType, int cx, int cy);
	CSize CalcScaleRatio(CSize screenSize, CSize actualSize);
	void PositionPage(UINT nPage);
	CSize CalcPageDisplaySize();
	void SetScaledSize(UINT nPage);
	
	void OnDraw(CDC* pDC);
	
	void OnNumPageChange();
	void OnNextPage();
	void OnPrevPage();
	BOOL InPageRect(const CPoint& point);
	void OnLButtonDown(UINT, CPoint point);
	void SetZoomState(UINT nNewState, CPoint point);
	void OnZoomIn();
	void OnZoomOut();
	void DoZoom(CPoint point);
	void SetCurrentPage(UINT nPage, BOOL bClearRatios);
	UINT GetCurrentPage() const;
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	bool InitializeDefaultPrinter(CPrintDialog* pPrinterDlg);
	bool SetPrintOut(bkprint::PrintOut* pPrintOut);
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	void GetScrollBarSizes(CSize& sizeSb);
	CPoint GetDeviceScrollPosition() const;
	void CenterOnPoint(CPoint ptCenter);

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID ) ;

protected:
	bool RegisterWindowClass();
	void UnregisterWindowClass();
	void DisposePrintDlg();

	void SetScrollSizes(const CSize& page,const CSize& window);
	void SetPaper(CPrintDialog* pPrinterDlg,const bkprint::Paper& paper);
	BOOL CreateCanvas(CPrintDialog* pPrinterDlg);
protected:
	
	CPreviewDC* m_pPreviewDC;  // Output and attrib DCs Set, not created
	CDC m_dcPrint;             // Actual printer DC

	struct PAGE_INFO
	{
		CRect rectScreen; // screen rect (screen device units)
		CSize sizeUnscaled; // unscaled screen rect (screen device units)
		CSize sizeScaleRatio; // scale ratio (cx/cy)
		CSize sizeZoomOutRatio; // scale ratio when zoomed out (cx/cy)
	};

	PAGE_INFO m_PageInfo; 
	
	UINT m_nZoomState;
	
	UINT m_nCurrentPage;
	
	HCURSOR m_hMagnifyCursor;

	CSize m_sizePrinterPPI; // printer pixels per inch
	CPoint m_ptCenterPoint;

	int m_xScroll;
	int m_yScroll;

	CPrintDialog* m_pPrinterDlg;
	bkprint::PrintOut* m_pPrintOut;
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP();
};

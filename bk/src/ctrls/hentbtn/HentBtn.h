#ifndef __HENTBTN_H__
#define __HENTBTN_H__

#include <cstdint>
#include "../../SessionAwareWnd.h"
#include <data\HentType.h>
#include "../../utils/BmpImageList.h"


#define BN_HENTCHANGE 0x0100
#define BN_EDITHENT 0x0200
#define HBS_EX_NOEDITHENT 0x00000001L

class CHentBtn : public CSessionAwareWnd<CButton>
{
public:
	CHentBtn();
	virtual ~CHentBtn();
	BOOL Create( DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
		
	void InitHent(uint32_t hentId);
	void SetHent(uint32_t hentId);
	uint32_t GetHent() const;
	

protected:
	CMenu        m_menu;
	CBitmap      m_btnImage;
	CBmpImageList   m_IL;
	BOOL         m_bPushed;
	BOOL         m_bMenuPushed;
		
	CWnd*        m_pParentWnd;
	
	COLORREF	 m_crMask;	
	BOOL         m_IsMenuEnabled;

	std::wstring m_sNoneHent;

	std::wstring m_sContentText;	
	HentType	 m_hentType;
	uint32_t	 m_hentId;

protected:
	void DrawArrow(CDC* pDC,CPoint ArrowTip);
	BOOL HitMenuBtn(CPoint point);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	void CreateMenu();
	void LoadHentImages();
	int GetImageForHentType(HentType type);
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSysColorChange();
	afx_msg void OnNoHent();
	afx_msg void OnEditHent();
	afx_msg void OnStyleChanged(int styleType, LPSTYLESTRUCT style);
	DECLARE_MESSAGE_MAP()
};

#endif 

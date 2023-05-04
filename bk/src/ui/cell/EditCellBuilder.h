#ifndef __EDITCELLBUILDER_H__
#define __EDITCELLBUILDER_H__

#include "../../ctrls/FilterEdit/FilterEdit.h"
#include <functional>

typedef std::function<void(const std::wstring&)> TextReceiver;

class CEditCell : public CFilterEdit
{
public:
	int Create(const RECT &rect, CWnd *pParentWnd, UINT nID);
	void SetRegex(const std::wstring& sRegex);
	void SetTextReceiver(TextReceiver& txtRecv);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void    SendText();
private:
	TextReceiver m_txtrecver;
protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	DECLARE_MESSAGE_MAP()
};

class EditCellBuilder
{
public:
	EditCellBuilder();
	EditCellBuilder& regex(const std::wstring& sRegex);
	EditCellBuilder& text(const std::wstring& sText);

	template<typename F>
	EditCellBuilder& receiver(F& f)
	{
		TextReceiver recver = f;
		receiver(recver);
		return *this;
	}

	EditCellBuilder& receiver(TextReceiver& recver);
	void show(CWnd* pParent,const CRect& rect);
private:
	std::wstring m_sRegex;
	std::wstring m_sText;
	TextReceiver m_recver;
};





#endif 

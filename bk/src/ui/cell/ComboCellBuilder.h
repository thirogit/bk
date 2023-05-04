#ifndef __COMBOCELLBUILDER_H__
#define __COMBOCELLBUILDER_H__

#include "../../ctrls/FilterEdit/FilterEdit.h"
#include <functional>
#include "../../ctrls/BaseCombo.h"

class ComboCellBuilder
{
public:
	typedef std::function<void (uint32_t)> SelectReceiver;
private:
	class CComboCell : public CBaseCombo
	{
	public:		
		int Create(const RECT &rect,CWnd *pParentWnd,UINT nID);
		void SetSelectReceiver(SelectReceiver& selRecvr);
	protected:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void    SendItem();		
	private:
		SelectReceiver m_selrecver;
		BOOL m_bESC;
	protected:
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnNcDestroy();
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg UINT OnGetDlgCode();
		afx_msg void OnCloseup();
		DECLARE_MESSAGE_MAP()
	};

public:
	ComboCellBuilder();
	
	ComboCellBuilder& add(uint32_t itemId,const std::wstring& sItemText);
	ComboCellBuilder& content(const ComboContent& comboContent);
	ComboCellBuilder& select(uint32_t itemId);

	template<typename F>
	ComboCellBuilder& receiver(F& f)
	{
		SelectReceiver recver = f;
		receiver(recver);
		return *this;
	}

	ComboCellBuilder& receiver(SelectReceiver& recver);
	void show(CWnd* pParent,const CRect& rect);
private:
	uint32_t m_selectedItemId;
	ComboContent m_content;
	SelectReceiver m_recver;
};





#endif 

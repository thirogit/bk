#ifndef __VATCOMBO_H__
#define __VATCOMBO_H__

#include "../enteredit/EnterEdit.h"
#include <types\NullDecimal.h>

#define IDC_COMBOENTEREDIT 1001

class CVATCombo : public CComboBox
{
protected:
	class CComboEnterEdit : public CEnterEdit
	{
	public:
		CComboEnterEdit(const TCHAR *pszRegEx);
	protected:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

		DECLARE_MESSAGE_MAP()
	};
public:
	CVATCombo();
	virtual void PreSubclassWindow();	
	NullDecimal GetVATRate();
	void SetVATRate(const NullDecimal& VAT);
	bool IsUncommited();
protected:
	void Init();
	CComboEnterEdit m_enteredit;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStuct);
	afx_msg BOOL OnSelChange();
	afx_msg void OnEditComplete(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP();
};




#endif 

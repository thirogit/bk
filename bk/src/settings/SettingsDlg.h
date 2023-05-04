#pragma once

#include "../SessionAwareWnd.h"
#include "../ui/treepropsheet/TreePropSheet.h"
#include "deductions/DeductionDefinitionsPage.h"
#include "defaultvalues/DefaultValuesPage.h"
#include "behaviour/BehaviourPage.h"

using namespace TreePropSheet;


class CSettingsDlg : public CSessionAwareWnd<CTreePropSheet>
{
	DECLARE_DYNAMIC(CSettingsDlg)

private:
	/**
	 * Adds all pages to this Settings-Dialog.
	 */
	void AddPropPages();
	/**
	 * Removes the pages and frees up memory.
	 */
	void RemovePropPages();

private:
	
	DeductionDefinitionsPage*	m_pDeductionDefinitionsPage;
	DefaultValuesPage*			m_pDefaultValuesPage;
	BehaviourPage*				m_pBehaviourPage;
	HICON						m_hIcon;

public:
	CSettingsDlg(CWnd* pParentWnd = NULL);
	virtual ~CSettingsDlg();

	
	
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
};



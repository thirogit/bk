#pragma once
#include "../../resource.h"
#include "../SettingsPropPage.h"
#include "DeductionDefsContent.h"
#include <boost/uuid/uuid_generators.hpp>
#include <data\runtime\ItemIndex.h>

class DeductionDefinitionsPage : public SettingsPropPage
{
	typedef ItemIndex<CRUDDeductionDefinition> DefinitionsIndex;

	DECLARE_DYNAMIC(DeductionDefinitionsPage)

public:
	DeductionDefinitionsPage(); 
	virtual ~DeductionDefinitionsPage();

	enum { IDD = IDD_DEDUCTIONDEFINITIONS };

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void DoDataExchange(CDataExchange* pDX);   
	
	afx_msg void OnRemove();
	afx_msg void OnEdit();
	afx_msg void OnAdd();
	afx_msg void OnCopy();

	void SaveDeductions();
	void ReloadDeductionsList();

	afx_msg void OnLvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	
	DeductionDefsContent m_ListContent;
	DefinitionsIndex m_definitions;
	CContentListCtrl m_List;
	boost::uuids::basic_random_generator<boost::mt19937> m_IdGenerator;

	DECLARE_MESSAGE_MAP()
};

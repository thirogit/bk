#ifndef __FINDHENTDLG_H__
#define __FINDHENTDLG_H__

#include "FindDlg.h"
#include "../../resource.h"
#include "../../ctrls/HentTypeCombo.h"
#include "../../ctrls/FilterEdit/FilterEdit.h"
#include "../../ctrls/pngbutton/PngButton.h"


class FindHentDlg : public CDialog
{
	DECLARE_DYNCREATE(FindHentDlg)
public:
	FindHentDlg();
	enum { IDD = IDD_FINDHENT };	
	virtual void Create(FindContainerDlg* container);
protected:
	FindContainerDlg* m_container;

	std::wstring		m_name;
	std::wstring		m_alias;	
	std::wstring		m_pobox;
	std::wstring		m_city;
	std::wstring		m_street;
	std::wstring		m_zip;
	CFilterEdit			m_personalNo;
	CFilterEdit			m_fiscalNo;
	CFilterEdit			m_hentNo;
	CHentTypeCombo		m_henttype;

	CPngButton			m_clearHentType;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog() ;
	void OnOK();
	void OnCancel();
	void UpdateFoundCount(int count);
	
protected:
	
	afx_msg void OnClearHentType();
	LRESULT OnFind(WPARAM wParam, LPARAM lParam);
		
	DECLARE_MESSAGE_MAP()
};

#endif
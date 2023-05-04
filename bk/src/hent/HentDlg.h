#ifndef __HENTDLG_H__
#define __HENTDLG_H__

#include <afxwin.h>
#include "../resource.h"

#include "../ctrls/maskededit/OXMaskedEdit.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/HentTypeCombo.h"
#include "../ctrls/FilterEdit/FilterEdit.h"
#include "../ctrls/AccountNoEdit.h"
#include "../ctrls/dateedit/DateEdit.h"
#include "../ctrls/HentNoEdit.h"
#include "../ctrls/LongitudeEdit.h"
#include "../ctrls/LatitudeEdit.h"
#include <data\impl\Hent.h>
#include "HentEditor.h"
#include "../SessionAwareDialog.h"
#include "../barcodescanner/ui/ScannerStatusBtn.h"

class HentDlg : public CSessionAwareDialog
{
public:
	HentDlg(HentEditor* pEditor,CWnd* pParent);
	enum { IDD = IDD_HENT };
protected:
	CHentNoEdit		m_hentno;
	COXMaskedEdit	m_FiscalNo;
	COXMaskedEdit	m_WetNo;
	COXMaskedEdit	m_ZipCode;
	CExtrasMemo		m_extras;
	std::wstring		m_plate;
	std::wstring		m_name;
	std::wstring		m_alias;
	std::wstring		m_phone;
	std::wstring		m_pobox;
	std::wstring		m_city;
	std::wstring		m_street;
	CHentTypeCombo	m_henttype;
	COXMaskedEdit	m_pesel;
	CFilterEdit		m_regon;
	CFilterEdit		m_idno;
	CDateEdit		m_issuedate;
	std::wstring		m_issuepost;
	CAccountNoEdit	m_accountno;
	std::wstring		m_bankname;	
	std::wstring		m_emailaddress;
	std::wstring		m_cellphoneno;
	std::wstring		m_wetlicenceno;
	CLongitudeEdit	m_Longitude;
	CLatitudeEdit	m_Latitude;
	

	HentEditor*     m_pEditor;	

	CScannerStatusBtn m_ScanHentNoBtn;

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    
	virtual BOOL	OnInitDialog() ;
	virtual void	OnOK();
	virtual void	ConfigureDialogControls();
	virtual void	InitDialogControls();
			bool	ValidateFields();

	 std::wstring	GetOkCaption();
	 std::wstring   GetWindowCaption();
			bool	OkHent();
			void	SaveHent();
			void    UpdateScannerStatusBtn();
protected:
	
	afx_msg void OnAliasFocus();
	afx_msg void OnAliasKillFocus();
	afx_msg void OnDestroy();
	afx_msg void OnScanHentNo();
	DECLARE_MESSAGE_MAP()
};

#endif
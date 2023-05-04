#ifndef __PASSPORTRECOGNITIONDLG_H_
#define __PASSPORTRECOGNITIONDLG_H_

#include "../../resource.h"
#include "../PassportRecognizer.h"
#include "twain/TwainPP.h"
#include "twain/TraceFile.h"
#include "../settings/PassportRecognizerSettings.h"
#include "../../ctrls/arrowbutton/ArrowButton.h"
#include "../../SessionAwareDialog.h"
#include "../session/PassportRecognitionSession.h"
#include "../engine/RecognitionEngine.h"
#include "../../ui/lazydlg/LazyDlg.h"
#include "../../ctrls/contentlistctrl/ContentListCtrl.h"
#include "ctrls\PassImageCtrl.h"
#include "InputField.h"
#include "../../ctrls/BaseCombo.h"
#include "ctrls/XEdit.h"
#include "../PassportImage.h"
#include "../../ctrls/tipwnd/TimerTipWnd.h"
#include "../../ctrls/cowsexcombo/CowSexCombo.h"
#include "content/RecognitionSessionContent.h"

class PassportRecognitionDlg : public CLazyDlg<CSessionAwareDialog>, public CTwainPP::ITwainResult
{
public:
	PassportRecognitionDlg(PassportRecognitionSession* pSession, PassportRecognizerSettings* pSettings,	CWnd* pParent = NULL);
	~PassportRecognitionDlg();

	enum { IDD = IDD_PASSPORTRECOGNITION };
protected:
	PassportRecognizer *m_pPassRecognizer;
	RecognitionEngine* m_pRecognitionEngine;
	CArrowButton  m_ProfileBtn;

	CInputFieldCtrl<CXEdit> m_passDate;
	CInputFieldCtrl<CXEdit> m_passNo;
	CInputFieldCtrl<CXEdit> m_hentNo;
	CInputFieldCtrl<CXEdit> m_cowNo;
	CInputFieldCtrl<CXEdit> m_birthDate;
	CInputFieldCtrl<CXEdit> m_birthPlace;
	CInputFieldCtrl<CXEdit> m_motherNo;
	CInputFieldCtrl<CCowSexCombo> m_cowSexCombo;
	CInputFieldCtrl<CXEdit> m_hentName;
	CInputFieldCtrl<CXEdit> m_hentStreet;
	CInputFieldCtrl<CXEdit> m_hentPobox;
	CInputFieldCtrl<CXEdit> m_hentCity;
	CInputFieldCtrl<CXEdit> m_hentZip;
	CInputFieldCtrl<CComboBox> m_stockCombo;

	CListBox m_passProfiles;
	CTabCtrl m_showTab;
	CMenu m_ProfileMenu;
	CTimerTipWnd m_alert;
	
	int				m_iSelectedTab;
	CContentListCtrl m_cowsList;
	CPassImageCtrl  m_passportImageCtrl;
	
	CTwainPP		m_TwainPP;
	CTraceFile		m_TraceFile;
	BOOL            m_bNoSource;
	PassportRecognizerSettings* m_pSettings;
	NewPtrFlushArray<CWnd> m_hentSection;
	NewPtrFlushArray<CInputField> m_allInputFields;
	fipWinImageEx	m_passportImg;
	PassportImagePtr m_ptrImage;
	uint32_t m_foundHentId;
	uint32_t m_foundStockId;
	PassportRecognitionSession* m_pSession;
	RecognitionSessionContent* m_content;

	void ProcessCowNo(const std::wstring& sCowNoText, const std::wstring& sCowNoBC);
	void ProcessFirstOwner(const std::wstring& sFirstOwnerFarmNo, const std::wstring& sFirstOwnerNameAndAddress);
	void ProcessMotherNo(const std::wstring& sMotherNo);
	void ProcessCowSex(const std::wstring& sCowSex);
	void ProcessPassDate(const std::wstring& sPassDate);
	void ProcessPassNo(const std::wstring& sPassNo);
	void ProcessCowStock(const std::wstring& sCowStock);
	void ProcessBirthDatePlace(const std::wstring& sBirthDatePlace);

	void ClearInvalid();
	void ClearFirstOwnerFields(bool bClearFarmNo);
	void ClearFields();
	void EnableControls(bool bEnable);
	void EnableHentSection(bool bEnable);
	void EnableRepeatButton(bool bEnable);
	void EnableAddCowButton(bool bEnable);	
	uint32_t GetSelectedProfile();
	void SetPassportImage(PassportImagePtr ptrImage, IPassportRecognitionProfile* pPassProfile);

	void ProcessRecognitionResult(const PassportRecognitionResult& recognitionResult);
	void SavePassportImg(Cow *pCow);

	void SetHentFields(IHent* pHent);
	bool ValidatePassNo(const std::wstring &passNo);
	bool ValidateFields();
	CowDetails CreateCowFromFields();
	DateTime GetDate(const std::wstring& sDt);
	bool ValidateDate(const std::wstring& sDt);
	bool ValidateCowNo(const std::wstring& sCowNo);
	bool ValidateHentNo(const std::wstring& sHentNo);	
	void ShowAlert(const std::wstring& message, CWnd* pWnd);
	IHent* FindHentByHentNo(const HentNo& hentNo);
	IStock* FindStockByCode(const std::wstring& stockCd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	virtual void OnTwainImageInfo(int nSizeX, int nSizeY);
	virtual BOOL OnTwainBitmap(HDIB hDIB);
	virtual void OnTwainError(CTwainPP::ETWainError eTwainError);
	virtual void OnAfterDlgShow();
protected:

	virtual BOOL OnInitDialog();
	void OnOK();
	void InitProfilesList();
	void InitStockSelection();
	void AddStockToSelection(const std::wstring& stockCode);
	std::wstring GetSelectedStock();

	void SelectStock(const std::wstring& stockCode);
	void DoOCRWithProfile(uint32_t profileId);
	bool DoManualInputAdd();
	bool DoManualInputUpdate();
	void DoFirstOCR();

	afx_msg void OnProfileMenuBtn();
	afx_msg void OnDuplicateProfile();
	afx_msg void OnDeleteProfile();
	afx_msg void OnEditProfile();
	afx_msg void OnAddNewProfile();
	afx_msg void OnDoScan();
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedScanningFinished();
	afx_msg void OnRepeatWithPrevProfile();
	afx_msg void OnRepeatWithNextProfile();
	afx_msg void OnOCRAgain();
	afx_msg void OnBnClickedAddHent();
	afx_msg void OnClose();
	afx_msg void OnStockCbnChange();
	afx_msg void OnAddCow();
	afx_msg void OnUpdateCow();

	afx_msg void OnPaint();	
	
//	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);	
	
	DECLARE_MESSAGE_MAP()
};
#endif
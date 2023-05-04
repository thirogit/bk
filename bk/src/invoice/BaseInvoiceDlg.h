#ifndef __BASEINVOICEDLG_H__
#define __BASEINVOICEDLG_H__

#include "../resource.h"
#include "../ctrls/ExtrasMemo.h"
#include "../ctrls/vatratecombo/VATCombo.h"
#include "../ctrls/dateedit/dateedit.h"
#include "../ctrls/paywaycombo/PayWayCombo.h"
#include "../ctrls/checkablegroup/checkablegroupbox.h"
#include "../SessionAwareDialog.h"
#include "../ctrls/xcolorstatic/XColorStatic.h"
#include "../ctrls/ownrdrwlistctrl/OwnrDrwListCtrl.h"
#include "../ctrls/decimaledit/DecimalEdit.h"
#include "calculation\InvoiceCalculation.h"
#include "view\CalculationViewCreator.h"
#include "content\InvoiceCalculationContent.h"
#include "content\InvoiceDeductionsContent.h"
#include "../ctrls/hentbtn/HentBtn.h"
#include "../ctrls/invoicetypecombo/InvoiceTypeCombo.h"
#include "../ctrls/clickablegroup/ClickableGroupBox.h"
#include "calculation\EmptyCalculationObserver.h"
#include "../ctrls/pngbutton/PngButton.h"
#include "../ctrls/arrowbutton/ArrowButton.h"


class CBaseInvoiceDlg : public CSessionAwareDialog
{
	class PriceEdit : public CDecimalEdit
	{
	public:
		PriceEdit();
	};


	class CDeductionListCtrl : public CContentListCtrl
	{
	public:
		CDeductionListCtrl();
		virtual void PostCreate();
	
		CImageList m_il;
		InvoiceCalculation* m_pCalculation;

		afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
		DECLARE_MESSAGE_MAP();		
	};

	class DlgCalculationObserver : public EmptyCalculationObserver
	{
	public:
		DlgCalculationObserver(CBaseInvoiceDlg* parent);
		virtual void DeductionChanged(const DeductionElement* pDeduction);
		virtual void DeductionDeleted(ItemId id);
		virtual void DeductionAdded(const DeductionElement* pDeduction);

		CBaseInvoiceDlg* m_parent;
	};

	friend class DlgCalculationObserver;

public:
	CBaseInvoiceDlg(CWnd* pParent = NULL);   
	virtual ~CBaseInvoiceDlg();
	enum { IDD = IDD_BIGINVOICE };
protected:	
	CPaywayCombo m_payway;	
	CVATCombo m_vat;	
	CDateEdit m_invoicedate;	
	CExtrasMemo m_extras;		
	CWrapperEnterEdit<PriceEdit> m_totalnetto;
	CCheckableGroupBox m_settotalnetto;
	BOOL m_bSetTotalNetto;
	CWrapperEnterEdit<PriceEdit> m_totalbrutto;
	CCheckableGroupBox m_settotalbrutto;
	BOOL m_bSetTotalBrutto;
	PriceEdit m_grossAfterDeductions;
	std::wstring m_customNumber;
	CHentBtn	  m_hentbtn;

	//CowPriceInvoiceSession* m_pInvoiceSession;
	//InvoiceUISettings*		m_pUISettings;
	//CStockPricePerKgList m_stockpricelist;
	//CStockStatList m_statList;
	
	CTabCtrl m_naviTab;
	int m_prevSelectedTab;
	CSpinButtonCtrl m_spnPayDueDays;

	//CLastInvoicesTip m_lastInvTip;
	
	COwnrDrwListCtrl m_itemlist;
	COwnrDrwListCtrl m_doclist;	
	CDeductionListCtrl m_deductionslist;
	
	//CMenu m_priceMenu;

	std::wstring m_sTransactionPlace;
	CDateEdit m_TransactionDate;
		
	BOOL m_bCustomPricePerKg;	
	CWrapperEnterEdit<PriceEdit> m_CustomPricePerKg;
	BOOL m_useMyPerKgPrice;
	CXColorStatic m_useMyPerKgPriceClrLbl;
	CXColorStatic m_useCustomPricePerKgClrLbl;			
	CWrapperEnterEdit<PriceEdit> m_totalweight;
	CCheckableGroupBox m_settotalweight;
	BOOL m_bSetTotalWeight;

	CClickableGroupBox m_hentgroup;

	InvoiceCalculation* m_pCalculation;
	CalculationElementViewFactory m_viewCreatorFactory;
	InvoiceCalculationContent* m_pContent;
	InvoiceHent m_hent;
	InvoiceDeductionsContent m_DeductionsContent;
	DlgCalculationObserver m_calculationObserver;
		
	CPngButton m_enableAllDeductionsBtn;
	CPngButton m_disableAllDeductionsBtn;
	CArrowButton m_addFromDefinitionBtn;


protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual BOOL OnInitDialog();
	void CreateTabs();
	void SetCurTab(int tabIndex, bool bForce = false);
	CRect GetTabRect();
	void AddNaviTab(CWnd* tabWnd,const std::wstring& sTabText);

	void UpdateTotals();
	void PutTotalNetto();
	void PutTotalBrutto();
	void PutTotalVAT();
	void PutTotalWeight();
	void OnCellDblClick(int row,uint32_t colId,CellDescriptor& cell);

	void OnElementWeight(ItemId elementId,const NullDecimal& weight);
	void OnElementPricePerKg(ItemId elementId, const NullDecimal& pricePerKg);
	void OnElementNetPrice(ItemId elementId,const NullDecimal& netPrice);
	void OnElementGrossPrice(ItemId elementId,const NullDecimal& grossPrice);
	void OnElementClass(ItemId elementId,uint32_t classId);
	void OnElementRClick(int row,uint32_t colId,const CellDescriptor& cell);


	void DeleteCustomPrice(ItemId id);
	void DeleteCustomClass(ItemId id);
	void DeleteCustomWeight(ItemId id);
	void DeleteCustomPricePerKg(ItemId id);
	void DeleteCustomValues(ItemId id);
	void AddDeductionFromDefinition(uint32_t definitionId);
	void EnableAllDeductions(bool bEnable);
	typedef std::mem_fun1_t<void,CBaseInvoiceDlg,ItemId> DeleteAction;

	void DeleteOnRow(int row,DeleteAction& action);
	void DeleteOnAllRows(DeleteAction& action);

	void OnDeductionDblClick(int row);
		
	virtual void SetupControls();	
	virtual void InitControls();
	void SetDetails(const InvoiceDetails& details);
	InvoiceDetails GetDetails();
	const InvoiceHent& GetInvoiceHent();
	void UpdateItemCount();
	bool Validate();

	virtual void AddItem();
	virtual void DelItem();
	void OnOK();

	virtual void SaveInvoice() = 0;
	virtual std::wstring GetCaption();

	/*void UpdateCowPriceList();	

	
	virtual void TabChanged(int newTabIndex);
	
	int GetSelectedPriceItem();
		
	int GetCurrentTabIndex();

	void SetTotalNetto(const Money& totalnetto);
	void SetTotalBrutto(const Money& totalbrutto);
		
	void DeleteCustomPrice(CowPriceData *pCowPrice);
	void DeleteCustomStock(CowPriceData *pCowPrice);
	void DeleteCustomWeight(CowPriceData *pCowPrice);
	void DeleteForOneRow(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*));
	void DeleteForAllRows(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*));
	void OnTabChanged(int oldTabIndex,int newTabIndex);*/
		
	//virtual void OnCowPriceSelected(CowPriceData* pCowPrice);
	//void SetDlgItemResText(UINT nID,UINT resID);
	//void SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable);
	
	afx_msg void OnCheckTotalWeight();
	afx_msg void OnCheckTotalBrutto();
	afx_msg void OnCheckTotalNetto();
	//afx_msg void OnEndLabelEditCowPrice (NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnEndLabelEditStockPrice (NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnEditTotalNetComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTotalGrossComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditTotalWeightComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditCustomPricePerKgComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditVatRateComplete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomPricePerKg();
	afx_msg void OnVATChange();
	//afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void MenuHandler(UINT nId);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUseMyPerKgPrice();
	afx_msg void OnAddItem();
	afx_msg void OnDelItem();
	afx_msg void OnSetTermWeight();
	afx_msg void OnSetTermStock();
	afx_msg void OnSetTermPrice();
	afx_msg void OnSetTermWeightAll();
	afx_msg void OnSetTermStockAll();
	afx_msg void OnSetTermPriceAll();
	//afx_msg void OnCowPriceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnDocListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);	
	//afx_msg void OnCustomNumberFocus();
	//afx_msg void OnCustomNumberLostFocus();
	afx_msg void OnSetTransactionPlace();
	afx_msg void OnSetTransactionDate();
	afx_msg void OnSetInvoiceDate();
	afx_msg void OnDestroy();
	afx_msg void OnHent();
	afx_msg void OnInvoiceHent();
	afx_msg void OnUpdateInvoiceHent();
	afx_msg void OnHentChange(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnAddDeduction();
	afx_msg void OnDeleteDeduction();
	afx_msg void OnAddDeductionFromDefinition();

	afx_msg void OnEnableAllDeductions();
	afx_msg void OnDisableAllDeductions();
	afx_msg void OnEditHent(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()	
};

#endif
#include "stdafx.h"
#include "BaseInvoiceDlg.h"
#include <data/datalimits.h>
#include "../utils/StringLoader.h"
#include "../uiconstants/ColorConstants.h"
#include "../uiconstants/PrecisionContstants.h"
#include <string/IntString.h>
#include "../ui/cell/DecimalCellBuilder.h"
#include "../ui/cell/EditCellBuilder.h"
#include "../ui/cell/ComboCellBuilder.h"
#include <string\TextUtils.h>
#include "../ui/ExceptionMsgBox.h"
#include "../hent/HentSearchDlg.h"
#include "../content/ClassesComboContent.h"
#include "InvoiceHentUpdateDlg.h"
#include "../ui/menu/MenuBuilder.h"
#include "view\DeductionElementView.h"
#include "ui/deduction/NewDeductionDetailsDlg.h"
#include "ui/deduction/EditDeductionDetailsDlg.h"
#include "../ContextAccess.h"
#include <boost/foreach.hpp>
#include "../settings/DefaultValuesEditor.h"
#include "../hent/HentEditor.h"
#include "../hent/HentDlg.h"

CBaseInvoiceDlg::PriceEdit::PriceEdit() : CDecimalEdit(PRICE_PREC) 
{
}


CBaseInvoiceDlg::DlgCalculationObserver::DlgCalculationObserver(CBaseInvoiceDlg* parent) : m_parent(parent)
{

}

void CBaseInvoiceDlg::DlgCalculationObserver::DeductionChanged(const DeductionElement* pDeduction)
{
	m_parent->PutTotalBrutto();
}

void CBaseInvoiceDlg::DlgCalculationObserver::DeductionDeleted(ItemId id)
{
	m_parent->PutTotalBrutto();
}

void CBaseInvoiceDlg::DlgCalculationObserver::DeductionAdded(const DeductionElement* pDeduction)
{
	m_parent->PutTotalBrutto();
}

	

BEGIN_MESSAGE_MAP(CBaseInvoiceDlg::CDeductionListCtrl, CContentListCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
END_MESSAGE_MAP()

CBaseInvoiceDlg::CDeductionListCtrl::CDeductionListCtrl() : m_pCalculation(NULL)
{
	ListHeaderDescriptor header;
	header(DeductionElementViewMember::Code, StringLoader(IDS_HEADER_DEDUCTIONCODE))
		(DeductionElementViewMember::Fraction, StringLoader(IDS_HEADER_DEDUCTIONFRACTION))
		(DeductionElementViewMember::Reason, StringLoader(IDS_HEADER_DEDUCTIONREASON))
		(DeductionElementViewMember::DeductedAmount, StringLoader(IDS_HEADER_DEDUCTEDAMOUNT))
		;

	SetHeaderDescriptor(header);
		
}

void CBaseInvoiceDlg::CDeductionListCtrl::PostCreate()
{
	CContentListCtrl::PostCreate();

	m_il.Create(16, 16, ILC_COLORDDB | ILC_MASK, 2, 1);
	CBitmap checkBoxes;
	checkBoxes.LoadBitmap(IDB_CHECKBOX);
	m_il.Add(&checkBoxes, RGB(255, 0, 255));
	SetImageList(&m_il, LVSIL_SMALL);
}


void CBaseInvoiceDlg::CDeductionListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = pNMIA->iItem, nSubItem = pNMIA->iSubItem;

	if (GetColId(nSubItem) == DeductionElementViewMember::Code)
	{
		if (pNMIA->ptAction.x < 16 && m_pContent && m_pCalculation)
		{
			DeductionElementRow* row = ((InvoiceDeductionsContent*)m_pContent)->GetDeductionRowAt(nItem);
			DeductionElementView* view = row->Content();
			m_pCalculation->EnableDeduction(view->GetItemId(), !view->GetIsEnabled());
		}
	}
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBaseInvoiceDlg, CSessionAwareDialog)
	
	ON_BN_CLICKED(IDC_SETTOTALNETTO, OnCheckTotalNetto)
	ON_BN_CLICKED(IDC_SETTOTALBRUTTO, OnCheckTotalBrutto)
	ON_BN_CLICKED(IDC_SETTOTALWEIGHT, OnCheckTotalWeight)

	//ON_NOTIFY(LVN_ENDLABELEDIT,IDC_COWLIST, OnEndLabelEditCowPrice)
	//ON_NOTIFY(LVN_ENDLABELEDIT,IDC_STOCKLIST, OnEndLabelEditStockPrice)

	ON_BN_CLICKED(IDC_CHECKPRICEPERKG, OnCustomPricePerKg)

	ON_CBN_SELCHANGE(IDC_VAT, OnVATChange)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_TOTALNETTO, OnEditTotalNetComplete)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_TOTALBRUTTO, OnEditTotalGrossComplete)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_VAT, OnEditVatRateComplete)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_CUSTOMPRICEPERKG, OnEditCustomPricePerKgComplete)
	ON_NOTIFY(EN_EDITCOMPLETE, IDC_TOTALWEIGHT, OnEditTotalWeightComplete)

	//	ON_NOTIFY(NM_RCLICK, IDC_COWLIST, OnRClick)
	//	ON_COMMAND_RANGE(IDM_PRICELIST_MIN,IDM_PRICELIST_MAX,MenuHandler)

	ON_NOTIFY(TCN_SELCHANGE, IDC_NAVTAB, OnTabSelChange)
	ON_NOTIFY(TCN_SELCHANGING, IDC_NAVTAB, OnTabSelChanging)
	ON_BN_CLICKED(IDC_USEMYPERKGPRICE, OnUseMyPerKgPrice)
	ON_BN_CLICKED(IDC_ADDITEM, OnAddItem)
	ON_BN_CLICKED(IDC_DELITEM, OnDelItem)

	ON_BN_CLICKED(IDC_SETTERMWEIGHTBTN, OnSetTermWeight)
	ON_BN_CLICKED(IDC_SETTERMSTOCKBTN, OnSetTermStock)
	ON_BN_CLICKED(IDC_SETTERMPRICEBTN, OnSetTermPrice)

	ON_BN_CLICKED(IDC_SETTERMWEIGHTALL, OnSetTermWeightAll)
	ON_BN_CLICKED(IDC_SETTERMSTOCKALL, OnSetTermStockAll)

	ON_BN_CLICKED(IDC_SETTRANSACTIONPLACE, OnSetTransactionPlace)
	ON_BN_CLICKED(IDC_SETTRANSACTIONDATE, OnSetTransactionDate)
	ON_BN_CLICKED(IDC_SETINVOICEDATE, OnSetInvoiceDate)

	//ON_NOTIFY(LVN_ITEMCHANGED,IDC_COWLIST,OnCowPriceListItemChanged)
	//ON_NOTIFY(LVN_ITEMCHANGED,IDC_DOCLIST,OnDocListItemChanged)

	//ON_EN_SETFOCUS(IDC_CUSTOMNUMBER,OnCustomNumberFocus)
	//ON_EN_KILLFOCUS(IDC_CUSTOMNUMBER,OnCustomNumberLostFocus)

	ON_BN_CLICKED(IDC_HENTBTN, OnHent)
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_HENTGROUP, OnUpdateInvoiceHent)
	ON_NOTIFY(BN_HENTCHANGE, IDC_HENTBTN, OnHentChange)

	ON_BN_CLICKED(IDC_ADDDEDUCTIONBTN, OnAddDeduction)
	ON_BN_CLICKED(IDC_DELDEDUCTIONBTN, OnDeleteDeduction)
	ON_BN_CLICKED(IDC_ADDFROMDEFINITION, OnAddDeductionFromDefinition)
	ON_BN_CLICKED(IDC_ENABLEALLDEDUCTIONSBTN, OnEnableAllDeductions)
	ON_BN_CLICKED(IDC_DISABLEALLDEDUCTIONSBTN, OnDisableAllDeductions)
	ON_NOTIFY(BN_EDITHENT, IDC_HENTBTN, OnEditHent)

END_MESSAGE_MAP()


CBaseInvoiceDlg::CBaseInvoiceDlg(CWnd* pParent /*=NULL*/) : CSessionAwareDialog(CBaseInvoiceDlg::IDD, pParent),
	m_useMyPerKgPrice(FALSE),	
	m_bCustomPricePerKg(FALSE),
	m_extras(MAXEXTRAS),
	m_pCalculation(NULL),
	m_pContent(NULL),
	m_calculationObserver(this)
	//m_stockpricelist(pInvoiceSession),	
	//m_statList(pInvoiceSession),	
{
	m_bSetTotalNetto = FALSE;
	m_bSetTotalBrutto = FALSE;
}


CBaseInvoiceDlg::~CBaseInvoiceDlg()
{
	
}

void CBaseInvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CSessionAwareDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_VAT,m_vat);
	DDX_Control(pDX,IDC_PAYWAYCOMBO,m_payway);	
	DDX_Control(pDX,IDC_EXTRAS,m_extras);
	DDX_Control(pDX,IDC_INVOICEDATE,m_invoicedate);
	DDX_Control(pDX,IDC_TOTALNETTO,m_totalnetto);
	DDX_Control(pDX,IDC_SETTOTALNETTO,m_settotalnetto);
	DDX_Control(pDX,IDC_SETTOTALWEIGHT,m_settotalweight);
	DDX_Control(pDX,IDC_TOTALWEIGHT,m_totalweight);	

	DDX_Control(pDX,IDC_TOTALBRUTTO,m_totalbrutto);
	DDX_Control(pDX,IDC_SETTOTALBRUTTO,m_settotalbrutto);
			
	DDX_StdText(pDX,IDC_CUSTOMNUMBER,m_customNumber);

	DDX_Check(pDX,IDC_CHECKPRICEPERKG,m_bCustomPricePerKg);
	DDX_Control(pDX,IDC_CUSTOMPRICEPERKG,m_CustomPricePerKg);
	DDX_Check(pDX,IDC_SETTOTALNETTO,m_bSetTotalNetto);
	DDX_Check(pDX,IDC_SETTOTALBRUTTO,m_bSetTotalBrutto);
	DDX_Check(pDX,IDC_SETTOTALWEIGHT,m_bSetTotalWeight);
	
	DDX_Control(pDX,IDC_NAVTAB,m_naviTab);
	DDX_Check(pDX,IDC_USEMYPERKGPRICE,m_useMyPerKgPrice);

	DDX_Control(pDX,IDC_PAYDUEDAYSSPIN,m_spnPayDueDays);

	DDX_StdText(pDX,IDC_TRANSACTIONPLACE,m_sTransactionPlace);
	DDX_Control(pDX,IDC_TRANSACTIONDATE,m_TransactionDate);

	DDX_Control(pDX,IDC_USECUSTOMPRICEPERKGCOLORLBL,m_useCustomPricePerKgClrLbl);
	DDX_Control(pDX,IDC_USEMYPRICEPERKGCOLORLBL,m_useMyPerKgPriceClrLbl);
	DDX_Control(pDX,IDC_HENTBTN,m_hentbtn);
	DDX_Control(pDX,IDC_HENTGROUP,m_hentgroup);
	DDX_Control(pDX, IDC_DEDUCTIONSLIST, m_deductionslist);
	DDX_Control(pDX, IDC_TOTALGROSSAFTERDEDUCTIONS, m_grossAfterDeductions);

	DDX_Control(pDX, IDC_ENABLEALLDEDUCTIONSBTN, m_enableAllDeductionsBtn);
	DDX_Control(pDX, IDC_DISABLEALLDEDUCTIONSBTN, m_disableAllDeductionsBtn);
	DDX_Control(pDX, IDC_ADDFROMDEFINITION, m_addFromDefinitionBtn);
}

BOOL CBaseInvoiceDlg::OnInitDialog()
{		
	CSessionAwareDialog::OnInitDialog();
		
	InitControls();
	SetupControls();

	SeasonSession* session = GetSeasonSession();
	

	m_pCalculation->AddObserver(&m_calculationObserver);

	m_pContent = new InvoiceCalculationContent(m_viewCreatorFactory);
	m_pContent->Create(m_pCalculation,session);
	m_itemlist.SetContent(m_pContent);

	m_DeductionsContent.Create(m_pCalculation, session);
	m_deductionslist.SetContent(&m_DeductionsContent);
	m_deductionslist.m_pCalculation = m_pCalculation;

	UpdateItemCount();
	OnCustomPricePerKg();
	PutTotalWeight();
			
	return TRUE;
}

void CBaseInvoiceDlg::SetupControls()
{
}

void CBaseInvoiceDlg::InitControls()
{
	m_settotalnetto.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalnetto.SetCheck(FALSE);

	m_settotalbrutto.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalbrutto.SetCheck(FALSE);

	m_settotalweight.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalweight.SetCheck(FALSE);

	m_enableAllDeductionsBtn.SetImage(IDB_CHECKALL);
	m_disableAllDeductionsBtn.SetImage(IDB_UNCHECKALL);
	m_addFromDefinitionBtn.SetArrowStyle(ABS_ARROW_DOWN);

	//m_vat.SetVATRate(m_pInvoiceSession->GetVATRate());
	//m_payway.SetHent(m_pInvoiceSession->GetInvoiceHent());

	m_payway.ReloadCombo();

	m_totalbrutto.SendMessage(EM_SETREADONLY,TRUE);
	m_totalnetto.SendMessage(EM_SETREADONLY,TRUE);
	m_totalweight.SendMessage(EM_SETREADONLY,TRUE);


	//m_priceMenu.LoadMenu(IDR_PRICELISTMENU);

	SendDlgItemMessage(IDC_CUSTOMNUMBER,EM_LIMITTEXT,MAXCUSTOMNUMBER);

	CreateTabs();	
	
//	SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);
//	SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);
//	SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);

	SetWindowText(GetCaption().c_str());	

	m_spnPayDueDays.SetRange32(0,MAX_PAYDUEDAYS);
	m_spnPayDueDays.SetPos(0);

	
	m_useCustomPricePerKgClrLbl.SetBackgroundColor(CUSTOM_PRICE_PER_KG_CLR);
	m_useMyPerKgPriceClrLbl.SetBackgroundColor(STOCK_PER_KG_PRICE_CLR);

	//m_lastInvTip.SetColors(RGB(255,255,0),RGB(255,255,128));
	//m_lastInvTip.Create(this);

	m_itemlist.SetOnRightClick(std::bind(&CBaseInvoiceDlg::OnElementRClick,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
	m_hentgroup.EnableWindow(FALSE);


	m_deductionslist.SetRowDblClickHandler(std::bind(&CBaseInvoiceDlg::OnDeductionDblClick, this, std::placeholders::_1));
}

void CBaseInvoiceDlg::OnElementRClick(int row,uint32_t colId,const CellDescriptor& cell)
{	
	MenuBuilder menu;

	MenuBuilder delCustPriceMenu;
	delCustPriceMenu.add(StringLoader(IDS_THISROW),std::bind(&CBaseInvoiceDlg::DeleteOnRow,this,row,DeleteAction(&CBaseInvoiceDlg::DeleteCustomPrice)))
		            .add(StringLoader(IDS_ALLROWS),std::bind(&CBaseInvoiceDlg::DeleteOnAllRows,this,DeleteAction(&CBaseInvoiceDlg::DeleteCustomPrice)));

	MenuBuilder delCustClassMenu;
	delCustClassMenu.add(StringLoader(IDS_THISROW),std::bind(&CBaseInvoiceDlg::DeleteOnRow,this,row,DeleteAction(&CBaseInvoiceDlg::DeleteCustomClass)))
		            .add(StringLoader(IDS_ALLROWS),std::bind(&CBaseInvoiceDlg::DeleteOnAllRows,this,DeleteAction(&CBaseInvoiceDlg::DeleteCustomClass)));

	MenuBuilder delCustWeightMenu;
	delCustWeightMenu.add(StringLoader(IDS_THISROW),std::bind(&CBaseInvoiceDlg::DeleteOnRow,this,row,DeleteAction(&CBaseInvoiceDlg::DeleteCustomWeight)))
		             .add(StringLoader(IDS_ALLROWS),std::bind(&CBaseInvoiceDlg::DeleteOnAllRows,this,DeleteAction(&CBaseInvoiceDlg::DeleteCustomWeight)));

	MenuBuilder delCustValuesMenu;
	delCustValuesMenu.add(StringLoader(IDS_THISROW),std::bind(&CBaseInvoiceDlg::DeleteOnRow,this,row,DeleteAction(&CBaseInvoiceDlg::DeleteCustomValues)))
		             .add(StringLoader(IDS_ALLROWS),std::bind(&CBaseInvoiceDlg::DeleteOnAllRows,this,DeleteAction(&CBaseInvoiceDlg::DeleteCustomValues)));

	MenuBuilder delCustPricePerKgMenu;
	delCustPricePerKgMenu.add(StringLoader(IDS_THISROW),std::bind(&CBaseInvoiceDlg::DeleteOnRow,this,row,DeleteAction(&CBaseInvoiceDlg::DeleteCustomPricePerKg)))
						 .add(StringLoader(IDS_ALLROWS),std::bind(&CBaseInvoiceDlg::DeleteOnAllRows,this,DeleteAction(&CBaseInvoiceDlg::DeleteCustomPricePerKg)));

	menu.addSubmenu(StringLoader(IDS_DELCUSTVALUES),delCustValuesMenu)
		.addSubmenu(StringLoader(IDS_DELCUSTPRICE),delCustPriceMenu)
		.addSubmenu(StringLoader(IDS_DELCUSTPRICEPERKG),delCustPricePerKgMenu)
		.addSubmenu(StringLoader(IDS_DELCUSTCLASS),delCustClassMenu)
		.addSubmenu(StringLoader(IDS_DELCUSTWEIGHT),delCustWeightMenu)
		
		;//.addSeparator()
		//.add(StringLoader(IDS_SHOWPASSIMG),);

	
	menu.track(this);


}

void CBaseInvoiceDlg::SetDetails(const InvoiceDetails& details)
{
	m_extras.SetText(details.GetExtras());
	m_invoicedate.SetDate(details.GetInvoiceDate());
	m_vat.SetVATRate(NullDecimal(details.GetVATRate()));
	m_payway.ReloadCombo(details.GetPayWay());	
	m_customNumber = details.GetCustomNumber();
	m_sTransactionPlace = details.GetTransactionPlace();
	m_TransactionDate.SetDate(details.GetTransactionDate());
	m_spnPayDueDays.SetPos(details.GetPayDueDays());
	UpdateData(FALSE);
}


const InvoiceHent& CBaseInvoiceDlg::GetInvoiceHent()
{
	return m_hent;
}

InvoiceDetails CBaseInvoiceDlg::GetDetails()
{
	InvoiceDetails details;
	details.SetExtras(m_extras.GetText());
	details.SetInvoiceDate(m_invoicedate.GetDate());
	details.SetVATRate(m_vat.GetVATRate().ToDecimal(Decimal(0L)));
	details.SetPayWay(m_payway.GetSelectedPayWay());
	details.SetCustomNumber(m_customNumber);
	details.SetTransactionPlace(m_sTransactionPlace);
	details.SetTransactionDate(m_TransactionDate.GetDate());
	details.SetPayDueDays(m_spnPayDueDays.GetPos());
	
	return details;
}

void CBaseInvoiceDlg::CreateTabs()
{
	const int itemHeight = 20;
	const DWORD dwListStyle = LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED | LVS_OWNERDATA | WS_BORDER | WS_TABSTOP | WS_CHILD | LVS_SINGLESEL;
	CRect tabRect = GetTabRect();
								
	m_prevSelectedTab = -1;


	m_itemlist.SetItemHeight(itemHeight);
	m_itemlist.Create(dwListStyle,tabRect,&m_naviTab,IDC_ITEMLIST);

	m_doclist.SetItemHeight(itemHeight);
	m_doclist.Create(dwListStyle,tabRect,&m_naviTab,IDC_DOCLIST);


	std::function<void (int,uint32_t,CellDescriptor&)> handler = [this](int row,uint32_t colId,CellDescriptor& cell)
																 {
																	 OnCellDblClick(row,colId,cell);
																 };
	m_itemlist.SetCellDblClickHandler(handler);
	
	AddNaviTab(&m_itemlist,StringLoader(IDS_INVOICEPRICINGTABTEXT));
	AddNaviTab(&m_doclist,StringLoader(IDS_INVOICEDOCSTABTEXT));
	
//	AddNaviTab(1,m_stockpricelist,tabItemRect,IDC_STOCKLIST,IDS_INVNAVITABSTOCKPRICE,false);

	//AddNaviTab(m_docList,IDC_DOCLIST,IDS_INVNAVITABDOCS,false);

//	AddNaviTab(3,m_statList,tabItemRect,IDC_STOCKSTATLIST,IDS_INVNAVITABSTAT,false);

	SetCurTab(0,true);

}

CRect CBaseInvoiceDlg::GetTabRect()
{
	CRect tabItemRect;
	m_naviTab.GetClientRect(&tabItemRect);
	tabItemRect.top += 28;
	tabItemRect.bottom -= 4;
	tabItemRect.left += 4;
	tabItemRect.right -= 8;
	return tabItemRect;
}

void CBaseInvoiceDlg::SetCurTab(int tabIndex, bool bForce/* = false*/)
{
	if(m_naviTab.GetCurSel() == tabIndex && bForce)
	{
		LRESULT lResult;
		OnTabSelChange(NULL,&lResult);
	}
	else
	{
		m_naviTab.SetCurSel(tabIndex);
	}
}


void CBaseInvoiceDlg::AddNaviTab(CWnd* tabWnd,const std::wstring& sTabText)
{
	CRect rect = GetTabRect();
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;	
	item.lParam = (LPARAM) tabWnd;
	item.pszText = const_cast<wchar_t*>(sTabText.c_str());
	m_naviTab.InsertItem(m_naviTab.GetItemCount(), &item);
	tabWnd->SetWindowPos(NULL, 0,0, 0, 0,SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE);		
}

void CBaseInvoiceDlg::OnCheckTotalWeight()
{
	UpdateData();
	m_totalweight.SendMessage(EM_SETREADONLY,!m_bSetTotalWeight);
	if(!m_bSetTotalWeight)
	{
		m_pCalculation->SetTargetTotalWeight(NullDecimal());
		PutTotalWeight();
	}
}

void CBaseInvoiceDlg::OnCheckTotalNetto()
{
	UpdateData();
	bool not_m_settotal = !m_bSetTotalNetto;

	if(not_m_settotal)
	{
		m_pCalculation->SetTargetNetTotal(NullDecimal());		
		UpdateTotals();
	}
	
	m_totalnetto.SendMessage(EM_SETREADONLY,not_m_settotal);	
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);
	m_settotalbrutto.EnableWindow(not_m_settotal);
}

void CBaseInvoiceDlg::OnCheckTotalBrutto()
{
	UpdateData();
	bool not_m_settotal = !m_bSetTotalBrutto;

	if(not_m_settotal)
	{
		m_pCalculation->SetTargetGrossTotal(NullDecimal());		
		UpdateTotals();
	}
	m_totalbrutto.SendMessage(EM_SETREADONLY,not_m_settotal);

	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);
	m_settotalnetto.EnableWindow(not_m_settotal);

}

void CBaseInvoiceDlg::UpdateTotals()
{	
	PutTotalNetto();
	PutTotalBrutto();
	PutTotalVAT();
}

void CBaseInvoiceDlg::OnEditTotalNetComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bSetTotalNetto)
	{	
		ExceptionMsgBox().TryCatchAny([this]()
									  {
											m_pCalculation->SetTargetNetTotal(m_totalnetto.GetDecimal());											
									  });
		UpdateTotals();
	}	
	*pResult = (LRESULT)0;
}


void CBaseInvoiceDlg::OnEditTotalGrossComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bSetTotalBrutto)
	{
		ExceptionMsgBox().TryCatchAny([this]()
									  {
											m_pCalculation->SetTargetGrossTotal(m_totalbrutto.GetDecimal());											
									  });
		UpdateTotals();
	}
	*pResult = (LRESULT)0;
}

void CBaseInvoiceDlg::OnCustomPricePerKg()
{
	UpdateData();
	if(!m_bCustomPricePerKg) 
	{
		m_pCalculation->SetCustomNetPricePerKgForAll(NullDecimal());		
	}
	else
	{
		m_pCalculation->SetCustomNetPricePerKgForAll(m_CustomPricePerKg.GetDecimal());		
	}

	UpdateTotals();

	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_bCustomPricePerKg);
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(!m_bCustomPricePerKg);
}

void CBaseInvoiceDlg::OnVATChange()
{	
	NullDecimal VAT = m_vat.GetVATRate();
	if(VAT.IsNull())
	{
		m_vat.SetVATRate(NullDecimal(m_pCalculation->GetVATRate()));
	}
	else
	{
		m_pCalculation->SetVATRate(VAT.ToDecimal(Decimal(0L)));		
		UpdateTotals();
	}
}


bool CBaseInvoiceDlg::Validate()
{
	if(m_hentbtn.GetHent() == NULL_ID)
	{
		AfxMessageBox(IDS_ERRMISSINGHENT,MB_OK | MB_ICONERROR,0);
		return false;
	}

	if(m_payway.GetSelectedPayWay() == PayWay_None)
	{
		AfxMessageBox(IDS_ERRNOPAYWAY,MB_OK,0);
		m_payway.SetFocus();
		return false;
	}

	if(m_invoicedate.IsInputEmpty())
	{
		AfxMessageBox(IDS_ERRNOINVDATE,MB_OK,0);
		m_invoicedate.SetFocus();
		return false;
	}
	else
	{
		if(!m_invoicedate.IsDateValid())
		{
			AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
			m_invoicedate.SetFocus();
			return false;
		}
	}
		
	if(!m_TransactionDate.IsInputEmpty())
	{
		if(!m_TransactionDate.IsDateValid())
		{
			AfxMessageBox(IDS_ERRBADDATE,MB_OK,0);
			m_TransactionDate.SetFocus();
			return false;
		}
	}

	/*bool gotError = false;

	for(int i = 0;i< m_pInvoiceSession->GetCowCount();i++)
	{		
		if(m_pInvoiceSession->GetCowPrice(i)->IsError())
		{	
			gotError = true;
		}
	}

	if(gotError)
	{
		AfxMessageBox(IDS_ERRINVALIDINVENTRIES,MB_OK,0);
		return false;
	}*/
	return true;
}

void CBaseInvoiceDlg::OnOK()
{
	UpdateData();
	
	if(!Validate()) return;
		
	if(AfxMessageBox(IDS_PROMPTSAVE,MB_YESNO,0) != IDYES)
		return;
	ExceptionMsgBox().TryCatchAny([this]
	{
		SaveInvoice();
		EndDialog(IDOK);
	});
}
//
//
//void CBaseInvoiceDlg::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
//	if (pNMIA)
//	{
//	
//		if( pNMIA->iItem >= 0)
//		{
//		
//			CMenu* pPopupMenu = m_priceMenu.GetSubMenu(0);
//			ASSERT(pPopupMenu); 
//	
//			if (m_pInvoiceSession->GetCowPrice(pNMIA->iItem)->GetPriceCustomInd() != CUSTOM_DISABLED)
//				pPopupMenu->EnableMenuItem(IDM_PRICELIST_DELCUSTPRICE, MF_BYCOMMAND | MF_ENABLED);
//			else
//                pPopupMenu->EnableMenuItem(IDM_PRICELIST_DELCUSTPRICE, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED); 
//	
//
//			// Get the cursor position
//			CPoint CursorPoint = (0, 0);
//			GetCursorPos(&CursorPoint);
//			// Track the popup menu
//			pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, CursorPoint.x, 
//									CursorPoint.y, this);
//
//		}
//		
//	}
//
//
//	*pResult = 0;
//}
//
//void CBaseInvoiceDlg::DeleteCustomPrice(CowPriceData *pCowPrice)
//{
//	pCowPrice->DisableCustomPrice();
//	UpdateCowPriceList();
//	UpdateTotal();
//}
//void CBaseInvoiceDlg::DeleteCustomStock(CowPriceData *pCowPrice)
//{
//	pCowPrice->SetCustomStock(NULL);
//	UpdateCowPriceList();
//	UpdateTotal();
//}
//void CBaseInvoiceDlg::DeleteCustomWeight(CowPriceData *pCowPrice)
//{
//	pCowPrice->SetCustomWeight(NullDouble());
//	UpdateCowPriceList();
//	UpdateTotal();
//}
//
//void CBaseInvoiceDlg::DeleteForOneRow(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*))
//{
//	CowPriceData *pPrice;
//	POSITION pos;
//	pos = m_pricelist.GetFirstSelectedItemPosition();
//	if(pos)
//	{	
//		pPrice = m_pInvoiceSession->GetCowPrice(m_pricelist.GetNextSelectedItem(pos));
//		(this->*pDeleteAction)(pPrice);	
//	}
//}
//
//void CBaseInvoiceDlg::DeleteForAllRows(void (CBaseInvoiceDlg::*pDeleteAction)(CowPriceData*))
//{
//	CowPriceData *pPrice;
//	for(int i = 0,s = m_pInvoiceSession->GetCowCount();i<s;i++)
//	{
//		pPrice = m_pInvoiceSession->GetCowPrice(i);
//		(this->*pDeleteAction)(pPrice);
//	}	
//}
//
//
//void CBaseInvoiceDlg::MenuHandler(UINT nId)
//{	
//
//	switch(nId)
//	{
//		case IDM_PRICELIST_DELCUSTPRICEALL:
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomPrice);	
//		break;		
//		case IDM_PRICELIST_DELCUSTPRICE:
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomPrice);		
//		break;		
//		case IDM_PRICELIST_DELCUSTSTOCKALL:
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomStock);		
//		break;		
//		case IDM_PRICELIST_DELCUSTSTOCK:
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomStock);		
//		break;
//		case IDM_PRICELIST_DELCUSTWEIGHT:
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomWeight);		
//		break;
//		case IDM_PRICELIST_DELCUSTWEIGHTALL:
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomWeight);		
//		break;	
//		case IDM_PRICELIST_SHOWPASSIMG:
//		{
//			CCowArray cows;
//			for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
//				cows.Add(m_pInvoiceSession->GetCowPrice(i)->GetCow());				
//			int j = m_pricelist.GetCurSel();
//			CPassImgViewDlg passView(cows,j);
//			passView.DoModal();
//
//		break;
//		}
//
//		case IDM_PRICELIST_DELALLCUSTVALUES:
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomWeight);
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomPrice);
//			DeleteForOneRow(&CBaseInvoiceDlg::DeleteCustomStock);
//		break;
//		case IDM_PRICELIST_DELALLCUSTVALUESALL:
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomWeight);
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomPrice);
//			DeleteForAllRows(&CBaseInvoiceDlg::DeleteCustomStock);
//		break;
//
//	}
//	
//}

void CBaseInvoiceDlg::PutTotalNetto()
{
	m_totalnetto.SetDecimal(m_pCalculation->GetNetTotal());
	m_totalnetto.CommitText();	
}

void CBaseInvoiceDlg::PutTotalBrutto()
{
	m_totalbrutto.SetDecimal(m_pCalculation->GetGrossTotal());
	m_totalbrutto.CommitText();
	m_grossAfterDeductions.SetDecimal(m_pCalculation->GetGrossAfterDeductions());
}

void CBaseInvoiceDlg::PutTotalVAT()
{
	SetDlgItemText(IDC_TOTALVAT,m_pCalculation->GetTaxValue().ToString(PRICE_PREC).c_str());
}



void CBaseInvoiceDlg::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
		int iNewTab = m_naviTab.GetCurSel();

		TCITEM item;
		CWnd* pWnd;

		item.mask = TCIF_PARAM;
		
		if(m_prevSelectedTab >= 0)
		{
			//** hide the current tab ---------
			m_naviTab.GetItem(m_prevSelectedTab, &item);
			pWnd = reinterpret_cast<CWnd*> (item.lParam);
			ASSERT_VALID(pWnd);
			pWnd->ShowWindow(SW_HIDE);
		}

		//** show the selected tab --------
		m_naviTab.GetItem(iNewTab, &item);
		pWnd = reinterpret_cast<CWnd*> (item.lParam);
		ASSERT_VALID(pWnd);
		pWnd->ShowWindow(SW_SHOW);

		//OnTabChanged(m_prevSelectedTab,iNewTab);

	*pResult = 0;
}

void CBaseInvoiceDlg::OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_prevSelectedTab = m_naviTab.GetCurSel();

	*pResult = 0;
}
//int CBaseInvoiceDlg::GetCurrentTabIndex()
//{
//	return m_naviTab.GetCurSel();
//}
//
//void CBaseInvoiceDlg::OnTabChanged(int oldTabIndex,int newTabIndex)
//{
//	TabChanged(newTabIndex);
//}

void CBaseInvoiceDlg::PutTotalWeight()
{
	m_totalweight.SetDecimal(m_pCalculation->GetWeightTotal());
	m_totalweight.CommitText();
}


void CBaseInvoiceDlg::OnUseMyPerKgPrice()
{
	/*UpdateData();
	m_pInvoiceSession->SetUseStockMyPerKgPrice(m_useMyPerKgPrice == TRUE);
	UpdateCowPriceList();
	UpdateTotal();	
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_useMyPerKgPrice);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(!m_useMyPerKgPrice);*/
}

void CBaseInvoiceDlg::OnCellDblClick(int row,uint32_t colId,CellDescriptor& cell)
{
	const CalculationElement* element = m_pCalculation->GetElementAt(row);
	switch(colId)
	{		
		case CalculationElementViewMember::CalculationElementView_Class:
			{
				ClassesComboContent content;
				content.CreateContent(GetSeasonSession());
				ComboCellBuilder classCell;
				classCell.content(content)
					     .receiver(std::bind(&CBaseInvoiceDlg::OnElementClass,this,element->GetId(),std::placeholders::_1))
						 .select(element->GetClassId())
						 .show(&m_itemlist,cell.GetRect());

			break;
			}

		case CalculationElementViewMember::CalculationElementView_Weight:
			{				
				DecimalCellBuilder weightCell;		
				weightCell.precision(WEIGHT_PREC)
						  .receiver(std::bind(&CBaseInvoiceDlg::OnElementWeight,this,element->GetId(),std::placeholders::_1))
						  .value(NullDecimal(element->GetWeight()))
						  .show(&m_itemlist,cell.GetRect());
				break;
			}
		case CalculationElementViewMember::CalculationElementView_NetPricePerKg:
			{				
				DecimalCellBuilder pricePerKgCell;
				pricePerKgCell.precision(PRICEPERKG_PREC)
					.receiver(std::bind(&CBaseInvoiceDlg::OnElementPricePerKg,this,element->GetId(),std::placeholders::_1))
					.value(NullDecimal(element->GetNetPricePerKg()))
					.show(&m_itemlist,cell.GetRect());
				break;
			}
		case CalculationElementViewMember::CalculationElementView_NetPrice:
			{				
				DecimalCellBuilder netPriceCell;		
				netPriceCell.precision(WEIGHT_PREC)
					.receiver(std::bind(&CBaseInvoiceDlg::OnElementNetPrice,this,element->GetId(),std::placeholders::_1))
					.value(NullDecimal(element->GetNetPrice()))
					.show(&m_itemlist,cell.GetRect());
				break;
			}
		case CalculationElementViewMember::CalculationElementView_GrossPrice:
			{				
				DecimalCellBuilder grossPriceCell;		
				grossPriceCell.precision(WEIGHT_PREC)
					.receiver(std::bind(&CBaseInvoiceDlg::OnElementGrossPrice,this,element->GetId(),std::placeholders::_1))
					.value(NullDecimal(element->GetGrossPrice()))
					.show(&m_itemlist,cell.GetRect());
				break;
			}
	}
}

void CBaseInvoiceDlg::OnElementClass(ItemId elementId,uint32_t classId)
{
	m_pCalculation->SetItemCustomClass(elementId,classId);
	
}

void CBaseInvoiceDlg::OnElementWeight(ItemId elementId,const NullDecimal& weight)
{
	m_pCalculation->SetItemCustomWeight(elementId,weight);
	PutTotalWeight();	
}

void CBaseInvoiceDlg::OnElementPricePerKg(ItemId elementId,const NullDecimal& pricePerKg)
{
	m_pCalculation->SetItemCustomNetPricePerKg(elementId, pricePerKg);
	UpdateTotals();
}

void CBaseInvoiceDlg::OnElementNetPrice(ItemId elementId,const NullDecimal& netPrice)
{
	m_pCalculation->SetItemCustomNetPrice(elementId,netPrice);
	UpdateTotals();
}

void CBaseInvoiceDlg::OnElementGrossPrice(ItemId elementId,const NullDecimal& grossPrice)
{
	m_pCalculation->SetItemCustomGrossPrice(elementId,grossPrice);
	UpdateTotals();
}

void CBaseInvoiceDlg::OnAddItem()
{
	AddItem();
	UpdateItemCount();
	UpdateTotals();
	PutTotalWeight();
}

void CBaseInvoiceDlg::UpdateItemCount()
{
	SetDlgItemText(IDC_COUNT,IntString(m_pCalculation->GetElementCount()).c_str());	
}

void CBaseInvoiceDlg::OnDelItem()
{	
	DelItem();
	UpdateItemCount();
	UpdateTotals();
	PutTotalWeight();
}

void CBaseInvoiceDlg::OnSetTermWeight()
{
	/*int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomWeight(m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow()).ToInt());
		UpdateCowPriceList();
		UpdateTotal();
	}*/
}
void CBaseInvoiceDlg::OnSetTermStock()
{
	/*int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomStock(m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow()));
		UpdateCowPriceList();
		UpdateTotal();
	}*/
	
}
void CBaseInvoiceDlg::OnSetTermPrice()
{

	/*int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomPriceNetto(m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow()));
		UpdateCowPriceList();
		UpdateTotal();
	}*/
	
}

void CBaseInvoiceDlg::OnEditTotalWeightComplete(NMHDR* pNMHDR, LRESULT* pResult)
{		
	NullDecimal totalWeight = m_totalweight.GetDecimal();

	ExceptionMsgBox().TryCatchAny(  [this,&totalWeight]
									{
										m_pCalculation->SetTargetTotalWeight(totalWeight);											
									});
	PutTotalWeight();
	
	*pResult = (LRESULT)0;
}


//void CBaseInvoiceDlg::OnDocListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	
//	if ((pNMListView->uChanged & LVIF_STATE) &&
//		(pNMListView->uNewState & LVIS_SELECTED)) 
//	{	
//		m_pInvoiceSession->SetSelectedDocItem(pNMListView->iItem);
//	}
//
//	*pResult = (LRESULT)0;
//
//}
//
//void CBaseInvoiceDlg::OnCowPriceListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//
//	CowPriceData *pCowPrice = NULL;
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	
//	if ((pNMListView->uChanged & LVIF_STATE) &&
//		(pNMListView->uNewState & LVIS_SELECTED)) 
//	{
//		pCowPrice = m_pInvoiceSession->GetCowPrice(pNMListView->iItem);
//		
//
//
//	//	Money termPrice = pInvoiceContext->GetTermPrice(pCowPrice->GetCow());
//	//	NullInt termWeight = pInvoiceContext->GetTermWeight(pCowPrice->GetCow());
//	//	Stock* pTermStock = pInvoiceContext->GetTermStock(pCowPrice->GetCow());
//
//		/*if(!termPrice.IsNull())
//			SetTermValue(IDC_TERMPRICE_VALUE,termPrice.ToString(),IDC_SETTERMPRICEBTN,TRUE);
//		else
//			SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);
//
//		if(!termWeight.IsNull())
//			SetTermValue(IDC_TERMWEIGHT_VALUE,termWeight.ToString(),IDC_SETTERMWEIGHTBTN,TRUE);
//		else
//			SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);
//
//
//		if(pTermStock)
//			SetTermValue(IDC_TERMSTOCK_VALUE,(LPCSTR)pTermStock->GetStockCode(),IDC_SETTERMSTOCKBTN,TRUE);
//		else
//			SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);*/
//
//		
//		m_pInvoiceSession->SetSelectedCowPriceItem(pNMListView->iItem);
//		OnCowPriceSelected(pCowPrice);
//
//	}
//
//	*pResult = (LRESULT)0;
//}
//
//void CBaseInvoiceDlg::OnCowPriceSelected(CowPriceData* pCowPrice)
//{
//
//}

//
//void CBaseInvoiceDlg::SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable)
//{
//	SetDlgItemText(nValueLabelID,pszValueText);
//	GetDlgItem(nSetBtnID)->EnableWindow(bEnable);
//}
//
//
void CBaseInvoiceDlg::OnSetTermWeightAll()
{
	/*CowPriceData *pCowPrice = NULL;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{		
		pCowPrice  =  m_pInvoiceSession->GetCowPrice(i);
		pCowPrice->SetCustomWeight(m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow()).ToInt());
	}
	
	UpdateCowPriceList();
	UpdateTotal();*/

}
void CBaseInvoiceDlg::OnSetTermStockAll()
{
	/*CowPriceData *pCowPrice = NULL;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		pCowPrice = m_pInvoiceSession->GetCowPrice(i);
		pCowPrice->SetCustomStock(m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow()));
	}

	UpdateCowPriceList();
	UpdateTotal();*/
}
//
//void CBaseInvoiceDlg::OnSetTermPriceAll()
//{
//	CowPriceData *pCowPrice = NULL;
//	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
//	{
//		pCowPrice  = m_pInvoiceSession->GetCowPrice(i);
//		pCowPrice->SetCustomPriceNetto(m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow()));
//	}
//
//	UpdateCowPriceList();
//	UpdateTotal();	
//}
//



//void CBaseInvoiceDlg::OnCustomNumberFocus()
//{
//	CWnd* customNumberText = GetDlgItem(IDC_CUSTOMNUMBER);
//
//	CRect rect;
//	customNumberText->GetClientRect(&rect);
//	customNumberText->ClientToScreen(&rect);
//	m_lastInvTip.Show(m_pInvoiceSession->GetInvoiceContext()->GetInvoiceType(),CPoint(rect.left,rect.bottom));
//}
//void CBaseInvoiceDlg::OnCustomNumberLostFocus()
//{
//	m_lastInvTip.Hide();
//}

//int CBaseInvoiceDlg::GetSelectedPriceItem()
//{
//	return m_pricelist.GetCurSel();
//}
//
//void CBaseInvoiceDlg::SetInvoiceControls()
//{
//	m_vat.SetVATRate(m_pInvoiceSession->GetVATRate());		
//	m_payway.ReloadCombo(m_pInvoiceSession->GetPayway());	
//	m_extras.SetWindowText(m_pInvoiceSession->GetExtras());
//	m_customNumber = m_pInvoiceSession->GetCustomNumber();
//	m_invoicedate.SetDate(m_pInvoiceSession->GetInvoiceDate());
//	m_spnPayDueDays.SetPos(m_pInvoiceSession->GetPayDueDays());
//	m_TransactionDate.SetDate(m_pInvoiceSession->GetTransactionDate());
//	m_sTransactionPlace = m_pInvoiceSession->GetTransactionPlace();
//}
//
//
 void CBaseInvoiceDlg::AddItem()
 {
	 //m_pInvoiceSession->AddItem();
	 //m_docList.UpdateItemCount();
	 //UpdateItemCount();
	 
 }

 void CBaseInvoiceDlg::DelItem()
 {
//	 /*m_pricelist.Lock();
//	 m_docList.Lock();
//
//	 m_pInvoiceSession->DelItem();
//	 m_docList.UpdateItemCount();
//	 UpdateItemCount();
//	 
//	 m_pricelist.Unlock();
//	 m_docList.Unlock();
//
//	 UpdateTotal();*/
 }


 //void CBaseInvoiceDlg::TabChanged(int newTabIndex)
 //{
	// //m_pDriver->TabChanged(newTabIndex);
 //}

 
 void CBaseInvoiceDlg::OnDestroy()
 {
	 m_deductionslist.SetContent(NULL);
	 m_itemlist.SetContent(NULL);

	 if (m_pContent)
	 {
		 delete m_pContent;
		 m_pContent = NULL;
	 }

	 m_DeductionsContent.Destroy();

	 CSessionAwareDialog::OnDestroy();
 }

void CBaseInvoiceDlg::OnHent()
{
	HentSearchDlg hentSearchDlg(this);
	uint32_t hentId = hentSearchDlg.FindHent();
	if (hentId != NULL_ID)
	{
		m_hentbtn.SetHent(hentId);
	}
}

void CBaseInvoiceDlg::OnEditCustomPricePerKgComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NullDecimal pricePerKg = m_CustomPricePerKg.GetDecimal();
	m_pCalculation->SetCustomNetPricePerKgForAll(pricePerKg);
	UpdateTotals();	
	*pResult = (LRESULT)0;
}

void CBaseInvoiceDlg::OnEditVatRateComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	NullDecimal vatRate = m_vat.GetVATRate();
	m_pCalculation->SetVATRate(vatRate.ToDecimal(Decimal(0L)));
	UpdateTotals();
	*pResult = (LRESULT)0;
}

void CBaseInvoiceDlg::OnSetTransactionPlace()
{
	if(m_hent.GetParentHentId() != NULL_ID)
	{
		SetDlgItemText(IDC_TRANSACTIONPLACE,m_hent.GetCity().c_str());					
	}
}

void CBaseInvoiceDlg::OnSetTransactionDate()
{		
	m_TransactionDate.SetDate(DateTime::now());
}

void CBaseInvoiceDlg::OnSetInvoiceDate()
{
	m_invoicedate.SetDate(DateTime::now());
}

void CBaseInvoiceDlg::OnUpdateInvoiceHent()
{
	SeasonSession* session = GetSeasonSession();

	uint32_t hentId = m_hentbtn.GetHent();

	if(hentId != NULL_ID)
	{
		HentClientObject* pHentCO = session->GetHent(hentId);
		if(pHentCO)
		{
			Hent* pHent = pHentCO->Object();
			InvoiceHent referenceHent;
			referenceHent.CopyFrom(pHent);

			CInvoiceHentUpdateDlg hentUpdateDlg(m_hent,referenceHent);
			if(hentUpdateDlg.DoModal() == IDOK)
			{
				m_hent.CopyFrom(hentUpdateDlg.GetUpdatedHent());
			}
		}
	}
}

void CBaseInvoiceDlg::OnHentChange(NMHDR* pNMHDR, LRESULT* pResult)
{	
	uint32_t hentId = m_hentbtn.GetHent();
	m_hentgroup.EnableWindow(hentId != NULL_ID);
	SeasonSession* session = GetSeasonSession();
	HentClientObject* pHentCO = session->GetHent(hentId);
	if(pHentCO != NULL)
	{
		Hent* pHent = pHentCO->Object();
		m_hent.CopyFrom(pHent);		
	}
	else
	{
		m_hent.CopyFrom(InvoiceHent());
	}
	
	*pResult = 0;
}

void CBaseInvoiceDlg::DeleteCustomPrice(ItemId id)
{
	m_pCalculation->DisableCustomPrice(id);
}

void CBaseInvoiceDlg::DeleteCustomClass(ItemId id)
{
	m_pCalculation->SetItemCustomClass(id,NULL_ID);
}

void CBaseInvoiceDlg::DeleteCustomWeight(ItemId id)
{
	m_pCalculation->SetItemCustomWeight(id,NullDecimal());
}

void CBaseInvoiceDlg::DeleteCustomPricePerKg(ItemId id)
{
	m_pCalculation->SetItemCustomNetPricePerKg(id,NullDecimal());
}

void CBaseInvoiceDlg::DeleteCustomValues(ItemId id)
{
	DeleteCustomPrice(id);
	DeleteCustomClass(id);
	DeleteCustomWeight(id);
	DeleteCustomPricePerKg(id);
}

void CBaseInvoiceDlg::DeleteOnRow(int row,DeleteAction& action)
{
	CalculationElementRow* pRow = m_pContent->GetCalculationRow(row);
	CalculationElementView* pContent = pRow->Content();
	action(this,pContent->GetItemId());
}

void CBaseInvoiceDlg::DeleteOnAllRows(DeleteAction& action)
{
	CalculationElementRow* pRow = NULL;
	CalculationElementView* pContent;
	for(int r = 0,count = m_pContent->GetRowCount();r < count;r++)
	{
		pRow = m_pContent->GetCalculationRow(r);
		pContent = pRow->Content();
		action(this,pContent->GetItemId());
	}
}

	
void CBaseInvoiceDlg::OnAddDeduction()
{
	CNewDeductionDetailsDlg detailsDlg(this);
	if (detailsDlg.DoModal() == IDOK)
	{
		DeductionDetails details = detailsDlg.GetDeducationDetails();
		InvoiceTransactionFacade* facade = m_pCalculation->GetTransaction();
		InvoiceDeduction deduction;
		deduction.CopyFrom(details);
		deduction.SetIsEnabled(true);
		facade->AddDeduction(deduction);
	}	
}

void CBaseInvoiceDlg::OnDeleteDeduction()
{
	int selectedRow = m_deductionslist.GetCurSel();
	if (selectedRow >= 0)
	{
		DeductionElementRow* row = m_DeductionsContent.GetDeductionRowAt(selectedRow);
		InvoiceTransactionFacade* facade = m_pCalculation->GetTransaction();
		facade->RemoveDeduction(row->Content()->GetItemId());
	}
}

void CBaseInvoiceDlg::OnAddDeductionFromDefinition()
{
	XContext* context = ContextAccess::GetXContext();

	MenuBuilder menu;

	MenuBuilder definitionsMenu;

	DeductionDefinitions definitions = std::move(context->GetDeductionDefinitions());

	if (definitions.size() > 0)
	{
		BOOST_FOREACH(DeductionDefinition* definition, definitions)
		{

			definitionsMenu.add(TextUtils::Format(L"%s - %s", definition->GetCode(), (definition->GetFraction()* Decimal(100L)).ToString(2))
				, std::bind(&CBaseInvoiceDlg::AddDeductionFromDefinition, this, definition->GetId()));

		}

		CRect rc;
		m_addFromDefinitionBtn.GetWindowRect(&rc);

		definitionsMenu.trackAt(rc.left, rc.bottom, this);
	}

}

void CBaseInvoiceDlg::OnDeductionDblClick(int row)
{
	DeductionElementRow* deduction = m_DeductionsContent.GetDeductionRowAt(row);
	DeductionElementView* view = deduction->Content();

	DeductionDetails details;
	details.SetCode(view->GetCode());
	details.SetFraction(view->GetFraction());
	details.SetReason(view->GetReason());

	CEditDeductionDetailsDlg editDeductionDlg(details, this);

		if (editDeductionDlg.DoModal() == IDOK)
		{
			details = editDeductionDlg.GetDeducationDetails();
			InvoiceTransactionFacade* facade = m_pCalculation->GetTransaction();
			InvoiceDeduction deduction;
			deduction.CopyFrom(details);
			deduction.SetIsEnabled(view->GetIsEnabled());
			facade->UpdateDeduction(view->GetItemId(),deduction);
		}


}

void CBaseInvoiceDlg::AddDeductionFromDefinition(uint32_t definitionId)
{

	XContext* context = ContextAccess::GetXContext();
	IDeductionDefinition* definition = context->GetDeductionDefinition(definitionId);
	if (definition != NULL)
	{

		InvoiceTransactionFacade* facade = m_pCalculation->GetTransaction();
		InvoiceDeduction deduction(definition);
		deduction.SetIsEnabled(true);		
		facade->AddDeduction(deduction);
	}
}

void CBaseInvoiceDlg::EnableAllDeductions(bool bEnable)
{
	InvoiceTransactionFacade* facade = m_pCalculation->GetTransaction();
	for (int i = 0, count = facade->GetDeductionCount(); i < count; i++)
	{
		const InvoiceTransactionDeductionItem* deductionItem = facade->GetDeductionAt(i);
		if (deductionItem->IsEnabled() != bEnable)
		{
			InvoiceDeduction deduction = deductionItem->GetDeduction();
			deduction.SetIsEnabled(bEnable);
			facade->UpdateDeduction(deductionItem->GetId(), deduction);
		}
	}
}

void CBaseInvoiceDlg::OnEnableAllDeductions()
{
	EnableAllDeductions(true);
}

void CBaseInvoiceDlg::OnDisableAllDeductions()
{
	EnableAllDeductions(false);
}

std::wstring CBaseInvoiceDlg::GetCaption()
{
	return L"";
}

void CBaseInvoiceDlg::OnEditHent(NMHDR* pNMHDR, LRESULT* pResult)
{
	uint32_t hentId = m_hentbtn.GetHent();
	SeasonSession* session = GetSeasonSession();

	ExceptionMsgBox msgBox;

	Try()
		.Prepare([session, hentId]
	{
		session->LockHent(hentId);
	})
		.Do([this, session, hentId]
	{
		HentEditor editor(session);
		editor.Edit(hentId);
		HentDlg hendDlg(&editor, this);
		hendDlg.DoModal();
	})
		.TearDown([session, hentId]
	{
		session->UnlockHent(hentId);
	})
		.Catch([&msgBox](std::exception& e)
	{
		msgBox.Show(e);
	})
		.Run();
}
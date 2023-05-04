#include "stdafx.h"
#include "BigInvoiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const GRIDHEADERS CBigInvoiceDlg::szCowPriceListCols[] =
{
	{IDS_HEADERHASTERM,		CCowPriceList::COL_HASTERM},
	{IDS_HEADEREAN,			CCowPriceList::COL_EAN},
	{IDS_HEADERSTOCK,		CCowPriceList::COL_STOCK},
	{IDS_HEADERWEIGHT,		CCowPriceList::COL_WEIGHT},
	{IDS_HEADERMYPRICE,		CCowPriceList::COL_MYPRICE},
	{IDS_HEADERHENT,		CCowPriceList::COL_HENT},
	{(UINT)-1,				CCowPriceList::COL_OTHERPRICE},
	{IDS_HEADERRULE,		CCowPriceList::COL_RULE},
	{IDS_HEADERPRICEPERKG,	CCowPriceList::COL_PRICEPERKG},
	{IDS_HEADERNETTO,		CCowPriceList::COL_NETTO},
	{IDS_HEADERVATVALUE,	CCowPriceList::COL_VAT},
	{IDS_HEADERBRUTTO,		CCowPriceList::COL_BRUTTO},
	{0,0}
};


CBigInvoiceDlg::CBigInvoiceDlg(CowPriceInvoiceSession* pInvoiceSession,CWnd* pParent /*=NULL*/) :
	CBaseInvoiceDlg(CBigInvoiceDlg::IDD,
					pInvoiceSession,
					new InvoiceUISettings(CFGID_BIGINVCOWSLIST,szCowPriceListCols),
					pParent),
	m_Rule4All(FALSE),
	m_useMyPerKgPrice(FALSE),	
	m_bCustomPricePerKg(FALSE),
	m_CustomPricePerKg(g_szRegExUnsigned),	
	m_stockpricelist(pInvoiceSession),	
	m_statList(pInvoiceSession),	
	m_RuleList(pInvoiceSession->GetInvoiceContext()),
	m_totalweight(g_szRegExUnsigned)
{
	switch(pInvoiceSession->GetInvoiceContext()->GetBuyOrSell())
	{
		case BUY:
			m_termPriceLabel = IDS_TERMBUYPRICE;
			m_termStockLabel = IDS_TERMBUYSTOCK;
			m_termWeightLabel = IDS_TERMBUYWEIGHT;
		break;
		case SELL:
			m_termPriceLabel = IDS_TERMSELLPRICE;
			m_termStockLabel = IDS_TERMSELLSTOCK;
			m_termWeightLabel = IDS_TERMSELLWEIGHT;
			
		break;
	}

	m_bSetTotalWeight = FALSE;
}


CBigInvoiceDlg::~CBigInvoiceDlg()
{
	delete m_pUISettings;	
}

void CBigInvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseInvoiceDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_RULELIST,m_RuleList);
	DDX_Check(pDX,IDC_RULE4ALL,m_Rule4All);

	DDX_Check(pDX,IDC_CHECKPRICEPERKG,m_bCustomPricePerKg);
	DDX_Control(pDX,IDC_CUSTOMPRICEPERKG,m_CustomPricePerKg);
	DDX_Check(pDX,IDC_USEMYPERKGPRICE,m_useMyPerKgPrice);
	DDX_Control(pDX,IDC_TOTALWEIGHT,m_totalweight);	
	DDX_Control(pDX,IDC_SETTOTALWEIGHT,m_settotalweight);
	DDX_Check(pDX,IDC_SETTOTALWEIGHT,m_bSetTotalWeight);

	DDX_Control(pDX,IDC_USECUSTOMPRICEPERKGCOLORLBL,m_useCustomPricePerKgClrLbl);
	DDX_Control(pDX,IDC_USEMYPRICEPERKGCOLORLBL,m_useMyPerKgPriceClrLbl);
			
}

BOOL CBigInvoiceDlg::OnInitDialog()
{		
	CBaseInvoiceDlg::OnInitDialog();

	
	
	m_settotalweight.SetTitleStyle(BS_AUTOCHECKBOX);
	m_settotalweight.SetCheck(FALSE);
	m_totalweight.SendMessage(EM_SETREADONLY,TRUE);

	OnCustomPricePerKg();

	SetDlgItemResText(IDC_TERMWEIGHT_LABEL,m_termWeightLabel);
	SetDlgItemResText(IDC_TERMPRICE_LABEL,m_termPriceLabel);
	SetDlgItemResText(IDC_TERMSTOCK_LABEL,m_termStockLabel);

	SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);
	SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);
	SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);

	m_useCustomPricePerKgClrLbl.SetBackgroundColor(CUSTOM_PRICE_PER_KG_CLR);
	m_useMyPerKgPriceClrLbl.SetBackgroundColor(STOCK_PER_KG_PRICE_CLR);

	return TRUE;
}


void CBigInvoiceDlg::CreateNaviTab()
{
	
	CBaseInvoiceDlg::CreateNaviTab();


	//AddNaviTab(0,m_pricelist,tabItemRect,IDC_COWLIST,IDS_INVNAVITABCOWPRICES,true);

	AddNaviTab(1,m_stockpricelist,IDC_STOCKLIST,IDS_INVNAVITABSTOCKPRICE,false);

	//AddNaviTab(2,m_docList,tabItemRect,IDC_DOCLIST,IDS_INVNAVITABDOCS,false);

	AddNaviTab(3,m_statList,IDC_STOCKSTATLIST,IDS_INVNAVITABSTAT,false);

	SetCurTab(0);

}






BEGIN_MESSAGE_MAP(CBigInvoiceDlg, CBaseInvoiceDlg)
	ON_BN_CLICKED(IDC_RULE4ALL,OnRule4All)
	ON_LBN_SELCHANGE(IDC_RULELIST,OnRuleChange)
	ON_BN_CLICKED(IDC_RULEEDIT,OnRuleEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT,IDC_STOCKLIST, OnEndLabelEditStockPrice)
	ON_BN_CLICKED(IDC_ADDRULE,OnAddRule)
	ON_BN_CLICKED(IDC_CHECKPRICEPERKG,OnCustomPricePerKg)
	
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_CUSTOMPRICEPERKG,OnEditCustomPricePerKgComplete)
	ON_NOTIFY(EN_EDITCOMPLETE,IDC_TOTALWEIGHT,OnEditTotalWeightComplete)
	ON_BN_CLICKED(IDC_USEMYPERKGPRICE,OnUseMyPerKgPrice)

	ON_BN_CLICKED(IDC_SETTERMWEIGHTBTN,OnSetTermWeight)
	ON_BN_CLICKED(IDC_SETTERMSTOCKBTN,OnSetTermStock)
	ON_BN_CLICKED(IDC_SETTERMPRICEBTN,OnSetTermPrice)

	ON_BN_CLICKED(IDC_SETTERMWEIGHTALL,OnSetTermWeightAll)
	ON_BN_CLICKED(IDC_SETTERMSTOCKALL,OnSetTermStockAll)
	ON_BN_CLICKED(IDC_SETTERMPRICEALL,OnSetTermPriceAll)

	ON_BN_CLICKED(IDC_SETTOTALWEIGHT,OnCheckTotalWeight)

END_MESSAGE_MAP()


void CBigInvoiceDlg::OnRule4All()
{
	UpdateData();
	if(!m_Rule4All)
	{
		m_pInvoiceSession->SetRule4All(NULL);
		UpdateCowPriceList();
		UpdateTotal();
	}
	else
		OnRuleChange();
}

void CBigInvoiceDlg::OnRuleChange()
{
	if(m_Rule4All)
	{	
		int nSel = m_RuleList.GetCurSel();
		if(nSel >= 0)
		{
			Rule *rule  = (Rule*)m_RuleList.GetItemData(nSel);
			m_pInvoiceSession->SetRule4All(rule);
			UpdateCowPriceList();
			UpdateTotal();
		}
	}
}

void CBigInvoiceDlg::TotalNettoChecked()
{
	CBaseInvoiceDlg::TotalNettoChecked();
	bool not_m_settotal = !m_bSetTotalNetto;

	m_RuleList.EnableWindow(not_m_settotal && m_Rule4All);
	GetDlgItem(IDC_RULE4ALL)->EnableWindow(not_m_settotal);
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);

}

void CBigInvoiceDlg::TotalBruttoChecked()
{
	CBaseInvoiceDlg::TotalBruttoChecked();
	bool not_m_settotal = !m_bSetTotalBrutto;


	m_RuleList.EnableWindow(not_m_settotal && m_Rule4All);
	GetDlgItem(IDC_RULE4ALL)->EnableWindow(not_m_settotal);
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!not_m_settotal || !m_bCustomPricePerKg);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(not_m_settotal);
	
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(not_m_settotal);
	
	
}





void CBigInvoiceDlg::OnEndLabelEditStockPrice (NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
    LV_ITEM	*plvItem = &plvDispInfo->item;
	Stock *pStock = NULL;
	
    if (plvItem->pszText != NULL)
    {		
		pStock = m_stockpricelist.GetStockItem(plvItem->iItem);		
		switch(m_stockpricelist.GetColID(plvItem->iSubItem))
		{
			case CStockPricePerKgList::STOCKPERKGPRICE_COL2:
			{
				pStock->ChangeMyPerKgPrice(Money(TextUtils::UniversalAlphaToFloat(plvItem->pszText)));
				if(pStock->UpdateThisStock())
				{
					pStock->CommitChanges();
				}
				else
				{
					pStock->RollbackChanges();
					SQLDB.SQLMessageBox(IDS_ERRUPDATINGSTOCK);
				}
				UpdateCowPriceList();
				UpdateTotal();				
			}
			break;
		}
		m_stockpricelist.Invalidate(FALSE);
	}
	*pResult = 0;
}

void CBigInvoiceDlg::OnEditCustomPricePerKgComplete(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString s;
	
	m_CustomPricePerKg.GetWindowText(s);
	m_pInvoiceSession->SetCustomPricePreKgForAll(Money(TextUtils::UniversalAlphaToFloat(s)));
	m_pInvoiceSession->SetUseCustomPricePerKgForAll(true);
	UpdateCowPriceList();
	UpdateTotal();
	
	*pResult = (LRESULT)0;
}

void CBigInvoiceDlg::OnAddRule()
{
	if(!HAS_PRIVLEDGE(ADDMODOWN))
	{
		AfxMessageBox(IDS_NOPRIVS,MB_OK,0);
		return;
	}

	Rule *newRule = new Rule(NULL_RULEID);
	CNewRuleDlg newRuleDlg(newRule,this);
	if(newRuleDlg.DoModal() == IDOK)
	{
		if(newRule->InsertThisRule())
		{
			_dataContext.ary_rules.Add(newRule);
		}
		else
		{
			SQLDB.SQLMessageBox(IDS_ERRUPDATINGRULE);
			delete newRule;
		}
		m_RuleList.Reload();
	}
	else 
		delete newRule;

}



void CBigInvoiceDlg::OnCustomPricePerKg()
{
	UpdateData();
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_bCustomPricePerKg);
	GetDlgItem(IDC_USEMYPERKGPRICE)->EnableWindow(!m_bCustomPricePerKg);
	if(!m_bCustomPricePerKg)
	{
		m_pInvoiceSession->SetUseCustomPricePerKgForAll(false);
		UpdateCowPriceList();
		UpdateTotal();	
	}
}

void CBigInvoiceDlg::OnRuleEdit()
{
	int nSel = m_RuleList.GetCurSel();
	if(nSel >= 0)
	{
		Rule *pRule  = (Rule*)m_RuleList.GetItemData(nSel);
		
		CEditRuleDlg editRuleDlg(pRule,this);
		if(editRuleDlg.DoModal() == IDOK)
		{
			if(!pRule->UpdateThisRule())
			{
				pRule->RollbackChanges();
				SQLDB.SQLMessageBox(IDS_ERRUPDATINGRULE);
			}
			else
			{
				pRule->CommitChanges();
				UpdateCowPriceList();
				UpdateTotal();
			}
		}
	}	
}

void CBigInvoiceDlg::OnUseMyPerKgPrice()
{
	UpdateData();
	m_pInvoiceSession->SetUseStockMyPerKgPrice(m_useMyPerKgPrice == TRUE);
	UpdateCowPriceList();
	UpdateTotal();	
	m_CustomPricePerKg.SendMessage(EM_SETREADONLY,!m_useMyPerKgPrice);
	GetDlgItem(IDC_CHECKPRICEPERKG)->EnableWindow(!m_useMyPerKgPrice);
}


void CBigInvoiceDlg::OnSetTermWeight()
{
	int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomWeight(m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow()));
		UpdateCowPriceList();
		UpdateTotal();
	}
}
void CBigInvoiceDlg::OnSetTermStock()
{
	int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomStock(m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow()));
		UpdateCowPriceList();
		UpdateTotal();
	}
	
}
void CBigInvoiceDlg::OnSetTermPrice()
{

	int iSel = m_pricelist.GetCurSel();
	if(iSel >= 0)
	{
		CowPriceData *pCowPrice =  m_pInvoiceSession->GetCowPrice(iSel);
		pCowPrice->SetCustomPriceNetto(m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow()));
		UpdateCowPriceList();
		UpdateTotal();
	}
	
}



void CBigInvoiceDlg::OnCowPriceSelected(CowPriceData* pCowPrice)
{
		const InvoiceContext* pInvoiceContext = m_pInvoiceSession->GetInvoiceContext();
		Money termPrice = pInvoiceContext->GetTermPrice(pCowPrice->GetCow());
		NullDouble termWeight = pInvoiceContext->GetTermWeight(pCowPrice->GetCow());
		Stock* pTermStock = pInvoiceContext->GetTermStock(pCowPrice->GetCow());

		if(!termPrice.IsNull())
			SetTermValue(IDC_TERMPRICE_VALUE,termPrice.ToString(),IDC_SETTERMPRICEBTN,TRUE);
		else
			SetTermValue(IDC_TERMPRICE_VALUE,szMissing,IDC_SETTERMPRICEBTN,FALSE);

		if(!termWeight.IsNull())
			SetTermValue(IDC_TERMWEIGHT_VALUE,termWeight.ToString(NULL,3),IDC_SETTERMWEIGHTBTN,TRUE);
		else
			SetTermValue(IDC_TERMWEIGHT_VALUE,szMissing,IDC_SETTERMWEIGHTBTN,FALSE);


		if(pTermStock)
			SetTermValue(IDC_TERMSTOCK_VALUE,(LPCSTR)pTermStock->GetStockCode(),IDC_SETTERMSTOCKBTN,TRUE);
		else
			SetTermValue(IDC_TERMSTOCK_VALUE,szMissing,IDC_SETTERMSTOCKBTN,FALSE);
}


void CBigInvoiceDlg::SetDlgItemResText(UINT nID,UINT resID)
{
	CString text;
	text.LoadString(resID);
	SetDlgItemText(nID,text);
}

void CBigInvoiceDlg::SetTermValue(UINT nValueLabelID,LPCSTR pszValueText,UINT nSetBtnID,BOOL bEnable)
{
	SetDlgItemText(nValueLabelID,pszValueText);
	GetDlgItem(nSetBtnID)->EnableWindow(bEnable);
}


void CBigInvoiceDlg::OnSetTermWeightAll()
{
	CowPriceData *pCowPrice = NULL;
	NullDouble termWeight;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{		
		pCowPrice  =  m_pInvoiceSession->GetCowPrice(i);
		termWeight = m_pInvoiceSession->GetInvoiceContext()->GetTermWeight(pCowPrice->GetCow());
		if(!termWeight.IsNull())
			pCowPrice->SetCustomWeight(termWeight);
	}
	
	UpdateCowPriceList();
	UpdateTotal();

}
void CBigInvoiceDlg::OnSetTermStockAll()
{
	CowPriceData *pCowPrice = NULL;
	Stock* pTermStock;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		pCowPrice = m_pInvoiceSession->GetCowPrice(i);
		pTermStock = m_pInvoiceSession->GetInvoiceContext()->GetTermStock(pCowPrice->GetCow());
		if(pTermStock)
			pCowPrice->SetCustomStock(pTermStock);
	}

	UpdateCowPriceList();
	UpdateTotal();
}

void CBigInvoiceDlg::OnSetTermPriceAll()
{
	Money termPrice;
	CowPriceData *pCowPrice = NULL;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		pCowPrice  = m_pInvoiceSession->GetCowPrice(i);
		termPrice = m_pInvoiceSession->GetInvoiceContext()->GetTermPrice(pCowPrice->GetCow());
		if(!termPrice.IsNull())
			pCowPrice->SetCustomPriceNetto(termPrice);
	}

	UpdateCowPriceList();
	UpdateTotal();	
}

void CBigInvoiceDlg::OnEditTotalWeightComplete(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString s;
	
	m_totalweight.GetWindowText(s);
	Weight weight(TextUtils::UniversalAlphaToFloat(s));

	if(weight > 0.0)
	{
		SetTotalWeight(weight);
	}
	else
	{
		PutTotalWeight();
	}
	
	*pResult = (LRESULT)0;
}

void CBigInvoiceDlg::PutTotalWeight()
{
	m_totalweight.SetCommitedText(m_pInvoiceSession->GetTotalWeight().ToString());
}

void CBigInvoiceDlg::OnCheckTotalWeight()
{
	UpdateData();
	m_totalweight.SendMessage(EM_SETREADONLY,!m_bSetTotalWeight);
}

void CBigInvoiceDlg::UpdateTotal()
{
	CBaseInvoiceDlg::UpdateTotal();
	PutTotalWeight();
}

void CBigInvoiceDlg::SetTotalWeight(const Weight& totalWeight)
{
	CCowPriceDataArray woCustomWeight;

	/*if(totalWeight.IsNull() || totalWeight.ToInt(0) <= 0)
	{
		PutTotalWeight();
		m_bSetTotalWeight = FALSE;
		UpdateData(FALSE);
		OnCheckTotalWeight();
		return;
	}
*/
	Weight totalCustWeight(0.0);	
	
	CowPriceData *cowprice;
	for(int i = 0;i < m_pInvoiceSession->GetCowCount();i++)
	{
		cowprice = m_pInvoiceSession->GetCowPrice(i);
		if(cowprice->IsCustomWeight()) 
			totalCustWeight += cowprice->GetWeight();
		else woCustomWeight.Add(cowprice);
	}

	if(woCustomWeight.GetSize() == 0)
	{
		
		AfxMessageBox(IDS_ERRSETTOTALWEIGHT,MB_OK,0);
		PutTotalWeight();
		m_bSetTotalWeight = FALSE;
		UpdateData(FALSE);
		OnCheckTotalWeight();		
		return;
	}
	
	Weight avgWeight = (totalWeight - totalCustWeight);

	if(avgWeight < 0 || avgWeight < woCustomWeight.GetSize()) 
	{
		AfxMessageBox(IDS_TOTALTOOSMALL);
		PutTotalWeight();
		m_bSetTotalWeight = FALSE;
		UpdateData(FALSE);
		OnCheckTotalWeight();
		return;
	}

	
	m_pInvoiceSession->SetAvgWeight(NullDouble((avgWeight/(double)woCustomWeight.GetSize()).GetDouble()));
	m_pInvoiceSession->EnableAvgWeight(true);
	UpdateTotal();
	UpdateCowPriceList();

}


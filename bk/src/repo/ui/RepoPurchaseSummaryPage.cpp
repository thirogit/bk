#include "stdafx.h"
#include "RepoPurchaseSummaryPage.h"
#include "PurchaseInvoiceCalculator.h"
#include "../../uiconstants/PrecisionContstants.h"
#include <string\IntString.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CRepoPurchaseSummaryPage, ETSLayoutDialog)
CRepoPurchaseSummaryPage::CRepoPurchaseSummaryPage() : ETSLayoutDialog(CRepoPurchaseSummaryPage::IDD,NULL,NULL,false)
{	

}

CRepoPurchaseSummaryPage::~CRepoPurchaseSummaryPage()
{
	
}

void CRepoPurchaseSummaryPage::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutDialog::DoDataExchange(pDX);		
}


BEGIN_MESSAGE_MAP(CRepoPurchaseSummaryPage, ETSLayoutDialog)
	
END_MESSAGE_MAP()


BOOL CRepoPurchaseSummaryPage::OnInitDialog()
{
	ETSLayoutDialog::OnInitDialog();	
	
	return TRUE;
}

void CRepoPurchaseSummaryPage::UpdateFrom(IPurchase* pPurchase)
{

	int totalTransCowCount = 0;
	Decimal totalTransWeight(0L);
	Decimal totalTransNet(0L);
	Decimal totalTransGross(0L);

	int totalCashCowCount = 0;
	Decimal totalCashWeight(0L);
	Decimal totalCashNet(0L);
	Decimal totalCashGross(0L);

	if(pPurchase)
	{
		IPurchaseInvoice* pInvoice = NULL;
		
		for(int i = 0; i < pPurchase->GetInvoiceCount();i++)
		{
			pInvoice = pPurchase->GetInvoiceAt(i);
			PurchaseInvoiceCalculator calculator(pInvoice);
			switch(pInvoice->GetPayWay())
			{
			case PayWay_Cash:
				totalCashCowCount += pInvoice->GetCowCount();
				totalCashWeight += calculator.GetTotalWeight();
				totalCashNet += calculator.GetTotalNet();
				totalCashGross += calculator.GetTotalGross();
				break;
			case PayWay_Transfer:
				totalTransCowCount += pInvoice->GetCowCount();
				totalTransWeight += calculator.GetTotalWeight();
				totalTransNet += calculator.GetTotalNet();
				totalTransGross += calculator.GetTotalGross();
				break;
			}
		}
	}
	
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_COWCOUNT, IntString(totalTransCowCount).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_WEIGHT, totalTransWeight.ToString(WEIGHT_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_NET, totalTransNet.ToString(PRICE_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_TRANS_GROSS, totalTransGross.ToString(PRICE_PREC).c_str());

	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_COWCOUNT, IntString(totalCashCowCount).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_WEIGHT, totalCashWeight.ToString(WEIGHT_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_NET, totalCashNet.ToString(PRICE_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_CASH_GROSS, totalCashGross.ToString(PRICE_PREC).c_str());

	SetDlgItemText(IDC_PURCHASE_TOTAL_COWCOUNT, IntString(totalCashCowCount + totalTransCowCount).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_NET, (totalCashNet + totalTransNet).ToString(PRICE_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_WEIGHT, (totalTransWeight + totalCashWeight).ToString(WEIGHT_PREC).c_str());
	SetDlgItemText(IDC_PURCHASE_TOTAL_GROSS, (totalCashGross + totalTransGross).ToString(PRICE_PREC).c_str());

}




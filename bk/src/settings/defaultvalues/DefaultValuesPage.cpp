#include "stdafx.h"
#include "DefaultValuesPage.h"
#include "../../ui/ExceptionMsgBox.h"
#include "../../context/ContextException.h"
#include "../../ContextAccess.h"
#include "../../content/StocksComboContent.h"
#include "../../content/ClassesComboContent.h"
#include "../DefaultValuesEditor.h"

IMPLEMENT_DYNAMIC(DefaultValuesPage, SettingsPropPage)

DefaultValuesPage::DefaultValuesPage() : SettingsPropPage(DefaultValuesPage::IDD)
{
}

DefaultValuesPage::~DefaultValuesPage()
{
}

void DefaultValuesPage::DoDataExchange(CDataExchange* pDX)
{
	SettingsPropPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEXCB, m_sexes);
	DDX_Check(pDX, IDC_STOCKCHECK, m_bSetStock);
	DDX_Control(pDX, IDC_STOCKSCB, m_stocks);
	DDX_Control(pDX, IDC_CLASSESCB, m_classes);
	DDX_Check(pDX, IDC_COWCLASSCHECK, m_bSetClass);
	
	DDX_Control(pDX, IDC_HENTTYPECB, m_henttypes);
	DDX_Check(pDX, IDC_HENTTYPECHECK, m_bSetHentType);

	DDX_Control(pDX, IDC_INMOTIVESCB, m_inmotives);
	DDX_Check(pDX, IDC_INMOTIVECHECK, m_bSetInMotive);

	DDX_Control(pDX, IDC_OUTMOTIVESCB, m_outmotives);
	DDX_Check(pDX, IDC_OUTMOTIVECHECK, m_bSetOutMotive);

	DDX_Control(pDX, IDC_VATRATEREULARINVOICE, m_vatRateRegInvoiceCb);
	DDX_Control(pDX, IDC_PAYWAYREGULARINVOICE, m_paywayRegInvoiceCb);
	DDX_Check(pDX, IDC_PAYWAYREGULARINVOICECHECK, m_bSetPayWayRegInvoice);

	DDX_Control(pDX, IDC_VATRATELUMPINVOICE, m_vatRateLumpInvoiceCb);
	DDX_Control(pDX, IDC_PAYWAYLUMPINVOICE, m_paywayLumpInvoiceCb);
	DDX_Check(pDX, IDC_PAYWAYLUMPINVOICECHECK, m_bSetPayWayLumpInvoice);

	
	DDX_Control(pDX, IDC_PAYDUEDAYSREGULARINVOICE, m_payDueDaysRegular);
	DDX_Control(pDX, IDC_PAYDUEDAYSLUMPINVOICE, m_payDueDaysLump);
	DDX_Check(pDX, IDC_PAYDUEDAYSREGINVOICECHECK, m_bSetPayDueDaysRegInvoice);
	DDX_Check(pDX, IDC_PAYDUEDAYSLUMPINVOICECHECK, m_bSetPayDueDaysLumpInvoice);

		
	
}


BEGIN_MESSAGE_MAP(DefaultValuesPage, SettingsPropPage)
	
	ON_BN_CLICKED(IDC_STOCKCHECK, OnDefaultStock)
	ON_BN_CLICKED(IDC_COWCLASSCHECK, OnDefaultCowClass)
	ON_BN_CLICKED(IDC_HENTTYPECHECK, OnDefaultHentType)
	ON_BN_CLICKED(IDC_INMOTIVECHECK, OnDefaultInMotive)
	ON_BN_CLICKED(IDC_OUTMOTIVECHECK, OnDefaultOutMotive)
	ON_BN_CLICKED(IDC_PAYWAYREGULARINVOICECHECK, OnDefaultPayWayRegInvoice)
	ON_BN_CLICKED(IDC_PAYWAYLUMPINVOICECHECK, OnDefaultPayWayLumpInvoice)
	ON_BN_CLICKED(IDC_PAYDUEDAYSREGINVOICECHECK, OnDefaultPayDueDaysRegInvoice)
	ON_BN_CLICKED(IDC_PAYDUEDAYSLUMPINVOICECHECK, OnDefaultPayDueDaysLumpInvoice)
	
	
END_MESSAGE_MAP()

BOOL DefaultValuesPage::OnInitDialog()
{
	SettingsPropPage::OnInitDialog();
	XContext* context = ContextAccess::GetXContext();
	DefaultValues values;
	DefaultValuesEditor editor(context);
	editor.Load(values);

	StocksComboContent stocksCbContent;
	stocksCbContent.CreateContent(context);
	m_stocks.SetContent(stocksCbContent);

	ClassesComboContent classesCbContent;
	classesCbContent.CreateContent(context);
	m_classes.SetContent(classesCbContent);

	m_inmotives.InitCombo(MotiveDirection_In);
	m_outmotives.InitCombo(MotiveDirection_Out);

	Set(values);

	return TRUE;
}

void DefaultValuesPage::SaveDefaultValues()
{
	XContext* context = ContextAccess::GetXContext();
	DefaultValues values;
	Get(values);
	DefaultValuesEditor editor(context);
	editor.Save(values);

}

void DefaultValuesPage::Get(DefaultValues& values)
{
	UpdateData();
	values.cowsex = m_sexes.GetSex();
	values.henttype = m_henttypes.GetSelectedHentType();
	values.bSetHentType = m_bSetHentType == TRUE;
	
	values.bSetStock = m_bSetStock == TRUE;
	values.stockId = m_stocks.GetSelectedItemId();

	values.inmotive = m_inmotives.GetSelectedMotive();
	values.bSetInMotive = m_bSetInMotive == TRUE;

	values.outmotive = m_outmotives.GetSelectedMotive();
	values.bSetOutMotive = m_bSetOutMotive == TRUE;
	
	values.paywayRegInvoice = m_paywayRegInvoiceCb.GetSelectedPayWay();
	values.bSetPayWayRegInvoice = m_bSetPayWayRegInvoice == TRUE;
	
	values.paywayLumpInvoice = m_paywayLumpInvoiceCb.GetSelectedPayWay();
	values.bSetPayWayLumpInvoice = m_bSetPayWayLumpInvoice == TRUE;

	values.classId  = m_classes.GetSelectedItemId();
	values.bSetClass = m_bSetClass == TRUE;

	values.vatRateRegInvoice = m_vatRateRegInvoiceCb.GetVATRate();
	values.vatRateLumpInvoice = m_vatRateLumpInvoiceCb.GetVATRate();

	values.bSetPayDueDaysRegInvoice = m_bSetPayDueDaysRegInvoice == TRUE;
	values.payDueDaysRegular = m_payDueDaysRegular.GetInteger();

	values.bSetPayDueDaysLumpInvoice = m_bSetPayDueDaysLumpInvoice == TRUE;
	values.payDueDaysLump = m_payDueDaysLump.GetInteger();

}

void DefaultValuesPage::Set(const DefaultValues& values)
{
	m_sexes.InitCombo(values.cowsex);

	m_henttypes.ReloadCombo(values.henttype);
	m_bSetHentType = values.bSetHentType;
	m_henttypes.EnableWindow(m_bSetHentType);
	
	m_bSetStock = values.bSetStock;
	m_stocks.EnableWindow(m_bSetStock);
	m_stocks.Select(values.stockId);

	m_inmotives.SelectMotive(values.inmotive);
	m_bSetInMotive = values.bSetInMotive;
	m_inmotives.EnableWindow(m_bSetInMotive);

	m_outmotives.SelectMotive(values.outmotive);
	m_bSetOutMotive = values.bSetOutMotive;
	m_outmotives.EnableWindow(m_bSetOutMotive);

	m_paywayRegInvoiceCb.ReloadCombo(values.paywayRegInvoice);
	m_bSetPayWayRegInvoice = values.bSetPayWayRegInvoice;
	m_paywayRegInvoiceCb.EnableWindow(m_bSetPayWayRegInvoice);

	m_paywayLumpInvoiceCb.ReloadCombo(values.paywayLumpInvoice);
	m_bSetPayWayLumpInvoice = values.bSetPayWayLumpInvoice;
	m_paywayLumpInvoiceCb.EnableWindow(m_bSetPayWayLumpInvoice);

	m_classes.Select(values.classId);
	m_bSetClass = values.bSetClass;
	m_classes.EnableWindow(m_bSetClass);

	m_bSetPayDueDaysRegInvoice = values.bSetPayDueDaysRegInvoice;	
	m_payDueDaysRegular.SetInteger(values.payDueDaysRegular);
	m_payDueDaysRegular.EnableWindow(m_bSetPayDueDaysRegInvoice);
	
	m_bSetPayDueDaysLumpInvoice = values.bSetPayDueDaysLumpInvoice;
	m_payDueDaysLump.SetInteger(values.payDueDaysLump);
	m_payDueDaysLump.EnableWindow(m_bSetPayDueDaysLumpInvoice);

	m_vatRateLumpInvoiceCb.SetVATRate(values.vatRateLumpInvoice);
	m_vatRateRegInvoiceCb.SetVATRate(values.vatRateRegInvoice);

	

	UpdateData(FALSE);
}


BOOL DefaultValuesPage::OnApply()
{

	UpdateData();

	try
	{
		SaveDefaultValues();
		SetModified(FALSE);
		return SettingsPropPage::OnApply();
	}
	catch (ContextException& e)
	{
		ExceptionMsgBox().Show(e);
		return FALSE;
	}


	
}

void DefaultValuesPage::OnDefaultStock()
{
	UpdateData();
	m_stocks.EnableWindow(m_bSetStock);
}

void DefaultValuesPage::OnDefaultCowClass()
{
	UpdateData();
	m_classes.EnableWindow(m_bSetClass);
}

void DefaultValuesPage::OnDefaultHentType()
{
	UpdateData();
	m_henttypes.EnableWindow(m_bSetHentType);
}

void DefaultValuesPage::OnDefaultInMotive()
{
	UpdateData();
	m_inmotives.EnableWindow(m_bSetInMotive);
}

void DefaultValuesPage::OnDefaultOutMotive()
{
	UpdateData();
	m_outmotives.EnableWindow(m_bSetOutMotive);
}

void DefaultValuesPage::OnDefaultPayWayRegInvoice()
{
	UpdateData();
	m_paywayRegInvoiceCb.EnableWindow(m_bSetPayWayRegInvoice);
}

void DefaultValuesPage::OnDefaultPayWayLumpInvoice()
{
	UpdateData();
	m_paywayLumpInvoiceCb.EnableWindow(m_bSetPayWayLumpInvoice);
}


void DefaultValuesPage::OnDefaultPayDueDaysRegInvoice()
{
	UpdateData();
	m_payDueDaysRegular.EnableWindow(m_bSetPayDueDaysRegInvoice);
}

void DefaultValuesPage::OnDefaultPayDueDaysLumpInvoice()
{
	UpdateData();
	m_payDueDaysLump.EnableWindow(m_bSetPayDueDaysLumpInvoice);
}




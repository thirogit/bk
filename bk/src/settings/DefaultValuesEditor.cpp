#include "stdafx.h"
#include "DefaultValuesEditor.h"

const wchar_t* OPTNM_DEFAULT_STOCK = L"DEFAULT_STOCK";
const wchar_t* OPTNM_DEFAULT_STOCK_ENABLED = L"DEFAULT_STOCK_ENABLED";

const wchar_t* OPTNM_DEFAULT_CLASS = L"DEFAULT_CLASS";
const wchar_t* OPTNM_DEFAULT_CLASS_ENABLED = L"DEFAULT_CLASS_ENABLED";

const wchar_t* OPTNM_DEFAULT_COWSEX = L"DEFAULT_COWSEX";

const wchar_t* OPTNM_DEFAULT_HENTTYPE = L"DEFAULT_HENTTYPE";
const wchar_t* OPTNM_DEFAULT_HENTTYPE_ENABLED = L"DEFAULT_HENTTYPE_ENABLED";

const wchar_t* OPTNM_DEFAULT_INMOTIVE = L"DEFAULT_INMOTIVE";
const wchar_t* OPTNM_DEFAULT_INMOTIVE_ENABLED = L"DEFAULT_INMOTIVE_ENABLED";

const wchar_t* OPTNM_DEFAULT_OUTMOTIVE = L"DEFAULT_OUTMOTIVE";
const wchar_t* OPTNM_DEFAULT_OUTMOTIVE_ENABLED = L"DEFAULT_OUTMOTIVE_ENABLED";

const wchar_t* OPTNM_VATRATE_REG_INVOICE = L"VATRATE_REG_INVOICE";

const wchar_t* OPTNM_DEFAULT_PAYWAY_REG_INVOICE = L"DEFAULT_PAYWAY_REG_INVOICE";
const wchar_t* OPTNM_DEFAULT_PAYWAY_REG_INVOICE_ENABLED = L"DEFAULT_PAYWAY_REG_INVOICE_ENABLED";

const wchar_t* OPTNM_VATRATE_LUMP_INVOICE = L"VATRATE_LUMP_INVOICE";

const wchar_t* OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE = L"DEFAULT_PAYWAY_LUMP_INVOICE";
const wchar_t* OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE_ENABLED = L"DEFAULT_PAYWAY_LUMP_INVOICE_ENABLED";

const wchar_t* OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE = L"DEFAULT_PAYDUEDAYS_REG_INVOICE";
const wchar_t* OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE_ENABLED = L"DEFAULT_PAYDUEDAYS_REG_INVOICE_ENABLED";

const wchar_t* OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE = L"DEFAULT_PAYDUEDAYS_LUMP_INVOICE";
const wchar_t* OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE_ENABLED = L"DEFAULT_PAYDUEDAYS_LUMP_INVOICE_ENABLED";


DefaultValuesEditor::DefaultValuesEditor(XContext* context) : m_context(context)
{
}
	
void DefaultValuesEditor::Load(DefaultValues& values)
{	
	UserSettings* settings = m_context->GetUserSettings();

	values.bSetStock = settings->GetSetting<bool>(OPTNM_DEFAULT_STOCK_ENABLED,false);
	values.stockId = settings->GetSetting<uint32_t>(OPTNM_DEFAULT_STOCK,NULL_ID);

	values.bSetClass = settings->GetSetting<bool>(OPTNM_DEFAULT_CLASS_ENABLED, false);
	values.classId = settings->GetSetting<uint32_t>(OPTNM_DEFAULT_CLASS, NULL_ID);

	values.cowsex = (COWSEX)settings->GetSetting<int>(OPTNM_DEFAULT_COWSEX, (int)(SEX_UNK));

	values.henttype = (HentType)settings->GetSetting<int>(OPTNM_DEFAULT_HENTTYPE, (int)HentType_None);
	values.bSetHentType = settings->GetSetting<bool>(OPTNM_DEFAULT_HENTTYPE_ENABLED, false);

	values.inmotive = (Motive)settings->GetSetting<int>(OPTNM_DEFAULT_INMOTIVE, (int)Motive_None);
	values.bSetInMotive = settings->GetSetting<bool>(OPTNM_DEFAULT_INMOTIVE_ENABLED, false);

	values.outmotive = (Motive)settings->GetSetting<int>(OPTNM_DEFAULT_OUTMOTIVE, (int)Motive_None);
	values.bSetOutMotive = settings->GetSetting<bool>(OPTNM_DEFAULT_OUTMOTIVE_ENABLED, false);

	if (settings->HasSetting(OPTNM_VATRATE_REG_INVOICE))
	{
		values.vatRateRegInvoice = settings->GetSetting < Decimal > (OPTNM_VATRATE_REG_INVOICE);
	}
	else
	{
		values.vatRateRegInvoice.MakeNull();
	}

	values.paywayRegInvoice = (PayWay)settings->GetSetting<int>(OPTNM_DEFAULT_PAYWAY_REG_INVOICE, (int)PayWay_None);
	values.bSetPayWayRegInvoice = settings->GetSetting<bool>(OPTNM_DEFAULT_PAYWAY_REG_INVOICE_ENABLED, false);


	if (settings->HasSetting(OPTNM_VATRATE_LUMP_INVOICE))
	{
		values.vatRateLumpInvoice = settings->GetSetting < Decimal >(OPTNM_VATRATE_LUMP_INVOICE);
	}
	else
	{
		values.vatRateLumpInvoice.MakeNull();
	}

	
	values.paywayLumpInvoice = (PayWay)settings->GetSetting<int>(OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE, (int)PayWay_None);
	values.bSetPayWayLumpInvoice = settings->GetSetting<bool>(OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE_ENABLED, false);

	
	if (settings->HasSetting(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE))
	{
		values.payDueDaysRegular = settings->GetSetting < int >(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE);
	}
	else
	{
		values.payDueDaysRegular.MakeNull();
	}
	
	values.bSetPayDueDaysRegInvoice = settings->GetSetting<bool>(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE_ENABLED, false);

	
	if (settings->HasSetting(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE))
	{
		values.payDueDaysLump = settings->GetSetting < int >(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE);
	}
	else
	{
		values.payDueDaysLump.MakeNull();
	}
	
	values.bSetPayDueDaysLumpInvoice = settings->GetSetting<bool>(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE_ENABLED, false);


}

void DefaultValuesEditor::Save(const DefaultValues& values)
{
	UserSettings* settings = m_context->GetUserSettings();

	settings->SetSetting(OPTNM_DEFAULT_STOCK_ENABLED, values.bSetStock);
	settings->SetSetting(OPTNM_DEFAULT_STOCK, values.stockId);
	
	 settings->SetSetting<bool>(OPTNM_DEFAULT_CLASS_ENABLED, values.bSetClass);
	 settings->SetSetting<uint32_t>(OPTNM_DEFAULT_CLASS, values.classId);

	 settings->SetSetting<int>(OPTNM_DEFAULT_COWSEX, (int)(values.cowsex.getCOWSEX()));

	 settings->SetSetting<int>(OPTNM_DEFAULT_HENTTYPE, (int)values.henttype);
	 settings->SetSetting<bool>(OPTNM_DEFAULT_HENTTYPE_ENABLED, values.bSetHentType);

	 settings->SetSetting<int>(OPTNM_DEFAULT_INMOTIVE, (int)values.inmotive);
	 settings->SetSetting<bool>(OPTNM_DEFAULT_INMOTIVE_ENABLED, values.bSetInMotive);

	 settings->SetSetting<int>(OPTNM_DEFAULT_OUTMOTIVE, (int)values.outmotive);
	 settings->SetSetting<bool>(OPTNM_DEFAULT_OUTMOTIVE_ENABLED, values.bSetOutMotive);

	if (!values.vatRateRegInvoice.IsNull())
	{
		settings->SetSetting < Decimal >(OPTNM_VATRATE_REG_INVOICE, values.vatRateRegInvoice.ToDecimal(Decimal(0L)));
	}
	else
	{
		settings->ClearSetting(OPTNM_VATRATE_REG_INVOICE);
	}

	settings->SetSetting<int>(OPTNM_DEFAULT_PAYWAY_REG_INVOICE, (int)values.paywayRegInvoice);
	settings->SetSetting<bool>(OPTNM_DEFAULT_PAYWAY_REG_INVOICE_ENABLED, values.bSetPayWayRegInvoice);


	if (!values.vatRateLumpInvoice.IsNull())
	{
		settings->SetSetting < Decimal >(OPTNM_VATRATE_LUMP_INVOICE, values.vatRateLumpInvoice.ToDecimal(Decimal(0L)));
	}
	else
	{
		settings->ClearSetting(OPTNM_VATRATE_LUMP_INVOICE);
	}


	 settings->SetSetting<int>(OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE, (int)values.paywayLumpInvoice);
	 settings->SetSetting<bool>(OPTNM_DEFAULT_PAYWAY_LUMP_INVOICE_ENABLED, values.bSetPayWayLumpInvoice);


	if (!values.payDueDaysRegular.IsNull())
	{
		settings->SetSetting < int >(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE, values.payDueDaysRegular.ToInt());
	}
	else
	{
		settings->ClearSetting(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE);
	}

	settings->SetSetting<bool>(OPTNM_DEFAULT_PAYDUEDAYS_REG_INVOICE_ENABLED, values.bSetPayDueDaysRegInvoice);


	if (!values.payDueDaysLump.IsNull())
	{
		settings->SetSetting < int >(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE, values.payDueDaysLump.ToInt());
	}
	else
	{
		settings->ClearSetting(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE);
	}

	settings->SetSetting<bool>(OPTNM_DEFAULT_PAYDUEDAYS_LUMP_INVOICE_ENABLED, values.bSetPayDueDaysLumpInvoice);


}


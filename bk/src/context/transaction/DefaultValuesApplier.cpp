#include "stdafx.h"
#include "DefaultValuesApplier.h"
#include "../../settings/DefaultValuesEditor.h"


DefaultValuesApplier::DefaultValuesApplier(SeasonSession* session)
{
	XContext* context = session->GetContext();
	DefaultValuesEditor defaultValueEditor(context);
	DefaultValues defaultValues;
	defaultValueEditor.Load(m_values);
}

DefaultValuesApplier::DefaultValuesApplier(const DefaultValues& values)
{
	m_values = values;	
}	
	
void DefaultValuesApplier::ApplyTo(CowEditor* editor) const
{
	editor->SetSex(m_values.cowsex);
	if (m_values.bSetClass)
	{
		editor->SetClassId(m_values.classId);
	}

	if (m_values.bSetStock)
	{
		editor->SetStockId(m_values.stockId);
	}
}

void DefaultValuesApplier::ApplyTo(InvoiceTransactionFacade* transaction) const
{
	InvoiceDetails details = transaction->GetDetails();
	switch (transaction->GetInvoiceType())
	{
	case InvoiceType_Regular:
		if (!m_values.vatRateRegInvoice.IsNull())
		{
			details.SetVATRate(m_values.vatRateRegInvoice.ToDecimal(Decimal(0L)));
		}

		if (m_values.bSetPayWayRegInvoice)
		{
			details.SetPayWay(m_values.paywayRegInvoice);
		}

		if (m_values.bSetPayDueDaysRegInvoice && !m_values.payDueDaysRegular.IsNull())
		{
			details.SetPayDueDays(m_values.payDueDaysRegular.ToInt(0));
		}


		break;
	case InvoiceType_Lump:
		if (!m_values.vatRateLumpInvoice.IsNull())
		{
			details.SetVATRate(m_values.vatRateLumpInvoice.ToDecimal(Decimal(0L)));
		}

		if (m_values.bSetPayWayLumpInvoice)
		{
			details.SetPayWay(m_values.paywayLumpInvoice);
		}

		if (m_values.bSetPayDueDaysLumpInvoice && !m_values.payDueDaysLump.IsNull())
		{
			details.SetPayDueDays(m_values.payDueDaysLump.ToInt(0));
		}
		break;
	}
	transaction->UpdateDetails(details);
}

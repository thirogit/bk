#include "stdafx.h"
#include "InvoicesSummary.h"

#include <string\IntString.h>
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../view/BuyInvoiceViewCreator.h"
#include "../../view/SellInvoiceViewCreator.h"
#include "../../text/YesNoText.h"
#include "../../invoice/calculator/InvoiceCalculator.h"

const int price_precision = 2;
const int weight_precision = 3;

InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::ItemsBandEvaluator(InvoicesSummaryDS* parent) : m_parent(parent)
{

}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_parent->m_pInputInterface->RowCount();
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::At(int index)
{	
	return std::move(CreateRowValues(index, m_parent->m_parent->m_pInputInterface->RowAt(index)));
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const InvoiceView* row)
{
	
	bkprint::Values values;

	values.Set(L"INVOICENO", row->GetCustomNumber());
	values.Set(L"INTERNALINVNO", row->GetInvoiceNo());
	values.Set(L"INVOICEDATE", row->GetInvoiceDate().GetDayDate());
	values.Set(L"COWAMOUNT", IntString(row->GetCowQty()));
	values.Set(L"VATRATE", (row->GetVatRate()*Decimal(100L)).ToString(1) + L'%');
	values.Set(L"PAYWAY", m_parent->m_parent->m_pContext->GetPayWayText(row->GetPayWay()));
	values.Set(L"HENT", row->GetInvoiceHent());

	std::wstring isPaidValue;
	DateTime paidDt = row->GetPaidDate();
	if (!paidDt.IsNull())
		isPaidValue = paidDt.GetDayDate();
	else
		isPaidValue = YesNoText(false).ToString();

	values.Set(L"ISPAID", isPaidValue);
	values.Set(L"VATVALUE", row->GetTotalTaxValue().ToString(price_precision));
	values.Set(L"NETVALUE", row->GetTotalNetValue().ToString(price_precision));
	values.Set(L"WEIGHTVALUE", row->GetTotalWeight().ToString(weight_precision));

	std::wstring totalGrossValue = row->GetTotalGrossValue().ToString(price_precision);
	if (row->GetDeductedAmount() > Decimal(0L))
	{
		totalGrossValue += L'/';
		totalGrossValue += row->GetGrossAfterDeductions().ToString(price_precision);
	}

	values.Set(L"GROSSVALUE", totalGrossValue);

	
	return std::move(values);
}


//#######################################################################################

InvoicesSummary::InvoicesSummaryDS::InvoicesSummaryDS(InvoicesSummary* parent, IPrintProfile* profile) : m_parent(parent)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

InvoicesSummary::InvoicesSummaryDS::~InvoicesSummaryDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::GetHeaderValues()
{
	return std::move(bkprint::Values());
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::GetFooterValues()
{
	
	bkprint::Values footer;
	footer.Set(L"TOTALVATVALUE", this->m_parent->m_pInputInterface->GetTotalTax().ToString(price_precision));
	footer.Set(L"TOTALNETVALUE", this->m_parent->m_pInputInterface->GetTotalNet().ToString(price_precision));

	Decimal totalGross = this->m_parent->m_pInputInterface->GetTotalGross();
	Decimal totalGrossAfterDeductions = this->m_parent->m_pInputInterface->GetTotalGrossAfterDeductions();
	std::wstring totalGrossValue = totalGross.ToString(price_precision);

	if (totalGross != totalGrossAfterDeductions)
	{
		totalGrossValue += L"/";
		totalGrossValue += totalGrossAfterDeductions.ToString(price_precision);
	}

	footer.Set(L"TOTALGROSSVALUE", totalGrossValue);
	footer.Set(L"TOTALCOWAMOUNT", IntString(this->m_parent->m_pInputInterface->GetTotalCowAmount()));
	footer.Set(L"TOTALWEIGHTVALUE", this->m_parent->m_pInputInterface->GetTotalWeight().ToString(weight_precision));

	
	return std::move(footer);
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values InvoicesSummary::InvoicesSummaryDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* InvoicesSummary::InvoicesSummaryDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

InvoicesSummary::InvoicesSummary(bkprint::Template* templ, InvoicesSummaryInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

InvoicesSummary::~InvoicesSummary()
{
}

bool InvoicesSummary::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut InvoicesSummary::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	InvoicesSummaryDS ds(this, profile);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));	

	return std::move(printOut);
}

void InvoicesSummary::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################

InvoicesSummaryInputInterface::~InvoicesSummaryInputInterface()
{
	DeleteRows();
}

void InvoicesSummaryInputInterface::DeleteRows()
{
	DeepDelete(m_rows);
	m_rows.clear();
}

size_t InvoicesSummaryInputInterface::RowCount() const
{
	return m_rows.size();
}

const InvoiceView* InvoicesSummaryInputInterface::RowAt(int index)
{
	return m_rows[index];
}

InvoiceViewRange InvoicesSummaryInputInterface::GetRows()
{
	return InvoiceViewRange(m_rows);
}

bool InvoicesSummaryInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void InvoicesSummaryInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRows();
	m_totalTax = Decimal(0L);
	m_totalNet = Decimal(0L);
	m_totalGross = Decimal(0L);
	m_totalWeight = Decimal(0L);
	m_totalCowAmount = 0;
	Consume(pSession,range);
}

void InvoicesSummaryInputInterface::AddRow(InvoiceView* row)
{
	m_rows.push_back(row);
}

const Decimal& InvoicesSummaryInputInterface::GetTotalWeight() const
{
	return m_totalWeight;
}

const Decimal& InvoicesSummaryInputInterface::GetTotalTax() const
{
	return m_totalTax;
}

const Decimal& InvoicesSummaryInputInterface::GetTotalNet() const
{
	return m_totalNet;
}

const Decimal& InvoicesSummaryInputInterface::GetTotalGross() const
{
	return m_totalGross;
}

int InvoicesSummaryInputInterface::GetTotalCowAmount() const
{
	return m_totalCowAmount;
}

const Decimal& InvoicesSummaryInputInterface::GetTotalGrossAfterDeductions() const
{
	return m_totalGrossAfterDeductions;
}
//#################################################################################################

void BuyInvoicesSummaryInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{	
	BuyInvoiceViewCreator viewCreator(pSession);
	BOOST_FOREACH(uint32_t invoiceId,range)
	{
		BuyInvoice* invoice = pSession->GetBuyInvoice(invoiceId)->Object();
		AddRow(viewCreator.CreateView(invoice));
		InvoiceCalculator::CalculationResult result = InvoiceCalculator::Calculate(invoice);
		m_totalTax += result.GetTaxValue();
		m_totalNet += result.GetNetValue();
		m_totalGrossAfterDeductions += result.GetGrossAfterDeductionsValue();
		m_totalGross += result.GetGrossValue();
		m_totalCowAmount += invoice->GetCowQty();
		m_totalWeight += result.GetWeight();
	}
}

//-------------------------------------------------------------------------------------------------
void SellInvoicesSummaryInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	SellInvoiceViewCreator viewCreator(pSession);
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		SellInvoice* invoice = pSession->GetSellInvoice(invoiceId)->Object();
		AddRow(viewCreator.CreateView(invoice));
	}
}


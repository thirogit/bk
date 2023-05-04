#include "stdafx.h"
#include "InvoiceSpecification.h"
#include <boost\foreach.hpp>
#include <string\IntString.h>
#include <arrays\DeepDelete.h>
#include <string/TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../invoice/calculator/InvoiceCalculator.h"
#include <data\utils\GetDocInvoice.h>

InvoiceSpecification::HyperInvoiceSpecDS::HyperInvoiceSpecDS(InvoiceSpecificationInputInterface* inputInterface, IPrintProfile* profile, DocumentContext* pContext)
{
	BOOST_FOREACH(const InvoiceSpecificationRecord* record, inputInterface->GetRecords())
	{
		m_dataSources.push_back(new InvoiceSpecDS(record, profile, pContext));
	}
}

InvoiceSpecification::HyperInvoiceSpecDS::~HyperInvoiceSpecDS()
{
	DeepDelete(m_dataSources);
}


int InvoiceSpecification::HyperInvoiceSpecDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* InvoiceSpecification::HyperInvoiceSpecDS::GetDS(int index)
{
	return m_dataSources[index];
}

//#######################################################################################
InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::ItemsBandEvaluator(InvoiceSpecDS* parent) : m_parent(parent)
{

}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::Footer()
{
	bkprint::Values values;
	return std::move(values);
}

size_t InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::At(int index)
{
	const InvoiceSpecificationRow* row = m_parent->m_record->rows[index];
	return std::move(CreateRowValues(index, row));
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const InvoiceSpecificationRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex+1));
	values.Set(L"FIRSTSTOCK", row->herdClassCd);
	values.Set(L"FIRSTWEIGHT", row->herdWeight.ToString(3));

	values.Set(L"SECONDSTOCK", row->paidClassCd);	
	values.Set(L"SECONDWEIGHT", row->paidWeight.ToString(3));
	values.Set(L"COWNO", row->cowNo.ToString());
	
	
	Decimal perKg = row->netValue / row->paidWeight;

	values.Set(L"UNITPRICENET", perKg.ToString(5));
	values.Set(L"NETTO", row->netValue.ToString(2));
	values.Set(L"VAT", row->vatValue.ToString(2));
	values.Set(L"BRUTTO", row->grossValue.ToString(2));

	if (row->buyFromHent.is_initialized())
	{
		values.Set(L"BUYFROMHENT", row->buyFromHent->name);
	}
	return std::move(values);
}




//#######################################################################################


InvoiceSpecification::InvoiceSpecDS::InvoiceSpecDS(const InvoiceSpecificationRecord* record, IPrintProfile* profile, DocumentContext* pContext) :
m_pContext(pContext), m_record(record)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);	
}

InvoiceSpecification::InvoiceSpecDS::~InvoiceSpecDS()
{	
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"SELLER", FormatHent(&m_record->seller));
	headerValues.Set(L"BUYER", FormatHent(&m_record->buyer));

	return std::move(headerValues);
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::GetFooterValues()
{
	bkprint::Values values;	
	values.Set(L"TOTALGROSSVALUE", m_record->totalGrossAfterDeductions.ToString(2));
	values.Set(L"TOTALNETTO", m_record->totalNet.ToString(2));
	values.Set(L"TOTALFIRSTWEIGHT", m_record->totalHerdWeight.ToString(3));
	values.Set(L"TOTALSECONDWEIGHT", m_record->totalPaidWeight.ToString(3));
	values.Set(L"COWAMOUNT", IntString(m_record->rows.size()));
	values.Set(L"VATRATE", (m_record->vatRate * Decimal(100L)).ToString(1));
	values.Set(L"TOTALVATVALUE", m_record->totalVat.ToString(2));

	return std::move(values);
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;

	values.Set(L"INVOICENO", m_record->invoiceNo);
	values.Set(L"INVOICEDATE", m_record->invoiceDate.GetDayDate());

	return std::move(values);
}

bkprint::Values InvoiceSpecification::InvoiceSpecDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* InvoiceSpecification::InvoiceSpecDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;	

	return NULL;
}

std::wstring InvoiceSpecification::InvoiceSpecDS::FormatHent(const HentRecord* hent)
{
	return TextUtils::Format(L"%s\n%s %s\n%s %s\n%s",hent->name, hent->street, hent->pobox, hent->zip, hent->city,hent->fiscalno);
}

//#######################################################################################

InvoiceSpecification::InvoiceSpecification(bkprint::Template* templ, InvoiceSpecificationInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool InvoiceSpecification::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

void InvoiceSpecification::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

bkprint::PrintOut InvoiceSpecification::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;	
	
	HyperInvoiceSpecDS ds(m_pInputInterface, profile, m_pContext);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	
	return std::move(printOut);
}

InvoiceSpecification::~InvoiceSpecification()
{
}

//#################################################################################################

InvoiceSpecificationInputInterface::~InvoiceSpecificationInputInterface()
{
	DeleteRecords();
}

InvoiceSpeciticationRecordRange InvoiceSpecificationInputInterface::GetRecords()
{
	return InvoiceSpeciticationRecordRange(m_records);
}

bool InvoiceSpecificationInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void InvoiceSpecificationInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void InvoiceSpecificationInputInterface::AddRecord(InvoiceSpecificationRecord* record)
{
	m_records.push_back(record);
}

void InvoiceSpecificationInputInterface::DeleteRecords()
{
	BOOST_FOREACH(InvoiceSpecificationRecord* record, m_records)
	{
		DeepDelete(record->rows);		
		delete record;
	}
	m_records.clear();
}

InvoiceSpecificationRecord* InvoiceSpecificationInputInterface::CreateRecord(SeasonSession* session,IInvoice* invoice)
{
	InvoiceSpecificationRecord* record = new InvoiceSpecificationRecord();

	record->buyer.CopyFrom(session->GetFarm());
	record->seller.CopyFrom(invoice->GetInvoiceHent());

	record->invoiceDate = invoice->GetInvoiceDate();

	record->invoiceNo = invoice->GetCustomNumber();
	if (record->invoiceNo.empty())
		record->invoiceNo = IntString(invoice->GetInvoiceNo());

	InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);

	record->totalNet = calculation.GetNetValue();
	record->totalVat = calculation.GetTaxValue();
	record->totalGross = calculation.GetGrossValue();
	record->totalGrossAfterDeductions = calculation.GetGrossAfterDeductionsValue();
	record->vatRate = invoice->GetVATRate();

	PtrEnumerator<ICowInvoiceEntry> entries = invoice->EnumEntries();
	while (entries.hasNext())
	{
		ICowInvoiceEntry* entry = *entries;
		InvoiceSpecificationRow* row = new InvoiceSpecificationRow();
		ICow* cow = session->GetCow(entry->GetCowId())->Object();

		row->cowId = cow->GetId();
		row->cowNo = cow->GetCowNo();

		IClass* herdCowClass = session->GetClass(cow->GetClassId());
		IClass* paidCowClass = session->GetClass(entry->GetClassId());

		row->herdWeight = cow->GetWeight();
		record->totalHerdWeight += row->herdWeight;

		row->herdClassCd = herdCowClass->GetClassCode();
		row->paidClassCd = paidCowClass->GetClassCode();

		row->paidWeight = entry->GetWeight();
		record->totalPaidWeight += row->paidWeight;

		row->netValue = entry->GetPrice();
		row->pricePerKg = row->netValue / row->paidWeight;
		row->vatValue = row->netValue * invoice->GetVATRate();
		row->grossValue = row->netValue + row->vatValue;

		entries.advance();
		record->rows.push_back(row);
	}

	return record;

}

//#################################################################################################

void BuyInvoiceSpecificationInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = pSession->GetBuyInvoice(invoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();		
		AddRecord(CreateRecord(pSession, pBuyInvoice));
	}
}

//#################################################################################################

void SellInvoiceSpecificationInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		SellInvoiceClientObject* pInvoiceCO = pSession->GetSellInvoice(invoiceId);
		SellInvoice* pSellInvoice = pInvoiceCO->Object();

		InvoiceSpecificationRecord* record = CreateRecord(pSession, pSellInvoice);

		BOOST_FOREACH(InvoiceSpecificationRow* row, record->rows)
		{
			ICowTrail* trail = pSession->GetCowTrail(row->cowId);
			const ICowFootprint* entryFootprint = trail->GetEntry();
			uint32_t buyInvoiceId = GetDocInvoice(entryFootprint->GetDoc());
			ICow* cow = pSession->GetCow(row->cowId)->Object();

			if (buyInvoiceId == NULL_ID)
			{
				buyInvoiceId = cow->GetBuyInvoiceId();
			}

			if (buyInvoiceId != NULL_ID)
			{
				BuyInvoice* buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
				HentRecord buyFromHent;
				buyFromHent.CopyFrom(buyInvoice->GetInvoiceHent());
				row->buyFromHent = buyFromHent;
			}
		}

		AddRecord(record);
	}
}
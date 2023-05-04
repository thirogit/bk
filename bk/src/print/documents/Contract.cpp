#include "stdafx.h"
#include "Contract.h"
#include <boost\foreach.hpp>
#include <string\IntString.h>
#include <arrays\DeepDelete.h>
#include <string/TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../invoice/calculator/InvoiceCalculator.h"
#include <data\utils\GetDocInvoice.h>

ContractDocument::ContractHyperDS::ContractHyperDS(ContractInputInterface* inputInterface, IPrintProfile* profile, DocumentContext* pContext)
{
	BOOST_FOREACH(const ContractRecord* record, inputInterface->GetRecords())
	{
		m_dataSources.push_back(new ContractDS(record, profile, pContext));
	}
}

ContractDocument::ContractHyperDS::~ContractHyperDS()
{
	DeepDelete(m_dataSources);
}


int ContractDocument::ContractHyperDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* ContractDocument::ContractHyperDS::GetDS(int index)
{
	return m_dataSources[index];
}

//#######################################################################################
ContractDocument::ContractDS::ItemsBandEvaluator::ItemsBandEvaluator(ContractDS* parent) : m_parent(parent)
{

}

bkprint::Values ContractDocument::ContractDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values ContractDocument::ContractDS::ItemsBandEvaluator::Footer()
{
	bkprint::Values values;
	values.Set(L"TRANSACTIONDT", m_parent->m_record->transactionDate.GetDayDate());
	return std::move(values);
}

size_t ContractDocument::ContractDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values ContractDocument::ContractDS::ItemsBandEvaluator::At(int index)
{
	const ContractRow* row = m_parent->m_record->rows[index];
	return std::move(CreateRowValues(index, row));
}

bkprint::Values ContractDocument::ContractDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const ContractRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex+1));
	values.Set(L"COWNO", row->cowNo.ToString());
		
	Decimal perKgGross = row->grossValue / row->paidWeight;

	values.Set(L"GROSSPRICEPERKG", perKgGross.ToString(5));
	
	return std::move(values);
}




//#######################################################################################


ContractDocument::ContractDS::ContractDS(const ContractRecord* record, IPrintProfile* profile, DocumentContext* pContext) :
m_pContext(pContext), m_record(record)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);	
}

ContractDocument::ContractDS::~ContractDS()
{	
}

bkprint::Values ContractDocument::ContractDS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"SELLER", FormatHent(&m_record->seller));
	headerValues.Set(L"BUYER", FormatHent(&m_record->buyer));
	headerValues.Set(L"CONTRACTNO", m_record->invoiceNo);
	

	return std::move(headerValues);
}

bkprint::Values ContractDocument::ContractDS::GetFooterValues()
{
	bkprint::Values values;	
	values.Set(L"PAYDUEDAYS", m_record->payDueDays.ToString());
	
	return std::move(values);
}

bkprint::Values ContractDocument::ContractDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;

	
	return std::move(values);
}

bkprint::Values ContractDocument::ContractDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* ContractDocument::ContractDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;	

	return NULL;
}

std::wstring ContractDocument::ContractDS::FormatHent(const HentRecord* hent)
{
	return TextUtils::Format(L"%s\n%s %s\n%s %s\n%s",hent->name, hent->street, hent->pobox, hent->zip, hent->city,hent->fiscalno);
}

//#######################################################################################

ContractDocument::ContractDocument(bkprint::Template* templ)
{
	m_pTemplate = templ;
	m_pInputInterface = new BuyInvoiceContractInputInterface();
}

bool ContractDocument::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

void ContractDocument::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

bkprint::PrintOut ContractDocument::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;	
	
	ContractHyperDS ds(m_pInputInterface, profile, m_pContext);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	
	return std::move(printOut);
}

ContractDocument::~ContractDocument()
{
	delete m_pInputInterface;
}

//#################################################################################################

ContractInputInterface::~ContractInputInterface()
{
	DeleteRecords();
}

ContractRecordRange ContractInputInterface::GetRecords()
{
	return ContractRecordRange(m_records);
}

bool ContractInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void ContractInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void ContractInputInterface::AddRecord(ContractRecord* record)
{
	m_records.push_back(record);
}

void ContractInputInterface::DeleteRecords()
{
	BOOST_FOREACH(ContractRecord* record, m_records)
	{
		DeepDelete(record->rows);		
		delete record;
	}
	m_records.clear();
}

ContractRecord* ContractInputInterface::CreateRecord(SeasonSession* session,IInvoice* invoice)
{
	ContractRecord* record = new ContractRecord();

	record->buyer.CopyFrom(session->GetFarm());
	record->seller.CopyFrom(invoice->GetInvoiceHent());

	record->transactionDate = invoice->GetTransactionDate();
	if (record->transactionDate.IsNull())
		record->transactionDate = invoice->GetInvoiceDate();

	record->invoiceNo = invoice->GetCustomNumber();
	if (record->invoiceNo.empty())
		record->invoiceNo = IntString(invoice->GetInvoiceNo());

	record->payDueDays = invoice->GetPayDueDays();

	InvoiceCalculator::CalculationResult calculation = InvoiceCalculator::Calculate(invoice);

	
	PtrEnumerator<ICowInvoiceEntry> entries = invoice->EnumEntries();
	while (entries.hasNext())
	{
		ICowInvoiceEntry* entry = *entries;
		ContractRow* row = new ContractRow();
		ICow* cow = session->GetCow(entry->GetCowId())->Object();		
		row->cowNo = cow->GetCowNo();	
		row->paidWeight = entry->GetWeight();
		
		row->grossValue = entry->GetPrice() * (Decimal(1L) + invoice->GetVATRate());

		entries.advance();
		record->rows.push_back(row);
	}

	return record;

}

//#################################################################################################

void BuyInvoiceContractInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = pSession->GetBuyInvoice(invoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();		
		AddRecord(CreateRecord(pSession, pBuyInvoice));
	}
}

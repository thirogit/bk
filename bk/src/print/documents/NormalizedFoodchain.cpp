#include "stdafx.h"
#include "NormalizedFoodchain.h"
#include <boost\foreach.hpp>
#include <string\IntString.h>
#include <arrays\DeepDelete.h>
#include <string/TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"

NormalizedFoodchainOptions::NormalizedFoodchainOptions(IPrintProfile* profile)
{
	if (profile)
	{
		PrintOption* option = profile->GetOption(L"SPIECIE");

		if (option)
		{
			m_spiecies = option->get();
		}
	}
}

const std::wstring& NormalizedFoodchainOptions::speicies()
{
	return m_spiecies;
}

//#######################################################################################

NormalizedFoodchain::HyperNormFoodchainDS::HyperNormFoodchainDS(NormFoodchainInputInterface* inputInterface, NormalizedFoodchainOptions* options, DocumentContext* pContext)
{
	BOOST_FOREACH(const NormalizedFoodchainRecord* record, inputInterface->GetRecords())
	{
		m_dataSources.push_back(new NormFoodchainDS(record, options, pContext));
	}
}

NormalizedFoodchain::HyperNormFoodchainDS::~HyperNormFoodchainDS()
{
	DeepDelete(m_dataSources);
}


int NormalizedFoodchain::HyperNormFoodchainDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* NormalizedFoodchain::HyperNormFoodchainDS::GetDS(int index)
{
	return m_dataSources[index];
}

//#######################################################################################
NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::ItemsBandEvaluator(NormFoodchainDS* parent) : m_parent(parent)
{

}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::Footer()
{
	bkprint::Values values;
	return std::move(values);
}

size_t NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::At(int index)
{
	const NormalizedFoodchainRow* row = m_parent->m_record->rows[index];
	return std::move(CreateRowValues(index, row));
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const NormalizedFoodchainRow* row)
{
	bkprint::Values values;

	std::wstring passNoAndCowNo;
	if (!row->passNo.empty())
	{
		passNoAndCowNo = row->passNo + L", ";
	}

	passNoAndCowNo += row->cowNo.ToString();

	values.Set(L"PASSNOANDCOWNO", passNoAndCowNo);
	
	return std::move(values);
}

//#######################################################################################


NormalizedFoodchain::NormFoodchainDS::NormFoodchainDS(const NormalizedFoodchainRecord* record, NormalizedFoodchainOptions* options, DocumentContext* pContext) :
m_pContext(pContext), m_record(record), m_options(options)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);	
}

NormalizedFoodchain::NormFoodchainDS::~NormFoodchainDS()
{	
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;
	int cowAmount = m_record->rows.size();
	heaaderValues.Set(L"OWNERNAME", m_record->buyFromHent.name);
	heaaderValues.Set(L"OWNERADDRESS", FormatHentAddress(&m_record->buyFromHent));
	//heaaderValues.Set(L"OWNERPHONENO", m_record->buyFromHent.phoneno);
	heaaderValues.Set(L"OWNERHENTNO", m_record->buyFromHent.number);
	
	heaaderValues.Set(L"SPIECIES", m_options->speicies());
	heaaderValues.Set(L"COWAMOUNTNUM", IntString(cowAmount));	
	heaaderValues.Set(L"COWAMOUNTTXT", m_pContext->GetNumberText(cowAmount));

	return std::move(heaaderValues);
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::GetFooterValues()
{
	
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values NormalizedFoodchain::NormFoodchainDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* NormalizedFoodchain::NormFoodchainDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;	

	return NULL;
}

std::wstring NormalizedFoodchain::NormFoodchainDS::FormatHentAddress(const HentRecord* hent)
{
	return TextUtils::Format(L"%s %s\n%s %s", hent->street, hent->pobox, hent->zip, hent->city);
}

//#######################################################################################

NormalizedFoodchain::NormalizedFoodchain(bkprint::Template* templ, NormFoodchainInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool NormalizedFoodchain::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

void NormalizedFoodchain::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

bkprint::PrintOut NormalizedFoodchain::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;	
	NormalizedFoodchainOptions options(profile);	
	HyperNormFoodchainDS ds(m_pInputInterface, &options, m_pContext);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	
	return std::move(printOut);
}

NormalizedFoodchain::~NormalizedFoodchain()
{
}

//#################################################################################################

NormFoodchainInputInterface::~NormFoodchainInputInterface()
{
	DeleteRecords();
}

NormalizedFoodchainRecordRange NormFoodchainInputInterface::GetRecords()
{
	return NormalizedFoodchainRecordRange(m_records);
}

bool NormFoodchainInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void NormFoodchainInputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void NormFoodchainInputInterface::AddRecord(NormalizedFoodchainRecord* record)
{
	m_records.push_back(record);
}

void NormFoodchainInputInterface::DeleteRecords()
{
	BOOST_FOREACH(NormalizedFoodchainRecord* record, m_records)
	{
		DeepDelete(record->rows);		
		delete record;
	}
	m_records.clear();
}

NormalizedFoodchainRecord* NormFoodchainInputInterface::CreateRecord(SeasonSession* session, IInvoice* invoice)
{
	NormalizedFoodchainRecord* record = new NormalizedFoodchainRecord();

	record->buyFromHent.CopyFrom(session->GetFarm());
	
	PtrEnumerator<ICowInvoiceEntry> entries = invoice->EnumEntries();
	while (entries.hasNext())
	{
		ICowInvoiceEntry* entry = *entries;
		NormalizedFoodchainRow* row = new NormalizedFoodchainRow();
		ICow* cow = session->GetCow(entry->GetCowId())->Object();

		row->cowNo = cow->GetCowNo();
		row->passNo = cow->GetPassNo();

		entries.advance();
		record->rows.push_back(row);
	}

	return record;

}

//#################################################################################################

void BuyNormFoodchainInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t invoiceId, range)
	{
		BuyInvoiceClientObject* pBuyInvoiceCO = pSession->GetBuyInvoice(invoiceId);
		BuyInvoice* pBuyInvoice = pBuyInvoiceCO->Object();		
		AddRecord(CreateRecord(pSession,pBuyInvoice));
	}
}

//#################################################################################################


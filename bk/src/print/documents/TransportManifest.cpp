#include "stdafx.h"
#include "TransportManifest.h"
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <arrays\DeepDelete.h>
#include "../../view/FarmHentViewCreator.h"
#include "../../view/HentViewCreator.h"
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"

TransportManifestRecord::TransportManifestRecord() : source(NULL_ID),destination(NULL_ID)
{
}

//#######################################################################################

TransportManifestOptions::TransportManifestOptions(IPrintProfile* profile)
{
	if (!profile) return;

	PrintOptionSerializer optionSerializer;

	PrintOption* spieciesOption = profile->GetOption(L"SPIECIE");
	if (spieciesOption)
	{
		m_spiecies = spieciesOption->get();
	}
}

const std::wstring& TransportManifestOptions::spiecies() const
{
	return m_spiecies;
}


//#######################################################################################


TransportManifest::TransManifDS::ItemsBandEvaluator::ItemsBandEvaluator(TransManifDS* parent) : m_parent(parent)
{

}

bkprint::Values TransportManifest::TransManifDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values TransportManifest::TransManifDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t TransportManifest::TransManifDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values TransportManifest::TransManifDS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values TransportManifest::TransManifDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const TransportManifestRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"PASSNO", row->passNo);
	
	return std::move(values);
}

//#######################################################################################

TransportManifest::TransManifDS::TransManifDS(const TransportManifestRecord* record, TransportManifest* parent, TransportManifestOptions* options) : 
	m_parent(parent), m_record(record), m_options(options)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

TransportManifest::TransManifDS::~TransManifDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values TransportManifest::TransManifDS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"SPIECIES", m_options->spiecies());
	headerValues.Set(L"ANIMALCOUNT", m_parent->m_pContext->GetNumberText(m_record->rows.size()));
	headerValues.Set(L"OWNERNAME", m_record->source.GetName());
	headerValues.Set(L"OWNERADDRESS", FormatAddress(m_record->source));
	headerValues.Set(L"BUYERNAME", m_record->destination.GetName());
	headerValues.Set(L"BUYERADDRESS", FormatAddress(m_record->destination));

	return std::move(headerValues);
}


std::wstring TransportManifest::TransManifDS::FormatAddress(const HentView& hent)
{
	return TextUtils::Format(L"%s %s, %s %s", hent.GetStreet(), hent.GetPOBox(), hent.GetZip(), hent.GetCity());
}

bkprint::Values TransportManifest::TransManifDS::GetFooterValues()
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values TransportManifest::TransManifDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values TransportManifest::TransManifDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* TransportManifest::TransManifDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}
//#######################################################################################

TransportManifest::HyperTransManifDS::HyperTransManifDS(TransportManifestRecordRange records, TransportManifestOptions* options, TransportManifest* parent)
{
	BOOST_FOREACH(const TransportManifestRecord* record, records)
	{
		m_dataSources.push_back(new TransManifDS(record, parent, options));
	}
}

TransportManifest::HyperTransManifDS::~HyperTransManifDS()
{
	DeepDelete(m_dataSources);
}


int TransportManifest::HyperTransManifDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* TransportManifest::HyperTransManifDS::GetDS(int index)
{
	return m_dataSources[index];
}



//#################################################################################################

TransportManifest::TransportManifest(bkprint::Template* templ, TransManifInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool TransportManifest::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}



bkprint::PrintOut TransportManifest::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	TransportManifestOptions options(profile);

	
	HyperTransManifDS ds(m_pInputInterface->GetRecords(), &options, this);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	

	return std::move(printOut);

}

TransportManifest::~TransportManifest()
{
	
}

void TransportManifest::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################
	
TransManifInputInterface::~TransManifInputInterface()
{
	DeleteRecords();
}

void TransManifInputInterface::DeleteRecords()
{
	BOOST_FOREACH(TransportManifestRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

TransportManifestRecordRange TransManifInputInterface::GetRecords()
{
	return TransportManifestRecordRange(m_records);
}

bool TransManifInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void TransManifInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void TransManifInputInterface::AddRecord(TransportManifestRecord* record)
{
	m_records.push_back(record);
}

//#################################################################################################

void MoveDocTransManifInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t moveDocId,range)
	{
		IMoveDoc* pDoc = pSession->GetMoveDoc(moveDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));
	}
}

TransportManifestRecord* MoveDocTransManifInputInterface::CreateRecord(SeasonSession* pSession,IMoveDoc* pDoc)
{
	TransportManifestRecord* record = new TransportManifestRecord();
	FarmHentViewCreator viewCreator(pSession);

	viewCreator.RefreshView(&record->source,pDoc->GetSrcHerdId());
	viewCreator.RefreshView(&record->destination,pDoc->GetDstHerdId());

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		TransportManifestRow* row = new TransportManifestRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();
			
		row->cowNo = cow->GetCowNo();
		row->passNo = cow->GetPassNo();

		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}



//-------------------------------------------------------------------------------------------------
void SellDocTransManifInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t sellDocId,range)
	{		
		ISellDoc* pDoc = pSession->GetSellDoc(sellDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));		
	}
}

TransportManifestRecord* SellDocTransManifInputInterface::CreateRecord(SeasonSession* pSession,ISellDoc* pDoc)
{
	TransportManifestRecord* record = new TransportManifestRecord();

	FarmHentViewCreator farmViewCreator(pSession);
	HentViewCreator hentViewCreator(pSession);

	HentClientObject* pHentCO = pSession->GetHent(pDoc->GetHentId());

	farmViewCreator.RefreshView(&record->source,pDoc->GetHerdId());
	hentViewCreator.RefreshView(&record->destination,pHentCO);	

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		TransportManifestRow* row = new TransportManifestRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();
			
		row->cowNo = cow->GetCowNo();
		row->passNo = cow->GetPassNo();

		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}

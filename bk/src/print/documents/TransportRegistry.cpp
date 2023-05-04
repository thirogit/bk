#include "stdafx.h"
#include "TransportRegistry.h"
#include <boost\foreach.hpp>
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <arrays\DeepDelete.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include "../../view/HentViewCreator.h"
#include "../../text/DocNoText.h"
#include "../../view/FarmHentViewCreator.h"


TransportRegistryOptions::TransportRegistryOptions(IPrintProfile* profile)
{
	m_donFillCarrier = false;
	if (profile)
	{
		PrintOptionSerializer optionSerializer;
		PrintOption* dontFillCarrierOption = profile->GetOption(L"DONOTFILLCARRIER");

		if (dontFillCarrierOption)
		{
			m_donFillCarrier = optionSerializer.GetBooleanValue(*dontFillCarrierOption);
		}

	}
}

bool TransportRegistryOptions::DontFillCarier() const
{
	return m_donFillCarrier;
}




//#################################################################################################


TransportRegistryRow::TransportRegistryRow() : buyFromHent(NULL_ID)
{
}


TransportRegistryRecord::TransportRegistryRecord() : carrier(NULL_ID),destination(NULL_ID)
{
}

//#################################################################################################

TransportRegistry::TransportRegDS::ItemsBandEvaluator::ItemsBandEvaluator(TransportRegDS* parent) : m_parent(parent)
{

}

bkprint::Values TransportRegistry::TransportRegDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values TransportRegistry::TransportRegDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t TransportRegistry::TransportRegDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values TransportRegistry::TransportRegDS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values TransportRegistry::TransportRegDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const TransportRegistryRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"FSTOWNER", FormatFirstOwner(row->buyFromHent.get_ptr()));
	
	return std::move(values);
}


std::wstring TransportRegistry::TransportRegDS::ItemsBandEvaluator::FormatFirstOwner(const HentView* hent)
{
	if (!hent) return L"---";
	return TextUtils::Format(L"%s, %s %s, %s %s, %s",
		hent->GetName(),
		hent->GetStreet(),
		hent->GetPOBox(),
		hent->GetZip(),
		hent->GetCity(),
		hent->GetHentNo().ToString());
	

}

//#######################################################################################

TransportRegistry::HyperTransportRegDS::HyperTransportRegDS(TransportRegistryRecordRange records, const TransportRegistryOptions* options, TransportRegistry* parent)
{
	BOOST_FOREACH(const TransportRegistryRecord* record, records)
	{
		m_dataSources.push_back(new TransportRegDS(record, parent, options));
	}
}

TransportRegistry::HyperTransportRegDS::~HyperTransportRegDS()
{
	DeepDelete(m_dataSources);
}


int TransportRegistry::HyperTransportRegDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* TransportRegistry::HyperTransportRegDS::GetDS(int index)
{
	return m_dataSources[index];
}



//#######################################################################################

TransportRegistry::TransportRegDS::TransportRegDS(const TransportRegistryRecord* record, TransportRegistry* parent, const TransportRegistryOptions* options) : m_parent(parent), m_record(record), m_options(options)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
	
}

TransportRegistry::TransportRegDS::~TransportRegDS()
{
	delete m_itemsBandEvaluator;
}

std::wstring TransportRegistry::TransportRegDS::FormatHent(const HentView* pHent)
{
	return TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
		pHent->GetName().c_str(),
		pHent->GetHentNo().ToString().c_str(),
		pHent->GetStreet().c_str(),
		pHent->GetPOBox().c_str(),
		pHent->GetZip().c_str(),
		pHent->GetCity().c_str());
}

bkprint::Values TransportRegistry::TransportRegDS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"DOCDATE", m_record->docDate.GetDayDate());
	headerValues.Set(L"TRANSPORTDATE", m_record->loadDate.GetDayDate());
	headerValues.Set(L"PLATENO", m_record->plateNo);
	headerValues.Set(L"EXTRAS", m_record->extras);
	headerValues.Set(L"DOCNUMBER", m_record->docNumber);
	if (!m_options->DontFillCarier())
	{
		headerValues.Set(L"CARRIER", FormatHent(&m_record->carrier));
	}
	headerValues.Set(L"DESTINATION", FormatHent(&m_record->destination));

	headerValues.Set(L"LOADSTARTTIME", m_record->loadStartTm.GetTime(Time::HHcolonMI));
	headerValues.Set(L"LOADENDTIME", m_record->loadEndTm.GetTime(Time::HHcolonMI));
	
	return std::move(headerValues);
}

bkprint::Values TransportRegistry::TransportRegDS::GetFooterValues()
{
	return std::move(bkprint::Values());
}

bkprint::Values TransportRegistry::TransportRegDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values TransportRegistry::TransportRegDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* TransportRegistry::TransportRegDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

TransportRegistry::TransportRegistry(bkprint::Template* templ, TransRegInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool TransportRegistry::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}




bkprint::PrintOut TransportRegistry::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;


	TransportRegistryOptions options(profile);


	HyperTransportRegDS ds(m_pInputInterface->GetRecords(), &options, this);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));


	

	return std::move(printOut);

}

void TransportRegistry::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


TransportRegistry::~TransportRegistry()
{
	
}

//#################################################################################################
	
TransRegInputInterface::~TransRegInputInterface()
{
	DeleteRecords();
}

void TransRegInputInterface::DeleteRecords()
{
	BOOST_FOREACH(TransportRegistryRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

TransportRegistryRecordRange TransRegInputInterface::GetRecords()
{
	return TransportRegistryRecordRange(m_records);
}

bool TransRegInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void TransRegInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void TransRegInputInterface::AddRecord(TransportRegistryRecord* record)
{
	m_records.push_back(record);
}

TransportRegistryRecord* TransRegInputInterface::CreateRecord(SeasonSession* pSession,IHentsDoc* pDoc)
{
	TransportRegistryRecord* record = new TransportRegistryRecord();

	record->docNumber = DocNoText(pDoc).ToString();
	record->docDate = pDoc->GetDocDate();
	record->loadDate = pDoc->GetLoadDate();
	record->plateNo = pDoc->GetPlateNo();
	record->extras = pDoc->GetExtras();
	record->loadStartTm = pDoc->GetLoadStartDtTm();
	record->loadEndTm = pDoc->GetLoadEndDtTm();

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		TransportRegistryRow* row = new TransportRegistryRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();

		row->cowNo = cow->GetCowNo();

		uint32_t buyInvoiceId = cow->GetBuyInvoiceId();
		if (buyInvoiceId == NULL_ID)
		{
			ICowTrail* trail = pSession->GetCowTrail(cow->GetId());
			IDoc* entryDoc = trail->GetEntry()->GetDoc();
			if (entryDoc->GetDocType() == DocType_Buy)
			{
				buyInvoiceId = ((BuyDoc*)entryDoc)->GetInvoiceId();
			}
		}
		
		if (buyInvoiceId != NULL_ID)
		{
			BuyInvoiceClientObject* buyInvoiceCO = pSession->GetBuyInvoice(buyInvoiceId);

			HentViewCreator viewCreator(pSession);
			HentView buyFromHent(NULL_ID);
			viewCreator.RefreshView(&buyFromHent, buyInvoiceCO->Object()->GetInvoiceHent());
			row->buyFromHent = buyFromHent;
		
		}
		else
		{
			row->buyFromHent.reset();
		}	

			
		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void BuyDocTransRegInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t buyDocId,range)
	{
		IHentsDoc* pDoc = pSession->GetBuyDoc(buyDocId)->Object();
		
		
		TransportRegistryRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->carrier, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->destination, pDoc->GetHerdId());

		AddRecord(record);

	}
}

//-------------------------------------------------------------------------------------------------
void SellDocTransRegInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t sellDocId,range)
	{		
		IHentsDoc* pDoc = pSession->GetSellDoc(sellDocId)->Object();
		TransportRegistryRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->destination, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->carrier, pDoc->GetHerdId());

		AddRecord(record);
	}
}


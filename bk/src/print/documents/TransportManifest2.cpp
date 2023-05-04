#include "stdafx.h"
#include "TransportManifest2.h"
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <arrays\DeepDelete.h>
#include "../../view/FarmHentViewCreator.h"
#include "../../view/HentViewCreator.h"
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include <data\utils\GetDocInvoice.h>

TransportManifest2Record::TransportManifest2Record() : source(NULL_ID), destination(NULL_ID), srcHerd(NULL_ID)
{
}

//#######################################################################################

TransportManifest2Options::TransportManifest2Options(IPrintProfile* profile)
{
	if (!profile) return;

	PrintOptionSerializer optionSerializer;

	PrintOption* spieciesOption = profile->GetOption(L"SPIECIE");
	if (spieciesOption)
	{
		m_spiecies = spieciesOption->get();
	}
}

const std::wstring& TransportManifest2Options::spiecies() const
{
	return m_spiecies;
}


//#######################################################################################


TransportManifest2::TransManif2DS::ItemsBandEvaluator::ItemsBandEvaluator(TransManif2DS* parent) : m_parent(parent)
{

}

bkprint::Values TransportManifest2::TransManif2DS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values TransportManifest2::TransManif2DS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t TransportManifest2::TransManif2DS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values TransportManifest2::TransManif2DS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values TransportManifest2::TransManif2DS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const TransportManifest2Row* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());

	std::wstring spiecie = row->className;
	if (spiecie.empty())
	{
		spiecie = m_parent->m_options->spiecies();
	}
	values.Set(L"SPIECIES", spiecie);
	//values.Set(L"WEIGHT", row->weight.ToString(3));

	if (row->buyFromHent.is_initialized())
	{
		InvoiceHent buyFromHent = row->buyFromHent.get();		
		values.Set(L"HERDNO", buyFromHent.GetHentNo().ToString());
		values.Set(L"NAMEANDADDRESS", buyFromHent.GetName() + L", " + 
			                          TextUtils::Format(L"%s %s, %s %s", buyFromHent.GetStreet(), buyFromHent.GetPOBox(), buyFromHent.GetZip(), buyFromHent.GetCity()));
	}
	
	
	return std::move(values);
}

//#######################################################################################

TransportManifest2::TransManif2DS::TransManif2DS(const TransportManifest2Record* record, TransportManifest2* parent, TransportManifest2Options* options) : 
	m_parent(parent), m_record(record), m_options(options)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

TransportManifest2::TransManif2DS::~TransManif2DS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values TransportManifest2::TransManif2DS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"SPIECIES", m_options->spiecies());
	headerValues.Set(L"ANIMALCOUNT", m_parent->m_pContext->GetNumberText(m_record->rows.size()));
	headerValues.Set(L"CARRIERNAME", m_record->source.GetName() /*+ L" " + FormatAddress(m_record->source)*/);	
	headerValues.Set(L"HERDADDRESS", TextUtils::Format(L"%s %s, %s %s", m_record->srcHerd.GetStreet(), m_record->srcHerd.GetPOBox(), m_record->srcHerd.GetZipCode(), m_record->srcHerd.GetCity()));
	headerValues.Set(L"CARRIERWETNO", m_record->source.GetWetNo());
	headerValues.Set(L"PLATENO", m_record->plateNo);
	headerValues.Set(L"LOADDATETIME", m_record->loadDate.GetDayDate() + L" " + m_record->loadTime.GetTime(Time::HHcolonMI));	
	
	HentNo herdNo = m_record->source.GetHentNo();
	std::wstring sHerdNo = herdNo.ToString();
	for (int i = 0,l = sHerdNo.length(); i < l;i++)
	{
		headerValues.Set(TextUtils::Format(L"HERDNO[%d]", i), std::wstring(1,sHerdNo.at(i)));		
	}	
		
	return std::move(headerValues);
}


std::wstring TransportManifest2::TransManif2DS::FormatAddress(const HentView& hent)
{
	return TextUtils::Format(L"%s %s, %s %s", hent.GetStreet(), hent.GetPOBox(), hent.GetZip(), hent.GetCity());
}

bkprint::Values TransportManifest2::TransManif2DS::GetFooterValues()
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values TransportManifest2::TransManif2DS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values TransportManifest2::TransManif2DS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* TransportManifest2::TransManif2DS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}


//#################################################################################################

TransportManifest2::TransportManifest2(bkprint::Template* templ, TransManif2InputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

bool TransportManifest2::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}



bkprint::PrintOut TransportManifest2::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	TransportManifest2Options options(profile);

	BOOST_FOREACH(const TransportManifest2Record* record, m_pInputInterface->GetRecords())
	{
		TransManif2DS ds(record, this, &options);
		bkprint::TemplateRenderer renderer(m_pTemplate);
		printOut = std::move(renderer.Print(&ds));
	}

	return std::move(printOut);

}

TransportManifest2::~TransportManifest2()
{
	
}

void TransportManifest2::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################
	
TransManif2InputInterface::~TransManif2InputInterface()
{
	DeleteRecords();
}

void TransManif2InputInterface::DeleteRecords()
{
	BOOST_FOREACH(TransportManifest2Record* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

TransportManifest2RecordRange TransManif2InputInterface::GetRecords()
{
	return TransportManifest2RecordRange(m_records);
}

bool TransManif2InputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void TransManif2InputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void TransManif2InputInterface::AddRecord(TransportManifest2Record* record)
{
	m_records.push_back(record);
}

//#################################################################################################

void MoveDocTransManif2InputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t moveDocId,range)
	{
		IMoveDoc* pDoc = pSession->GetMoveDoc(moveDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));
	}
}

TransportManifest2Record* MoveDocTransManif2InputInterface::CreateRecord(SeasonSession* pSession,IMoveDoc* pDoc)
{
	TransportManifest2Record* record = new TransportManifest2Record();
	FarmHentViewCreator viewCreator(pSession);

	viewCreator.RefreshView(&record->source,pDoc->GetSrcHerdId());
	viewCreator.RefreshView(&record->destination,pDoc->GetDstHerdId());

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		TransportManifest2Row* row = new TransportManifest2Row();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();			
		row->cowNo = cow->GetCowNo();

		uint32_t classId = cow->GetClassId();		
		IClass* cowClass = pSession->GetClass(classId);
		if (cowClass != NULL)
		{
			row->className = cowClass->GetClassNm();
		}

		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}



//-------------------------------------------------------------------------------------------------
void SellDocTransManif2InputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t sellDocId,range)
	{		
		ISellDoc* pDoc = pSession->GetSellDoc(sellDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));		
	}
}

TransportManifest2Record* SellDocTransManif2InputInterface::CreateRecord(SeasonSession* pSession,ISellDoc* pDoc)
{
	TransportManifest2Record* record = new TransportManifest2Record();

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
		TransportManifest2Row* row = new TransportManifest2Row();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();
			
		row->cowNo = cow->GetCowNo();
		row->weight = entry->GetWeight().ToDecimal(cow->GetWeight());
		
		uint32_t classId = cow->GetClassId();
		IClass* cowClass = pSession->GetClass(classId);
		if (cowClass != NULL)
		{
			row->className = cowClass->GetClassNm();
		}

		uint32_t buyInvoiceId = cow->GetBuyInvoiceId();
		BuyInvoice* buyInvoice = NULL;

		if (buyInvoiceId != NULL_ID)
		{
			buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
		}
		else
		{
			ICowTrail* trail = pSession->GetCowTrail(cow->GetId());
			const ICowFootprint* entryFootprint = trail->GetEntry();
			IDoc* entryDoc = entryFootprint->GetDoc();
			buyInvoiceId = GetDocInvoice(entryDoc);
			if (buyInvoiceId != NULL_ID)
			{
				buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
			}
		}
		if (buyInvoice != NULL)
		{
			InvoiceHent invoicehent;
			invoicehent.CopyFrom(buyInvoice->GetInvoiceHent());
			row->buyFromHent = invoicehent;
		}

		record->rows.push_back(row);
		record->loadDate = pDoc->GetLoadDate();
		record->loadTime = pDoc->GetLoadStartDtTm();
		record->plateNo = pDoc->GetPlateNo();
		record->srcHerd.CopyFrom(pSession->GetHerd(pDoc->GetHerdId()));
		
		cowsEnum.advance();
	}

	return record;
}

//-------------------------------------------------------------------------------------------------
void BuyDocTransManif2InputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t docId, range)
	{
		IBuyDoc* pDoc = pSession->GetBuyDoc(docId)->Object();
		AddRecord(CreateRecord(pSession, pDoc));
	}
}

TransportManifest2Record* BuyDocTransManif2InputInterface::CreateRecord(SeasonSession* pSession, IBuyDoc* pDoc)
{
	TransportManifest2Record* record = new TransportManifest2Record();

	FarmHentViewCreator farmViewCreator(pSession);
	HentViewCreator hentViewCreator(pSession);

	HentClientObject* pHentCO = pSession->GetHent(pDoc->GetHentId());

	farmViewCreator.RefreshView(&record->source, pDoc->GetHerdId());
	hentViewCreator.RefreshView(&record->destination, pHentCO);

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while (cowsEnum.hasNext())
	{
		TransportManifest2Row* row = new TransportManifest2Row();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();

		row->cowNo = cow->GetCowNo();
		row->weight = entry->GetWeight().ToDecimal(cow->GetWeight());

		uint32_t classId = cow->GetClassId();
		IClass* cowClass = pSession->GetClass(classId);
		if (cowClass != NULL)
		{
			row->className = cowClass->GetClassNm();
		}

		uint32_t buyInvoiceId = cow->GetBuyInvoiceId();
		BuyInvoice* buyInvoice = NULL;

		if (buyInvoiceId != NULL_ID)
		{
			buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
		}
		else
		{
			ICowTrail* trail = pSession->GetCowTrail(cow->GetId());
			const ICowFootprint* entryFootprint = trail->GetEntry();
			IDoc* entryDoc = entryFootprint->GetDoc();
			buyInvoiceId = GetDocInvoice(entryDoc);
			if (buyInvoiceId != NULL_ID)
			{
				buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
			}
		}
		if (buyInvoice != NULL)
		{
			InvoiceHent invoicehent;
			invoicehent.CopyFrom(buyInvoice->GetInvoiceHent());
			row->buyFromHent = invoicehent;
		}

		record->rows.push_back(row);
		record->loadDate = pDoc->GetLoadDate();
		record->loadTime = pDoc->GetLoadStartDtTm();
		record->plateNo = pDoc->GetPlateNo();
		record->srcHerd.CopyFrom(pSession->GetHerd(pDoc->GetHerdId()));

		cowsEnum.advance();
	}

	return record;
}

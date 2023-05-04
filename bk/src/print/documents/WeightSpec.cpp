#include "stdafx.h"
#include "WeightSpec.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string\TextUtils.h>
#include "../../text/CowSexText.h"
#include <string\IntString.h>
#include <utils\SafeGet.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../text/DocNoText.h"
#include "../../view/HentViewCreator.h"
#include "../../view/FarmHentViewCreator.h"
#include "../options/PrintOptionSerializer.h"

WeightSpecOptions::WeightSpecOptions(IPrintProfile* profile)
{
	m_DontFillDeliver = false;
	if (profile)
	{
		PrintOptionSerializer serializer;
		PrintOption* dontFillDeliverOption = profile->GetOption(L"DONOTFILLDELIVER");
		if (dontFillDeliverOption)
			m_DontFillDeliver = serializer.GetBooleanValue(*dontFillDeliverOption);
	}
}

bool WeightSpecOptions::GetDontFillDeliver()
{
	return m_DontFillDeliver;
}

//#################################################################################################

WeightSpecRecord::WeightSpecRecord() : fromHent(NULL_ID), toHent(NULL_ID)
{

}

//#################################################################################################

WeightSpecification::WeightSpecDS::ItemsBandEvaluator::ItemsBandEvaluator(WeightSpecDS* parent) : m_parent(parent)
{

}

bkprint::Values WeightSpecification::WeightSpecDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values WeightSpecification::WeightSpecDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t WeightSpecification::WeightSpecDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values WeightSpecification::WeightSpecDS::ItemsBandEvaluator::At(int index)
{	
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values WeightSpecification::WeightSpecDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const WeightSpecRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"COWCLASS", row->classCd);
	values.Set(L"COWSEX", CowSexText(row->sex).ToString());
	values.Set(L"WEIGHTDECLARED", row->weight.ToString(3));//GetWeightFormat()));	

	return std::move(values);
}

//#######################################################################################

WeightSpecification::WeightSpecDS::WeightSpecDS(const WeightSpecRecord* record, WeightSpecification* parent, IPrintProfile* profile) : m_parent(parent), m_record(record), m_options(profile)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

WeightSpecification::WeightSpecDS::~WeightSpecDS()
{
	delete m_itemsBandEvaluator;
}



std::wstring WeightSpecification::WeightSpecDS::FormatHent(const HentView* pHent)
{
	return TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
		pHent->GetName().c_str(),
		pHent->GetHentNo().ToString().c_str(),
		pHent->GetStreet().c_str(),
		pHent->GetPOBox().c_str(),
		pHent->GetZip().c_str(),
		pHent->GetCity().c_str());
}

bkprint::Values WeightSpecification::WeightSpecDS::GetHeaderValues()
{
	bkprint::Values headerValues;

	headerValues.Set(L"DOCDATE", m_record->docDate.GetDayDate());
	headerValues.Set(L"TRANSPORTDATE", m_record->loadDate.GetDayDate());
	headerValues.Set(L"PLATENO", m_record->plateNo);
	headerValues.Set(L"EXTRAS", m_record->extras);
	headerValues.Set(L"DOCNUMBER", m_record->docNumber);

	headerValues.Set(L"LOADSTARTTIME", m_record->loadTm.GetTime(Time::HHcolonMI));
	headerValues.Set(L"LOADENDTIME", m_record->unloadTm.GetTime(Time::HHcolonMI));

	if (!m_options.GetDontFillDeliver())
	{
		headerValues.Set(L"SOURCE", FormatHent(&m_record->fromHent));
	}
	headerValues.Set(L"DESTINATION", FormatHent(&m_record->toHent));
	
	return std::move(headerValues);
}

bkprint::Values WeightSpecification::WeightSpecDS::GetFooterValues()
{
	return std::move(bkprint::Values());
}

bkprint::Values WeightSpecification::WeightSpecDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values WeightSpecification::WeightSpecDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* WeightSpecification::WeightSpecDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

WeightSpecification::WeightSpecification(bkprint::Template* templ, WeightSpecInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

WeightSpecification::~WeightSpecification()
{

}


bool WeightSpecification::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut WeightSpecification::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	BOOST_FOREACH(const WeightSpecRecord* record, m_pInputInterface->GetRecords())
	{
		WeightSpecDS ds(record,this, profile);
		bkprint::TemplateRenderer renderer(m_pTemplate);
		printOut = std::move(renderer.Print(&ds));
	}

	return std::move(printOut);

}

void WeightSpecification::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################

WeightSpecInputInterface::~WeightSpecInputInterface()
{
	DeleteRecords();
}

void WeightSpecInputInterface::DeleteRecords()
{
	BOOST_FOREACH(WeightSpecRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

size_t WeightSpecInputInterface::RecordCount() const
{
	return m_records.size();
}

const WeightSpecRecord* WeightSpecInputInterface::RecordAt(int index)
{
	return m_records[index];
}

WeightSpecRecordRange WeightSpecInputInterface::GetRecords()
{
	return WeightSpecRecordRange(m_records);
}

bool WeightSpecInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void WeightSpecInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void WeightSpecInputInterface::AddRecord(WeightSpecRecord* record)
{
	m_records.push_back(record);
}

WeightSpecRecord* WeightSpecInputInterface::CreateRecord(SeasonSession* pSession,IDoc* pDoc)
{
	WeightSpecRecord* record = new WeightSpecRecord();
		
	record->docNumber = DocNoText(pDoc).ToString();
	record->docDate = pDoc->GetDocDate();
	record->loadDate = pDoc->GetLoadDate();
	record->plateNo = pDoc->GetPlateNo();
	record->extras = pDoc->GetExtras();
	record->loadTm = pDoc->GetLoadStartDtTm();
	record->unloadTm = pDoc->GetLoadEndDtTm();

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		WeightSpecRow* row = new WeightSpecRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();

		IClass* pOutClass = pSession->GetClass(entry->GetClassId());
		if(!pOutClass)
		{
			pOutClass = pSession->GetClass(cow->GetClassId());
		}

		row->classCd = SafeGet(&IClass::GetClassCode,pOutClass,L"");
		row->cowNo = cow->GetCowNo();
		
		row->sex = cow->GetSex();	
		
		row->weight = entry->GetWeight().ToDecimal(cow->GetWeight());
		
		record->rows.push_back(row);

		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void BuyDocWeightSpecInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{

	HentViewCreator hentViewCreator(pSession);	
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId,range)
	{
		IHentsDoc* pDoc = pSession->GetBuyDoc(docId)->Object();
		WeightSpecRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->fromHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->toHent, pDoc->GetHerdId());

		AddRecord(record);

		

	}
}

//-------------------------------------------------------------------------------------------------
void SellDocWeightSpecInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId, range)
	{
		IHentsDoc* pDoc = pSession->GetSellDoc(docId)->Object();
		WeightSpecRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->toHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->fromHent, pDoc->GetHerdId());
		AddRecord(record);
	}
}


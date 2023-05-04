#include "stdafx.h"
#include "HentsDocSpecification.h"
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

HentsDocSpecificationRecord::HentsDocSpecificationRecord() : fromHent(NULL_ID),toHent(NULL_ID)
{

}

//#################################################################################################

HentsDocSpecification::HyperHentsDocSpecDS::HyperHentsDocSpecDS(HentsDocSpecRecordRange records, HentsDocSpecification* parent)
{
	BOOST_FOREACH(const HentsDocSpecificationRecord* record, records)
	{
		m_dataSources.push_back(new HentsDocSpecDS(record, parent,NULL));
	}
}

HentsDocSpecification::HyperHentsDocSpecDS::~HyperHentsDocSpecDS()
{
	DeepDelete(m_dataSources);
}


int HentsDocSpecification::HyperHentsDocSpecDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* HentsDocSpecification::HyperHentsDocSpecDS::GetDS(int index)
{
	return m_dataSources[index];
}


//#################################################################################################

HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::ItemsBandEvaluator(HentsDocSpecDS* parent) : m_parent(parent)
{

}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::At(int index)
{	
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const HentsDocSpecificationRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"COWCLASS", row->classCd);
	values.Set(L"COWSEX", CowSexText(row->sex).ToString());
	values.Set(L"WEIGHT", row->weight.ToString(3));//GetWeightFormat()));
	values.Set(L"GRP", row->group.ToString());

	return std::move(values);
}

//#######################################################################################

HentsDocSpecification::HentsDocSpecDS::HentsDocSpecDS(const HentsDocSpecificationRecord* record, HentsDocSpecification* parent, IPrintProfile* profile) : m_parent(parent), m_record(record)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

HentsDocSpecification::HentsDocSpecDS::~HentsDocSpecDS()
{
	delete m_itemsBandEvaluator;
}



std::wstring HentsDocSpecification::HentsDocSpecDS::FormatHent(const HentView* pHent)
{
	return TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
		pHent->GetName().c_str(),
		pHent->GetHentNo().ToString().c_str(),
		pHent->GetStreet().c_str(),
		pHent->GetPOBox().c_str(),
		pHent->GetZip().c_str(),
		pHent->GetCity().c_str());
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	heaaderValues.Set(L"DOCDATE", m_record->docDate.GetDayDate());
	heaaderValues.Set(L"LOADDATE", m_record->loadDate.GetDayDate());
	heaaderValues.Set(L"PLATENO", m_record->plateNo);
	heaaderValues.Set(L"EXTRAS", m_record->extras);
	heaaderValues.Set(L"DOCNUMBER", m_record->docNumber);

	heaaderValues.Set(L"SELLER", FormatHent(&m_record->fromHent));
	heaaderValues.Set(L"HENT", FormatHent(&m_record->toHent));

	
	return std::move(heaaderValues);
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::GetFooterValues()
{
	bkprint::Values values;
	values.Set(L"TOTALWEIGHT", m_record->totalWeight.ToString(3));
	return std::move(values);
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values HentsDocSpecification::HentsDocSpecDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* HentsDocSpecification::HentsDocSpecDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

HentsDocSpecification::HentsDocSpecification(bkprint::Template* templ, HentsDocSpecInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

HentsDocSpecification::~HentsDocSpecification()
{

}


bool HentsDocSpecification::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut HentsDocSpecification::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	
	HyperHentsDocSpecDS ds(m_pInputInterface->GetRecords(), this);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	

	return std::move(printOut);

}

void HentsDocSpecification::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################

HentsDocSpecInputInterface::~HentsDocSpecInputInterface()
{
	DeleteRecords();
}

void HentsDocSpecInputInterface::DeleteRecords()
{
	BOOST_FOREACH(HentsDocSpecificationRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

size_t HentsDocSpecInputInterface::RecordCount() const
{
	return m_records.size();
}

const HentsDocSpecificationRecord* HentsDocSpecInputInterface::RecordAt(int index)
{
	return m_records[index];
}

HentsDocSpecRecordRange HentsDocSpecInputInterface::GetRecords()
{
	return HentsDocSpecRecordRange(m_records);
}

bool HentsDocSpecInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void HentsDocSpecInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void HentsDocSpecInputInterface::AddRecord(HentsDocSpecificationRecord* record)
{
	m_records.push_back(record);
}

HentsDocSpecificationRecord* HentsDocSpecInputInterface::CreateRecord(SeasonSession* pSession,IHentsDoc* pDoc)
{
	HentsDocSpecificationRecord* record = new HentsDocSpecificationRecord();
		
	record->docNumber = DocNoText(pDoc).ToString();
	record->docDate = pDoc->GetDocDate();
	record->loadDate = pDoc->GetLoadDate();
	record->plateNo = pDoc->GetPlateNo();
	record->extras = pDoc->GetExtras();

	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		HentsDocSpecificationRow* row = new HentsDocSpecificationRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();

		IClass* pOutClass = pSession->GetClass(entry->GetClassId());
		if(!pOutClass)
		{
			pOutClass = pSession->GetClass(cow->GetClassId());
		}

		row->classCd = SafeGet(&IClass::GetClassCode,pOutClass,L"");
		row->cowNo = cow->GetCowNo();
		row->group = entry->GetGroup();
		row->sex = cow->GetSex();

		Decimal weight = entry->GetWeight().ToDecimal(cow->GetWeight());
		record->totalWeight += weight;
		row->weight = weight;
		record->rows.push_back(row);

		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void BuyDocSpecInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{

	HentViewCreator hentViewCreator(pSession);	
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId,range)
	{
		IHentsDoc* pDoc = pSession->GetBuyDoc(docId)->Object();
		HentsDocSpecificationRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->fromHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->toHent, pDoc->GetHerdId());

		AddRecord(record);

		

	}
}

//-------------------------------------------------------------------------------------------------
void SellDocSpecInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId, range)
	{
		IHentsDoc* pDoc = pSession->GetSellDoc(docId)->Object();
		HentsDocSpecificationRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->toHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->fromHent, pDoc->GetHerdId());
		AddRecord(record);
	}
}


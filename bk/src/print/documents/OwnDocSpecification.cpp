#include "stdafx.h"
#include "OwnDocSpecification.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string\TextUtils.h>
#include "../../text/CowSexText.h"
#include <string\IntString.h>
#include <utils\SafeGet.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../text/DocNoText.h"

OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::ItemsBandEvaluator(OwnDocSpecDS* parent) : m_parent(parent)
{

}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const OwnDocSpecificationRow* row)
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

//#################################################################################################

OwnDocSpecification::HyperOwnDocSpecDS::HyperOwnDocSpecDS(OwnDocSpecRecordRange records, OwnDocSpecification* parent)
{
	BOOST_FOREACH(const OwnDocSpecificationRecord* record, records)
	{
		m_dataSources.push_back(new OwnDocSpecDS(record, parent, NULL));
	}
}

OwnDocSpecification::HyperOwnDocSpecDS::~HyperOwnDocSpecDS()
{
	DeepDelete(m_dataSources);
}


int OwnDocSpecification::HyperOwnDocSpecDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* OwnDocSpecification::HyperOwnDocSpecDS::GetDS(int index)
{
	return m_dataSources[index];
}


//#######################################################################################

OwnDocSpecification::OwnDocSpecDS::OwnDocSpecDS(const OwnDocSpecificationRecord* record, OwnDocSpecification* parent, IPrintProfile* profile) : m_parent(parent), m_record(record)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

OwnDocSpecification::OwnDocSpecDS::~OwnDocSpecDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	heaaderValues.Set(L"DOCDATE", m_record->docDate.GetDayDate());
	heaaderValues.Set(L"LOADDATE", m_record->loadDate.GetDayDate());
	heaaderValues.Set(L"PLATENO", m_record->plateNo);
	heaaderValues.Set(L"EXTRAS", m_record->extras);
	heaaderValues.Set(L"DOCNUMBER", m_record->docNumber);
	
	return std::move(heaaderValues);
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::GetFooterValues()
{
	bkprint::Values values;
	values.Set(L"TOTALWEIGHT", m_record->totalWeight.ToString(3));
	return std::move(values);
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values OwnDocSpecification::OwnDocSpecDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* OwnDocSpecification::OwnDocSpecDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################


OwnDocSpecification::OwnDocSpecification(bkprint::Template* templ, OwnDocSpecInputInterface* inputInterface) 
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

OwnDocSpecification::~OwnDocSpecification()
{
}


bool OwnDocSpecification::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut OwnDocSpecification::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	HyperOwnDocSpecDS ds(m_pInputInterface->GetRecords(), this);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));


	return std::move(printOut);

}

void OwnDocSpecification::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

//#################################################################################################

OwnDocSpecInputInterface::~OwnDocSpecInputInterface()
{
	DeleteRecords();
}

void OwnDocSpecInputInterface::DeleteRecords()
{
	BOOST_FOREACH(OwnDocSpecificationRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

OwnDocSpecRecordRange OwnDocSpecInputInterface::GetRecords()
{
	return OwnDocSpecRecordRange(m_records);
}

bool OwnDocSpecInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void OwnDocSpecInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void OwnDocSpecInputInterface::AddRecord(OwnDocSpecificationRecord* record)
{
	m_records.push_back(record);
}

OwnDocSpecificationRecord* OwnDocSpecInputInterface::CreateRecord(SeasonSession* pSession,IOwnDoc* pDoc)
{
	OwnDocSpecificationRecord* record = new OwnDocSpecificationRecord();

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
		OwnDocSpecificationRow* row = new OwnDocSpecificationRow();
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
		row->weight = entry->GetWeight().ToDecimal(cow->GetWeight());
		record->totalWeight += row->weight;
		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void InDocSpecInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t inDocId,range)
	{
		IOwnDoc* pDoc = pSession->GetInDoc(inDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));
	}
}

//-------------------------------------------------------------------------------------------------
void OutDocSpecInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t outDocId,range)
	{		
		IOwnDoc* pDoc = pSession->GetOutDoc(outDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));		
	}
}


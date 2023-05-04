#include "stdafx.h"
#include "HealthCertAttachment.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <string\IntString.h>
#include "../../bkprint/renderer/TemplateRenderer.h"


HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::ItemsBandEvaluator(HealthCertAttachmentDS* parent) : m_parent(parent)
{

}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const HealthCertAttachmentRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"PASSNO", row->passportNo);
	

	return std::move(values);
}

//#######################################################################################

HealthCertAttachment::HealthCertAttachmentDS::HealthCertAttachmentDS(const HealthCertAttachmentRecord* record, IPrintProfile* profile, DocumentContext* pContext) : m_pContext(pContext), m_record(record)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

HealthCertAttachment::HealthCertAttachmentDS::~HealthCertAttachmentDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::GetHeaderValues()
{
	bkprint::Values headerValues;
	return std::move(headerValues);
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::GetFooterValues()
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values HealthCertAttachment::HealthCertAttachmentDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* HealthCertAttachment::HealthCertAttachmentDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################

HealthCertAttachment::HyperHealthCertAttachmentDS::HyperHealthCertAttachmentDS(HealthCertAttachInputInterface* inputInterface, IPrintProfile* profile, DocumentContext* pContext)
{
	BOOST_FOREACH(const HealthCertAttachmentRecord* record, inputInterface->GetRecords())
	{
		m_dataSources.push_back(new HealthCertAttachmentDS(record, profile, pContext));
	}
}

HealthCertAttachment::HyperHealthCertAttachmentDS::~HyperHealthCertAttachmentDS()
{
	DeepDelete(m_dataSources);
}


int HealthCertAttachment::HyperHealthCertAttachmentDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* HealthCertAttachment::HyperHealthCertAttachmentDS::GetDS(int index)
{
	return m_dataSources[index];
}


//#######################################################################################


HealthCertAttachment::HealthCertAttachment(bkprint::Template* templ, HealthCertAttachInputInterface* inputInterface) 
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

HealthCertAttachment::~HealthCertAttachment()
{
}


bool HealthCertAttachment::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut HealthCertAttachment::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	HyperHealthCertAttachmentDS ds(m_pInputInterface, profile, m_pContext);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}

void HealthCertAttachment::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

//#################################################################################################

HealthCertAttachInputInterface::~HealthCertAttachInputInterface()
{
	DeleteRecords();
}

void HealthCertAttachInputInterface::DeleteRecords()
{
	BOOST_FOREACH(HealthCertAttachmentRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

HealthCertAttachmentRecordRange HealthCertAttachInputInterface::GetRecords()
{
	return HealthCertAttachmentRecordRange(m_records);
}

bool HealthCertAttachInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void HealthCertAttachInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void HealthCertAttachInputInterface::AddRecord(HealthCertAttachmentRecord* record)
{
	m_records.push_back(record);
}

HealthCertAttachmentRecord* HealthCertAttachInputInterface::CreateRecord(SeasonSession* pSession,IDoc* pDoc)
{
	HealthCertAttachmentRecord* record = new HealthCertAttachmentRecord();
	
	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		HealthCertAttachmentRow* row = new HealthCertAttachmentRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();
				
		row->cowNo = cow->GetCowNo();
		row->passportNo = cow->GetPassNo();
		record->rows.push_back(row);		
		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void SellDocHealthCertInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t inDocId,range)
	{
		ISellDoc* pDoc = pSession->GetSellDoc(inDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));
	}
}

//-------------------------------------------------------------------------------------------------
void OutDocHealthCertInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t outDocId,range)
	{		
		IOutDoc* pDoc = pSession->GetOutDoc(outDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));		
	}
}


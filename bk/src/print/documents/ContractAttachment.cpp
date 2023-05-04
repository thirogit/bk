#include "stdafx.h"
#include "ContractAttachment.h"
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

ContractAttachmentOptions::ContractAttachmentOptions(IPrintProfile* profile)
{
	if (profile)
	{
		PrintOption* contractDtOption = profile->GetOption(L"CONTRACTDT");
		PrintOptionSerializer serializer;
		if (contractDtOption)
			m_contractDt = serializer.GetDateTimeValue(*contractDtOption);

		PrintOption* deliveryDtOption = profile->GetOption(L"DELIVERYDT");
		if (deliveryDtOption)
			m_deliveryDt = serializer.GetDateTimeValue(*deliveryDtOption);

		PrintOption* farmerOption = profile->GetOption(L"FARMER");
		if (farmerOption)
			m_farmer = serializer.GetStringValue(*farmerOption);

	}
}

const std::wstring& ContractAttachmentOptions::GetFarmer() const
{
	return m_farmer;
}
const DateTime& ContractAttachmentOptions::GetContractDate() const
{
	return m_contractDt;
}

const DateTime& ContractAttachmentOptions::GetDeliveryDate() const
{
	return m_deliveryDt;
}


//#################################################################################################

ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::ItemsBandEvaluator(ContractAttachmentDS* parent) : m_parent(parent)
{

}

bkprint::Values ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::Footer()
{
	bkprint::Values values;
	values.Set(L"TOTALWEIGHT", m_parent->m_record->totalWeight.ToString(3));
	return std::move(values);
}

size_t ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_record->rows.size();
}

bkprint::Values ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::At(int index)
{
	return std::move(CreateRowValues(index, m_parent->m_record->rows[index]));
}

bkprint::Values ContractAttachment::ContractAttachmentDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const ContractAttachmentRow* row)
{
	bkprint::Values values;

	values.Set(L"INDEX", IntString(nindex + 1));
	values.Set(L"COWNO", row->cowNo.ToString());
	values.Set(L"PASSNO", row->passportNo);
	values.Set(L"COWSTOCK", row->stockCd);
	values.Set(L"COWSEX", CowSexText(row->sex).ToString());
	values.Set(L"WEIGHT", row->weight.ToString(m_parent->m_parent->m_pContext->GetWeightFormat()));	

	return std::move(values);
}

//#################################################################################################

ContractAttachment::HyperContractAttachmentDS::HyperContractAttachmentDS(ContractAttachmentRecordRange records, ContractAttachment* parent, ContractAttachmentOptions* options)
{
	BOOST_FOREACH(const ContractAttachmentRecord* record, records)
	{
		m_dataSources.push_back(new ContractAttachmentDS(record, parent, options));
	}
}

ContractAttachment::HyperContractAttachmentDS::~HyperContractAttachmentDS()
{
	DeepDelete(m_dataSources);
}


int ContractAttachment::HyperContractAttachmentDS::GetDSCount()
{
	return m_dataSources.size();
}

bkprint::ExpandableTemplateDataSource* ContractAttachment::HyperContractAttachmentDS::GetDS(int index)
{
	return m_dataSources[index];
}


//#######################################################################################

ContractAttachment::ContractAttachmentDS::ContractAttachmentDS(const ContractAttachmentRecord* record, ContractAttachment* parent, ContractAttachmentOptions* options) : m_parent(parent), m_record(record), m_options(options)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

ContractAttachment::ContractAttachmentDS::~ContractAttachmentDS()
{
	delete m_itemsBandEvaluator;
}

std::wstring ContractAttachment::ContractAttachmentDS::FormatHent(const HentView* pHent)
{
	return L"";
}


bkprint::Values ContractAttachment::ContractAttachmentDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	DateTime contractDt = m_options->GetContractDate();
	if (contractDt.IsNull())
	{
		contractDt = m_record->docDate;
	}

	DateTime deliveryDt = m_options->GetDeliveryDate();
	if (deliveryDt.IsNull())
	{
		deliveryDt = m_record->loadDate;
	}

	heaaderValues.Set(L"CONTRACTDT", contractDt.GetDayDate());
	heaaderValues.Set(L"DELIVERYDT", deliveryDt.GetDayDate());
	
	heaaderValues.Set(L"COMPANY_NAME", m_record->company->GetName());
	heaaderValues.Set(L"COMPANY_STREET", m_record->company->GetStreet());
	heaaderValues.Set(L"COMPANY_POBOX", m_record->company->GetPOBox());
	heaaderValues.Set(L"COMPANY_ZIPCODE", m_record->company->GetZip());
	heaaderValues.Set(L"COMPANY_CITY", m_record->company->GetCity());
	heaaderValues.Set(L"COMPANY_FISCALNO", m_record->company->GetFiscalNo());

	std::wstring farmer = m_options->GetFarmer();

	if (farmer.empty())
	{
		farmer = m_record->farmer;
	}

	heaaderValues.Set(L"FARMER", farmer);
	
	return std::move(heaaderValues);
}

bkprint::Values ContractAttachment::ContractAttachmentDS::GetFooterValues()
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values ContractAttachment::ContractAttachmentDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values ContractAttachment::ContractAttachmentDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	values.Set(L"PAGE", IntString(page));
	values.Set(L"PAGECOUNT", IntString(pageCount));
	return std::move(values);
}

bkprint::BandEvaluator* ContractAttachment::ContractAttachmentDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}

//#######################################################################################


ContractAttachment::ContractAttachment(bkprint::Template* templ, ContractAttachmentInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

ContractAttachment::~ContractAttachment()
{
}


bool ContractAttachment::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut ContractAttachment::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	ContractAttachmentOptions options(profile);
	HyperContractAttachmentDS ds(m_pInputInterface->GetRecords(), this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));


	return std::move(printOut);

}

void ContractAttachment::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

//#################################################################################################

ContractAttachmentInputInterface::~ContractAttachmentInputInterface()
{
	DeleteRecords();
}

void ContractAttachmentInputInterface::DeleteRecords()
{
	BOOST_FOREACH(ContractAttachmentRecord* record,m_records)
	{
		DeepDelete(record->rows);
		delete record;
	}
	m_records.clear();
}

ContractAttachmentRecordRange ContractAttachmentInputInterface::GetRecords()
{
	return ContractAttachmentRecordRange(m_records);
}

bool ContractAttachmentInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void ContractAttachmentInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void ContractAttachmentInputInterface::AddRecord(ContractAttachmentRecord* record)
{
	m_records.push_back(record);
}

ContractAttachmentRecord* ContractAttachmentInputInterface::CreateRecord(SeasonSession* pSession,IOwnDoc* pDoc)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	ContractAttachmentRecord* record = new ContractAttachmentRecord();

	record->docDate = pDoc->GetDocDate();
	record->loadDate = pDoc->GetLoadDate();
	record->company = farmViewCreator.CreateView(pDoc->GetHerdId());
	record->farmer = pDoc->GetExtras();
	
	ICow* cow;
	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		ContractAttachmentRow* row = new ContractAttachmentRow();
		entry = *cowsEnum;

		cow = pSession->GetCow(entry->GetCowId())->Object();

		IStock* pOutStock = pSession->GetStock(entry->GetStockId());
		if(!pOutStock)
		{
			pOutStock = pSession->GetStock(cow->GetStockId());
		}

		row->stockCd = SafeGet(&IStock::GetStockCode, pOutStock,L"");
		row->cowNo = cow->GetCowNo();
		row->passportNo = cow->GetPassNo();
		row->sex = cow->GetSex();
		row->weight = entry->GetWeight().ToDecimal(cow->GetWeight());
		record->totalWeight += row->weight;
		record->rows.push_back(row);
		cowsEnum.advance();
	}

	return record;
}

//#################################################################################################

void OutDocContractAttachmentInputInterface::Consume(SeasonSession* pSession,const UniqueIds& range)
{
	BOOST_FOREACH(uint32_t outDocId,range)
	{		
		IOwnDoc* pDoc = pSession->GetOutDoc(outDocId)->Object();
		AddRecord(CreateRecord(pSession,pDoc));		
	}
}


#include "stdafx.h"
#include "Disinfection.h"
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

DisinfectionRecord::DisinfectionRecord() : fromHent(NULL_ID), toHent(NULL_ID), animalCount(0)
{

}

//#######################################################################################

DisinfectionOptions::DisinfectionOptions(IPrintProfile* profile) : m_dontPutPlateNo(false)
{
	if (!profile) return;

	PrintOptionSerializer optionSerializer;

	PrintOption* agentOption = profile->GetOption(L"CHEMAGENT");
	if (agentOption)
	{
		m_chemicalAgent = agentOption->get();
	}

	PrintOption* plateNoOption = profile->GetOption(L"NOPLATENO");
	if (plateNoOption)
	{
		m_dontPutPlateNo = optionSerializer.GetBooleanValue(*plateNoOption);
	}
}

const std::wstring& DisinfectionOptions::chemicalAgent() const
{
	return m_chemicalAgent;
}

bool DisinfectionOptions::dontPutPlateNo() const
{
	return m_dontPutPlateNo;
}



//#################################################################################################

Disinfection::DisinfectionDS::DisinfectionDS(const DisinfectionRecord* record, DisinfectionOptions* options) : m_record(record), m_options(options)
{
}

int Disinfection::DisinfectionDS::GetSheetCount()
{
	return 1;
}

bkprint::Values Disinfection::DisinfectionDS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	values.Set(L"EVENTDAY", m_record->loadDate.GetDayDate());
	if (!m_options->dontPutPlateNo())
	{
		values.Set(L"PLATENO", m_record->plateNo);
	}

	values.Set(L"CHEMICALAGENT", m_options->chemicalAgent());

	return std::move(values);
}

//#################################################################################################

Disinfection::HyperDisinfectionDS::HyperDisinfectionDS(Disinfection* parent, DisinfectionOptions* options)
{
	BOOST_FOREACH(const DisinfectionRecord* record, parent->m_pInputInterface->GetRecords())
	{
		m_dataSrcs.push_back(new DisinfectionDS(record, options));
	};
}

Disinfection::HyperDisinfectionDS::~HyperDisinfectionDS()
{
	DeepDelete(m_dataSrcs);
}

int Disinfection::HyperDisinfectionDS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* Disinfection::HyperDisinfectionDS::GetDS(int index)
{
	return m_dataSrcs[index];
}



//#################################################################################################



Disinfection::Disinfection(bkprint::Template* templ, DisinfectionDocInputInterface* inputInterface)
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}

Disinfection::~Disinfection()
{

}


bool Disinfection::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut Disinfection::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	DisinfectionOptions options(profile);
	HyperDisinfectionDS ds(this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}

void Disinfection::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


//#################################################################################################

DisinfectionDocInputInterface::~DisinfectionDocInputInterface()
{
	DeleteRecords();
}

void DisinfectionDocInputInterface::DeleteRecords()
{
	
	DeepDelete(m_records);	
	m_records.clear();
}

size_t DisinfectionDocInputInterface::RecordCount() const
{
	return m_records.size();
}

const DisinfectionRecord* DisinfectionDocInputInterface::RecordAt(int index)
{
	return m_records[index];
}

DisinfectionRecordRange DisinfectionDocInputInterface::GetRecords()
{
	return DisinfectionRecordRange(m_records);
}

bool DisinfectionDocInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void DisinfectionDocInputInterface::Feed(SeasonSession* pSession,const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession,range);
}

void DisinfectionDocInputInterface::AddRecord(DisinfectionRecord* record)
{
	m_records.push_back(record);
}

DisinfectionRecord* DisinfectionDocInputInterface::CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc)
{
	DisinfectionRecord* record = new DisinfectionRecord();
		
	record->docNumber = DocNoText(pDoc).ToString();
	record->docDate = pDoc->GetDocDate();
	record->loadDate = pDoc->GetLoadDate();
	record->plateNo = pDoc->GetPlateNo();
	record->extras = pDoc->GetExtras();
	record->animalCount = pDoc->GetCowCount();

	return record;
}

//#################################################################################################

void BuyDocDisinfectionInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{

	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId, range)
	{
		IHentsDoc* pDoc = pSession->GetBuyDoc(docId)->Object();
		DisinfectionRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->fromHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->toHent, pDoc->GetHerdId());

		AddRecord(record);



	}
}

//-------------------------------------------------------------------------------------------------
void SellDocDisinfectionInputInterface::Consume(SeasonSession* pSession, const UniqueIds& range)
{
	HentViewCreator hentViewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);

	BOOST_FOREACH(uint32_t docId, range)
	{
		IHentsDoc* pDoc = pSession->GetSellDoc(docId)->Object();
		DisinfectionRecord* record = CreateRecord(pSession, pDoc);

		hentViewCreator.RefreshView(&record->toHent, pSession->GetHent(pDoc->GetHentId()));
		farmViewCreator.RefreshView(&record->fromHent, pDoc->GetHerdId());
		AddRecord(record);
	}
}


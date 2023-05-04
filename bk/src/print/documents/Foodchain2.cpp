#include "stdafx.h"
#include "Foodchain2.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include "../../view/CowPresenceViewCreator.h"
#include <data\utils\GetDocInvoice.h>
#include <string\IntString.h>

Foodchain2Options::Foodchain2Options(IPrintProfile* profile) 
{
	if (!profile) return;

	PrintOption* spiecieOption = profile->GetOption(L"SPIECIE");
	if (spiecieOption){
		m_spiecies = spiecieOption->get();
	}

	PrintOption* slaughterOption = profile->GetOption(L"SLAUGHTER");
	if (slaughterOption) {
		m_slaughter = slaughterOption->get();
	}

	PrintOption* platenoOption = profile->GetOption(L"PLATENO");
	if (platenoOption) {
		m_plateno = platenoOption->get();
	}
	
}

const std::wstring& Foodchain2Options::slaughter()
{
	return m_slaughter;
}	

const std::wstring& Foodchain2Options::spiecies()
{
	return m_spiecies;
}

const std::wstring& Foodchain2Options::plateno()
{
	return m_plateno;
}

//#################################################################################################

Foodchain2Record::Foodchain2Record() : hent(NULL_ID)
{
}

//#################################################################################################

Foodchain2::Foodchain2DS::Foodchain2DS(const Foodchain2Record* record, Foodchain2Options* options) : m_record(record), m_options(options)
{
}

int Foodchain2::Foodchain2DS::GetSheetCount()
{
	return 1;
}

bkprint::Values Foodchain2::Foodchain2DS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	values.Set(L"SPIECIES", m_options->spiecies());
	values.Set(L"SLAUGHTER", m_options->slaughter());	
	values.Set(L"COWAMOUNT", IntString(m_record->cowNos.size()));

	std::wstring plateNo = m_options->plateno();
	
	if (plateNo.empty())
	{
		plateNo = m_record->plateNo;
	}

	values.Set(L"PLATENO", plateNo);
	values.Set(L"CITY", m_record->hent.GetStreet());
	values.Set(L"ZIP", m_record->hent.GetZip());
	//values.Set(L"DISTRICT", );
	values.Set(L"POBOX", m_record->hent.GetPOBox());
	values.Set(L"POST", m_record->hent.GetCity());
	//values.Set(L"COUNTY", );
	values.Set(L"HENTNO", m_record->hent.GetHentNo().ToString());
	values.Set(L"NAME", m_record->hent.GetName());
	values.Set(L"DATE", m_record->date.GetDateInFmt(DateTime::YYYYdashMMdashDD));
	
	return std::move(values);
}

//#################################################################################################

Foodchain2::HyperFoodchain2DS::HyperFoodchain2DS(Foodchain2* parent, Foodchain2Options* options)
{
	BOOST_FOREACH(const Foodchain2Record* record, parent->m_inputInterface->GetRecords())
	{
		m_dataSrcs.push_back(new Foodchain2DS(record, options));
	};
}

Foodchain2::HyperFoodchain2DS::~HyperFoodchain2DS()
{
	DeepDelete(m_dataSrcs);
}

int Foodchain2::HyperFoodchain2DS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* Foodchain2::HyperFoodchain2DS::GetDS(int index)
{
	return m_dataSrcs[index];
}



//#################################################################################################

Foodchain2::Foodchain2(bkprint::Template* templ, Foodchain2InputInterface* inputInterface) : m_inputInterface(inputInterface)
{
	m_pTemplate = templ;	
}

Foodchain2::~Foodchain2()
{
	
}

bool Foodchain2::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_inputInterface->Query(interfase_type))
	{
		*interfase = m_inputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut Foodchain2::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	Foodchain2Options options(profile);
	HyperFoodchain2DS ds(this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	return std::move(printOut);
}

void Foodchain2::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

//#################################################################################################

Foodchain2InputInterface::~Foodchain2InputInterface()
{
	DeleteRecords();
}

void Foodchain2InputInterface::DeleteRecords()
{
	BOOST_FOREACH(Foodchain2Record* record, m_records)
	{
		delete record;
	}
	m_records.clear();
}

Foodchain2RecordRange Foodchain2InputInterface::GetRecords()
{
	return Foodchain2RecordRange(m_records);
}

bool Foodchain2InputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void Foodchain2InputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void Foodchain2InputInterface::AddRecord(Foodchain2Record* record)
{
	m_records.push_back(record);
}

Foodchain2Record* Foodchain2InputInterface::CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc)
{
	Foodchain2Record* record = new Foodchain2Record();

	ICowEntry* entry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while (cowsEnum.hasNext())
	{
		entry = *cowsEnum;
		Cow* cow = pSession->GetCow(entry->GetCowId())->Object();
		record->cowNos.push_back(cow->GetCowNo());
		cowsEnum.advance();
	}

	HentClientObject* hentCO = pSession->GetHent(pDoc->GetHentId());
	HentViewCreator viewCreator(pSession);
	viewCreator.RefreshView(&record->hent, hentCO);

	record->plateNo = pDoc->GetPlateNo();
	record->date = pDoc->GetDocDate();

	return record;

}

void Foodchain2InputInterface::AddRecord(SeasonSession* pSession, IHentsDoc* pDoc)
{
	AddRecord(CreateRecord(pSession, pDoc));
}

//#################################################################################################
void Foodchain2BuyDocInputInterface::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	BOOST_FOREACH(uint32_t id, ids)
	{
		BuyDoc* doc = pSession->GetBuyDoc(id)->Object();
		AddRecord(pSession, doc);		
	}
}

//#################################################################################################
void Foodchain2SellDocInputInterface::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	BOOST_FOREACH(uint32_t id, ids)
	{
		SellDoc* doc = pSession->GetSellDoc(id)->Object();
		AddRecord(pSession, doc);
	}
}






#include "stdafx.h"
#include "Foodchain3.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include "../../view/CowPresenceViewCreator.h"
#include <data\utils\GetDocInvoice.h>
#include <string\IntString.h>

Foodchain3Options::Foodchain3Options(IPrintProfile* profile) 
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

const std::wstring& Foodchain3Options::slaughter()
{
	return m_slaughter;
}
	

const std::wstring& Foodchain3Options::spiecies()
{
	return m_spiecies;
}

const std::wstring& Foodchain3Options::plateno()
{
	return m_plateno;
}

//#################################################################################################

Foodchain3Record::Foodchain3Record() : hent(NULL_ID)
{
}

//#################################################################################################

Foodchain3::Foodchain3DS::Foodchain3DS(const Foodchain3Record* record, Foodchain3Options* options) : m_record(record), m_options(options)
{
}

int Foodchain3::Foodchain3DS::GetSheetCount()
{
	return 1;
}

bkprint::Values Foodchain3::Foodchain3DS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	values.Set(L"BUYFROMHENTNAME", m_record->hent.GetName());
	values.Set(L"BUYFROMHENTZIP", m_record->hent.GetZip());
	values.Set(L"BUYFROMHENTCITY", m_record->hent.GetCity());
	values.Set(L"BUYFROMHENTSTREETADDR", m_record->hent.GetStreet() + L" " + m_record->hent.GetPOBox());
	values.Set(L"BUYFROMHENTHENTNO", m_record->hent.GetHentNo().ToString());
	
	values.Set(L"SPIECIE", m_options->spiecies());
	values.Set(L"SLAUGHTER", m_options->slaughter());	
	values.Set(L"COWAMOUNT", IntString(m_record->cowNos.size()));
	std::wstring plateNo = m_options->plateno();

	if (plateNo.empty())
	{
		plateNo = m_record->plateNo;
	}

	values.Set(L"PLATENO", plateNo);
	values.Set(L"DATE", m_record->date.GetDateInFmt(DateTime::YYYYdashMMdashDD));

	std::wstring sCowNos;
	auto cowNoIt = m_record->cowNos.begin();
	while (cowNoIt != m_record->cowNos.end())
	{
		sCowNos += cowNoIt->ToString();
		cowNoIt++;

		if (cowNoIt != m_record->cowNos.end())
		{
			sCowNos += L", ";
		}
	}
	values.Set(L"COWNOS", sCowNos);
	
	return std::move(values);
}

//#################################################################################################

Foodchain3::HyperFoodchain3DS::HyperFoodchain3DS(Foodchain3* parent, Foodchain3Options* options)
{
	BOOST_FOREACH(const Foodchain3Record* record, parent->m_inputInterface->GetRecords())
	{
		m_dataSrcs.push_back(new Foodchain3DS(record, options));
	};
}

Foodchain3::HyperFoodchain3DS::~HyperFoodchain3DS()
{
	DeepDelete(m_dataSrcs);
}

int Foodchain3::HyperFoodchain3DS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* Foodchain3::HyperFoodchain3DS::GetDS(int index)
{
	return m_dataSrcs[index];
}



//#################################################################################################

Foodchain3::Foodchain3(bkprint::Template* templ, Foodchain3InputInterface* inputInterface) : m_inputInterface(inputInterface)
{
	m_pTemplate = templ;	
}

Foodchain3::~Foodchain3()
{
	
}

bool Foodchain3::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (m_inputInterface->Query(interfase_type))
	{
		*interfase = m_inputInterface;
		return true;
	}
	return false;
}

bkprint::PrintOut Foodchain3::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	Foodchain3Options options(profile);
	HyperFoodchain3DS ds(this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	return std::move(printOut);
}

void Foodchain3::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

//#################################################################################################

Foodchain3InputInterface::~Foodchain3InputInterface()
{
	DeleteRecords();
}

void Foodchain3InputInterface::DeleteRecords()
{
	BOOST_FOREACH(Foodchain3Record* record, m_records)
	{
		delete record;
	}
	m_records.clear();
}

Foodchain3RecordRange Foodchain3InputInterface::GetRecords()
{
	return Foodchain3RecordRange(m_records);
}

bool Foodchain3InputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void Foodchain3InputInterface::Feed(SeasonSession* pSession, const UniqueIds& range)
{
	DeleteRecords();
	Consume(pSession, range);
}

void Foodchain3InputInterface::AddRecord(Foodchain3Record* record)
{
	m_records.push_back(record);
}

Foodchain3Record* Foodchain3InputInterface::CreateRecord(SeasonSession* pSession, IHentsDoc* pDoc)
{
	Foodchain3Record* record = new Foodchain3Record();

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

void Foodchain3InputInterface::AddRecord(SeasonSession* pSession, IHentsDoc* pDoc)
{
	AddRecord(CreateRecord(pSession, pDoc));
}

//#################################################################################################
void Foodchain3BuyDocInputInterface::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	BOOST_FOREACH(uint32_t id, ids)
	{
		BuyDoc* doc = pSession->GetBuyDoc(id)->Object();
		AddRecord(pSession, doc);		
	}
}

//#################################################################################################
void Foodchain3SellDocInputInterface::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	BOOST_FOREACH(uint32_t id, ids)
	{
		SellDoc* doc = pSession->GetSellDoc(id)->Object();
		AddRecord(pSession, doc);
	}
}






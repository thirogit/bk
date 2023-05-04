#include "stdafx.h"
#include "Displacement.h"
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string/TextUtils.h>
#include <boost\foreach.hpp>
#include <arrays\DeepDelete.h>
#include <errortoken\ErrorToken.h>
#include "../../text/DocNoText.h"
#include "../runtime/FeedException.h"
#include "../options/PrintOptionSerializer.h"
#include "../options/DisplacementEventType.h"
#include "../../bkprint/renderer/TemplateRenderer.h"
#include <boost/throw_exception.hpp>
#include <data\utils\GetDocHerd.h>
#include <boost/range/adaptors.hpp>

DisplacementOptions::DisplacementOptions(IPrintProfile* profile)
{	
	m_events = DisplacementEventType_None;
	m_dontPrintFrom = false;
	m_dontPrintTo = false;
	m_dontPutPrintDate = false;

	if (profile)
	{
		PrintOption* eventsOption = profile->GetOption(L"EVENTS");
		PrintOptionSerializer serializer;
		if (eventsOption)
			m_events = serializer.GetDisplacementEventsValue(*eventsOption);

		PrintOption* noFromOption = profile->GetOption(L"NOFROM");		
		if (noFromOption)
			m_dontPrintFrom = serializer.GetBooleanValue(*noFromOption);

		PrintOption* noToOption = profile->GetOption(L"NOTO");
		if (noToOption)
			m_dontPrintTo = serializer.GetBooleanValue(*noToOption);

		PrintOption* noPrintDateOption = profile->GetOption(L"NOPRINTDATE");
		if (noPrintDateOption)
			m_dontPutPrintDate = serializer.GetBooleanValue(*noPrintDateOption);

		PrintOption* printDateOption = profile->GetOption(L"PRINTDATE");
		if (printDateOption)
			m_PrintDate = serializer.GetDateTimeValue(*printDateOption);
	}	
}

uint32_t DisplacementOptions::GetEvents()
{
	return m_events;
}

bool DisplacementOptions::GetDontPrintFrom()
{
	return m_dontPrintFrom;
}

bool DisplacementOptions::GetDontPrintTo()
{
	return m_dontPrintTo;
}

bool DisplacementOptions::GetDontPutPrintDate()
{
	return m_dontPutPrintDate;
}

const DateTime& DisplacementOptions::GetPrintDate()
{
	return m_PrintDate;
}


DisplacementNotification::DisplacementNotificationDS::DisplacementNotificationDS(const DisplacementRecord* record, int cowsOnSheet, DisplacementOptions* options)
{
	m_cowsOnSheet = cowsOnSheet;
	m_record = record;
	m_options = options;
}

int DisplacementNotification::DisplacementNotificationDS::GetSheetCount()
{
	int cowCount = m_record->GetCowCount();
	return (cowCount / m_cowsOnSheet) + (cowCount % m_cowsOnSheet > 0 ? 1 : 0);
}

bkprint::Values DisplacementNotification::DisplacementNotificationDS::GetValuesForCharacters(const std::wstring& s, int length, const std::wstring& valueNameFmt)
{
	bkprint::Values values;
	char c;
	for (int i = 0, len = s.length(); i < length; i++)
	{
		c = (i < len) ? s.at(i) : '-';
		values.Set(TextUtils::Format(valueNameFmt, i + 1), std::wstring(1,c));
	}
	return std::move(values);
}

uint32_t DisplacementNotification::DisplacementNotificationDS::GetEvents()
{
	return m_options->GetEvents();
}

bkprint::Values DisplacementNotification::DisplacementNotificationDS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	DisplacementDirection direction = m_record->GetDirection();
	
	if (!m_options->GetDontPrintFrom())
	{
		values += std::move(GetValuesForCharacters(direction.GetFrom().ToString(), 14, L"SRCFARMNO[%d]"));
	}

	CowNoRange range = m_record->GetCows();

	auto it = range.begin(), endIt = range.end();

	std::advance(it,sheet*m_cowsOnSheet);

	for (int i = 0; i < m_cowsOnSheet && it != endIt; i++, it++)
	{
		values += std::move(GetValuesForCharacters(it->ToString(), 14, TextUtils::Format(L"COWNO[%d][%%d]", i + 1)));
	}
	
	if (!m_options->GetDontPrintTo())
	{
		values += std::move(GetValuesForCharacters(direction.GetTo().ToString(), 14, L"DESTFARMNO[%d]"));
	}
	
	values += std::move(GetValuesForCharacters(m_record->GetEventDate().GetDateInFmt(DateTime::DDMMYYYY), 8, L"EVENTDATE[%d]"));

	if (!m_options->GetDontPutPrintDate())
	{
		DateTime printDt = m_options->GetPrintDate();

		if (printDt.IsNull())
		{
			printDt = DateTime::now();
		}

		values += std::move(GetValuesForCharacters(printDt.GetDateInFmt(DateTime::DDMMYYYY), 8, L"PRINTDATE[%d]"));
	}


	uint32_t events = GetEvents();
	if (events & (DisplacementEventType_ExportToEU | DisplacementEventType_ExportNotToEU))
	{
		values += std::move(GetValuesForCharacters(direction.GetTo().GetCountry().GetCountryCode2A(), 8, L"COUNTRY2A[%d]"));		
	}

	std::wstring sEventCheckMark = L"X";

	if (events & DisplacementEventType_Buy)
		values.Set(L"EVENTBUY", sEventCheckMark);

	if (events & DisplacementEventType_Sell)
		values.Set(L"EVENTSELL", sEventCheckMark);		

	if (events & DisplacementEventType_BuySell)
		values.Set(L"EVENTBUYANDSELL", sEventCheckMark);
		

	if (events & DisplacementEventType_ExportToEU)
	{
		values.Set(L"EVENTEXPORTTOEU", sEventCheckMark);
	}

	if (events & DisplacementEventType_ExportNotToEU)
	{
		values.Set(L"EVENTEXPORTNOTTOEU", sEventCheckMark);
	}

	if (events & DisplacementEventType_Other)
		values.Set(L"EVENTOTHER", sEventCheckMark);

	return std::move(values);
}



//#################################################################################################

DisplacementNotification::HyperDisplacementNotificationDS::HyperDisplacementNotificationDS(DisplacementNotification* parent, IPrintProfile* profile) : m_options(profile)
{
	BOOST_FOREACH(const DisplacementRecord* record, parent->m_pInputInterface->GetRecords())
	{
		m_dataSrcs.push_back(new DisplacementNotificationDS(record, parent->GetCowsOnSheet(), &m_options));
	};
}

DisplacementNotification::HyperDisplacementNotificationDS::~HyperDisplacementNotificationDS()
{
	DeepDelete(m_dataSrcs);
}

int DisplacementNotification::HyperDisplacementNotificationDS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* DisplacementNotification::HyperDisplacementNotificationDS::GetDS(int index)
{
	return m_dataSrcs[index];
}



//#################################################################################################

DisplacementDirection::DisplacementDirection()
{
}

DisplacementDirection::DisplacementDirection(const HentNo& from,const HentNo& to) : m_from(from),m_to(to)
{
}

DisplacementDirection::DisplacementDirection(const DisplacementDirection& src) : m_from(src.m_from),m_to(src.m_to)
{
}

DisplacementDirection& DisplacementDirection::operator=(const DisplacementDirection& right)
{
	m_from = right.m_from;
	m_to = right.m_to;
	return *this;
}

const HentNo& DisplacementDirection::GetFrom() const
{
	return m_from;
}

const HentNo& DisplacementDirection::GetTo() const
{
	return m_to;
}

//#################################################################################################

DisplacementRecord::DisplacementRecord()
{
}

void DisplacementRecord::SetDirection(const DisplacementDirection& direction)
{
	m_Direction = direction;
}

void DisplacementRecord::SetEventDate(const DateTime& eventDt)
{
	m_EventDate = eventDt;
}

void DisplacementRecord::AddCow(const CowNo& cowNo)
{
	m_cows.insert(cowNo);
}

const DisplacementDirection& DisplacementRecord::GetDirection() const
{
	return m_Direction;
}


const DateTime& DisplacementRecord::GetEventDate() const
{
	return m_EventDate;
}

size_t DisplacementRecord::GetCowCount() const
{
	return m_cows.size();
}

CowNoRange DisplacementRecord::GetCows() const
{
	return CowNoRange(m_cows);
}

/////////////////////////////////////////////////////////////////////////////////////////

DisplacementNotification::DisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface) 
{
	m_pTemplate = templ;
	m_pInputInterface = inputInterface;
}
	
bool DisplacementNotification::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(m_pInputInterface->Query(interfase_type))
	{
		*interfase = m_pInputInterface;
		return true;
	}	
	*interfase = NULL;
	return false;
}

DisplacementNotification::~DisplacementNotification()
{
}

bkprint::PrintOut DisplacementNotification::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	HyperDisplacementNotificationDS ds(this, profile);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}
//#################################################################################################

SimplexDisplacementNotification::SimplexDisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface) : DisplacementNotification(templ, inputInterface)
{
}

//#################################################################################################

DuplexDisplacementNotification::DuplexDisplacementNotification(bkprint::Template* templ, DisplacementInputInterface* inputInterface) : DisplacementNotification(templ, inputInterface)
{
}

int DuplexDisplacementNotification::GetCowsOnSheet() const
{
	return 60;
}

//#################################################################################################


DisplacementRecordsRange DisplacementInputInterface::GetRecords()
{
	return DisplacementRecordsRange(m_records);
}

bool DisplacementInputInterface::Query(const type_info& interfase_type)
{
	return typeid(IdsInputInterface) == interfase_type;
}

void DisplacementInputInterface::Feed(SeasonSession* pSession,const UniqueIds& ids)
{
	DeleteRecords();
	Consume(pSession,ids);
}

void DisplacementInputInterface::DeleteRecords()
{
	DeepDelete(m_records);
	m_records.clear();
}

DisplacementInputInterface::~DisplacementInputInterface()
{
	DeleteRecords();
}

void DisplacementInputInterface::AddRecord(DisplacementRecord* record)
{
	m_records.push_back(record);
}

//#################################################################################################
void BuyDocDisplacementInput::Consume(SeasonSession* pSession,const UniqueIds& ids)
{
				
	BOOST_FOREACH(uint32_t buyDocId, ids)
	{	
		IBuyDoc* pDoc = pSession->GetBuyDoc(buyDocId)->Object();
		DisplacementRecord* record = new DisplacementRecord();
						
		record->SetEventDate(pDoc->GetLoadDate());
		
		IHent* pHent = pSession->GetHent(pDoc->GetHentId())->Object();
						
		HentNo toPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(),
												HerdNoFactory::Create(pSession->GetHerd(pDoc->GetHerdId())->GetHerdIndex()));

		record->SetDirection(DisplacementDirection(pHent->GetHentNo(), toPlace));

		for(int i = 0, count = pDoc->GetCowCount();i < count;i++)
		{
			ICowEntry* pEntry = pDoc->GetCowAt(i);
			ICow* pCow = pSession->GetCow(pEntry->GetCowId())->Object();
			record->AddCow(pCow->GetCowNo());			
		}
		
		AddRecord(record);
	}

}

//#################################################################################################

void SellDocDisplacementInput::Consume(SeasonSession* pSession,const UniqueIds& ids)
{	
	BOOST_FOREACH(uint32_t sellDocId, ids)
	{		
		ISellDoc* pDoc = pSession->GetSellDoc(sellDocId)->Object();
		DisplacementRecord* record = new DisplacementRecord();
						
		record->SetEventDate(pDoc->GetLoadDate());
		
		IHent* pHent = pSession->GetHent(pDoc->GetHentId())->Object();
						
		HentNo toPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(),
												HerdNoFactory::Create(pSession->GetHerd(pDoc->GetHerdId())->GetHerdIndex()));

		record->SetDirection(DisplacementDirection(toPlace, pHent->GetHentNo()));

		for(int i = 0, count = pDoc->GetCowCount();i < count;i++)
		{
			ICowEntry* pEntry = pDoc->GetCowAt(i);
			ICow* pCow = pSession->GetCow(pEntry->GetCowId())->Object();
			record->AddCow(pCow->GetCowNo());			
		}
		
		AddRecord(record);
	}

}

//#################################################################################################

void MoveDocDisplacementInput::Consume(SeasonSession* pSession,const UniqueIds& ids)
{
	std::vector<IMoveDoc*> moveDocs;
	BOOST_FOREACH(uint32_t moveDocId,ids)
	{	
		IMoveDoc* pDoc = pSession->GetMoveDoc(moveDocId)->Object();									
		moveDocs.push_back(pDoc);
	}
				
	BOOST_FOREACH(IMoveDoc* pDoc,moveDocs)
	{		
		DisplacementRecord* record = new DisplacementRecord();
						
		record->SetEventDate(pDoc->GetLoadDate());

						
		HentNo toPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(),
												HerdNoFactory::Create(pSession->GetHerd(pDoc->GetDstHerdId())->GetHerdIndex()));

		HentNo fromPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(),
												HerdNoFactory::Create(pSession->GetHerd(pDoc->GetSrcHerdId())->GetHerdIndex()));

		record->SetDirection(DisplacementDirection(fromPlace,toPlace));		

		for(int i = 0, count = pDoc->GetCowCount();i < count;i++)
		{
			ICowEntry* pEntry = pDoc->GetCowAt(i);
			ICow* pCow = pSession->GetCow(pEntry->GetCowId())->Object();
			record->AddCow(pCow->GetCowNo());			
		}
		
		AddRecord(record);
	}

}

//#################################################################################################

void BuyInvoiceDisplacementInput::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	std::vector<IBuyInvoice*> buyInvoices;
	BOOST_FOREACH(uint32_t id, ids)
	{
		IBuyInvoice* invoice = pSession->GetBuyInvoice(id)->Object();
		buyInvoices.push_back(invoice);
	}
	
	BOOST_FOREACH(IBuyInvoice* invoice, buyInvoices)
	{
		std::map<uint32_t, DisplacementRecord*> recordsPerEntryDocs;
		PtrEnumerator<ICowInvoiceEntry> entryEnum = invoice->EnumEntries();
		while (entryEnum.hasNext())
		{
			ICowInvoiceEntry* entry = *entryEnum;
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			ICowTrail* trail = pSession->GetCowTrail(entry->GetCowId());
			const ICowFootprint* entryFootprint = trail->GetEntry();
			IDoc* entryDoc = entryFootprint->GetDoc();
	
			auto it = recordsPerEntryDocs.find(entryDoc->GetId());
			if (it != recordsPerEntryDocs.end())
			{
				it->second->AddCow(cow->GetCowNo());
			}
			else
			{
				DisplacementRecord* record = new DisplacementRecord();
				const IInvoiceHent* buyFromHent = invoice->GetInvoiceHent();
				
				HentNo fromPlace = buyFromHent->GetHentNo();
				HentNo toPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(),HerdNoFactory::Create(pSession->GetHerd(GetDocHerd(entryDoc))->GetHerdIndex()));
								
				record->SetEventDate(entryDoc->GetLoadDate());
				record->SetDirection(DisplacementDirection(fromPlace, toPlace));
				record->AddCow(cow->GetCowNo());
				recordsPerEntryDocs[entryDoc->GetId()] = record;
			}
			
			
			entryEnum.advance();
		}

		BOOST_FOREACH(DisplacementRecord* record, recordsPerEntryDocs | boost::adaptors::map_values)
		{
			AddRecord(record);
		}

	
	}

}

//#################################################################################################

void SellInvoiceDisplacementInput::Consume(SeasonSession* pSession, const UniqueIds& ids)
{
	std::vector<ISellInvoice*> invoices;
	BOOST_FOREACH(uint32_t id, ids)
	{
		ISellInvoice* invoice = pSession->GetSellInvoice(id)->Object();
		invoices.push_back(invoice);
	}

	BOOST_FOREACH(ISellInvoice* invoice, invoices)
	{
		std::map<uint32_t, DisplacementRecord*> recordsPerEntryDocs;
		PtrEnumerator<ICowInvoiceEntry> entryEnum = invoice->EnumEntries();
		while (entryEnum.hasNext())
		{
			ICowInvoiceEntry* entry = *entryEnum;
			ICow* cow = pSession->GetCow(entry->GetCowId())->Object();
			ICowTrail* trail = pSession->GetCowTrail(entry->GetCowId());
			const ICowFootprint* exitFootprint = trail->GetExit();
			IDoc* exitDoc = exitFootprint->GetDoc();

			auto it = recordsPerEntryDocs.find(exitDoc->GetId());
			if (it != recordsPerEntryDocs.end())
			{
				it->second->AddCow(cow->GetCowNo());
			}
			else
			{
				DisplacementRecord* record = new DisplacementRecord();
				const IInvoiceHent* sellToHent = invoice->GetInvoiceHent();

				HentNo toPlace = sellToHent->GetHentNo();
				HentNo fromPlace = HentNoFactory::Create(pSession->GetFarm()->GetFarmNo(), HerdNoFactory::Create(pSession->GetHerd(GetDocHerd(exitDoc))->GetHerdIndex()));

				record->SetEventDate(exitDoc->GetLoadDate());
				record->SetDirection(DisplacementDirection(fromPlace, toPlace));

				record->AddCow(cow->GetCowNo());
				recordsPerEntryDocs[exitDoc->GetId()] = record;
			}


			entryEnum.advance();
		}

		BOOST_FOREACH(DisplacementRecord* record, recordsPerEntryDocs | boost::adaptors::map_values)
		{
			AddRecord(record);
		}


	}

}
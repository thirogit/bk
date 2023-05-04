#include "stdafx.h"
#include "DisplacementViaMarket.h"
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
#include "../../view/CowPresenceViewCreator.h"

#define COWS_ON_SHEET 10


DisplacementViaMarketOptions::DisplacementViaMarketOptions(IPrintProfile* profile)
{
	
	if (profile)
	{
		PrintOption* merketNoOption = profile->GetOption(L"MARKETNO");
		
		if (merketNoOption)
		{
			m_MarketNo = HentNoFactory::FromString(merketNoOption->get());
		}

		PrintOption* fromNoOption = profile->GetOption(L"FROMNO");

		if (fromNoOption)
		{
			m_SrcHentNo = HentNoFactory::FromString(fromNoOption->get());
		}

		PrintOption* toNoOption = profile->GetOption(L"TONO");

		if (toNoOption)
		{
			m_DstHentNo = HentNoFactory::FromString(toNoOption->get());
		}
		
	}
}

HentNo DisplacementViaMarketOptions::GetMarketNo() const
{
	return m_MarketNo;
}

HentNo DisplacementViaMarketOptions::GetSrcHentNo() const
{
	return  m_SrcHentNo;
}

HentNo DisplacementViaMarketOptions::GetDstHentNo() const
{
	return m_DstHentNo;
}

DisplacementViaMarketDocument::DisplacementViaMarketNotificationDS::DisplacementViaMarketNotificationDS(const DisplacementViaMarketRecord* record,const DisplacementViaMarketOptions* options)
{
	m_record = record;
	m_options = options;
}

int DisplacementViaMarketDocument::DisplacementViaMarketNotificationDS::GetSheetCount()
{
	int cowCount = m_record->GetRowCount();
	return (cowCount / COWS_ON_SHEET) + (cowCount % COWS_ON_SHEET > 0 ? 1 : 0);
}

bkprint::Values DisplacementViaMarketDocument::DisplacementViaMarketNotificationDS::GetValuesForCharacters(const std::wstring& s, int length, const std::wstring& valueNameFmt)
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


bkprint::Values DisplacementViaMarketDocument::DisplacementViaMarketNotificationDS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	HentNo marketNo = m_options->GetMarketNo();
	
	if (!marketNo.IsNull())
	{
		values += std::move(GetValuesForCharacters(marketNo.ToString(), 14, L"MARKETNO[%d]"));
	}

	DisplacementViaMarketRowsRange range = m_record->GetRows();

	auto it = range.begin(), endIt = range.end();

	std::advance(it,sheet*COWS_ON_SHEET);

	for (int i = 0; i < COWS_ON_SHEET && it != endIt; i++, it++)
	{
		const DisplacementViaMarketRecordRow* row = *it;
		values += std::move(GetValuesForCharacters(row->cowNo.ToString(), 14, TextUtils::Format(L"COWNO[%d][%%d]", i + 1)));

		HentNo hentNoFrom = m_options->GetSrcHentNo();
		if (hentNoFrom.IsNull())
		{
			hentNoFrom = row->hentNoFrom;
		}

		if (!hentNoFrom.IsNull())
		{
			values += std::move(GetValuesForCharacters(hentNoFrom.ToString(), 14, TextUtils::Format(L"FROM[%d][%%d]", i + 1)));
		}

		HentNo hentNoTo = m_options->GetDstHentNo();
		if (hentNoTo.IsNull())
		{
			hentNoTo = row->hentNoTo;
		}

		if (!hentNoTo.IsNull())
		{
			values += std::move(GetValuesForCharacters(hentNoTo.ToString(), 14, TextUtils::Format(L"TO[%d][%%d]", i + 1)));
		}
	}

	DateTime eventDt = m_record->GetEventDate();
	if (!eventDt.IsNull())
	{
		values += std::move(GetValuesForCharacters(m_record->GetEventDate().GetDateInFmt(DateTime::DDMMYYYY), 8, L"EVENTDATE[%d]"));
	}

	values.Set(L"PRINTDATE", DateTime::now().GetDateInFmt(DateTime::DDdashMMdashYYYY));

	
		

	

	
	return std::move(values);
}



//#################################################################################################

DisplacementViaMarketDocument::HyperDisplacementViaMarketNotificationDS::HyperDisplacementViaMarketNotificationDS(DisplacementViaMarketDocument* parent, const DisplacementViaMarketOptions* options)
{
	BOOST_FOREACH(const DisplacementViaMarketRecord* record, parent->m_records)
	{
		m_dataSrcs.push_back(new DisplacementViaMarketNotificationDS(record, options));
	};
}

DisplacementViaMarketDocument::HyperDisplacementViaMarketNotificationDS::~HyperDisplacementViaMarketNotificationDS()
{
	DeepDelete(m_dataSrcs);
}

int DisplacementViaMarketDocument::HyperDisplacementViaMarketNotificationDS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* DisplacementViaMarketDocument::HyperDisplacementViaMarketNotificationDS::GetDS(int index)
{
	return m_dataSrcs[index];
}


//#################################################################################################

DisplacementViaMarketRecord::DisplacementViaMarketRecord()
{
}


DisplacementViaMarketRecord::~DisplacementViaMarketRecord()
{
	DeepDelete(m_rows);
}

void DisplacementViaMarketRecord::SetEventDate(const DateTime& eventDt)
{
	m_EventDate = eventDt;
}

void DisplacementViaMarketRecord::AddRow(const CowNo& cowNo, const HentNo& hentNoFrom, const HentNo& hentNoTo)
{

	DisplacementViaMarketRecordRow* row = new DisplacementViaMarketRecordRow();
	row->cowNo = cowNo;
	row->hentNoFrom = hentNoFrom;
	row->hentNoTo = hentNoTo;

	m_rows.insert(row);
}


const DateTime& DisplacementViaMarketRecord::GetEventDate() const
{
	return m_EventDate;
}

size_t DisplacementViaMarketRecord::GetRowCount() const
{
	return m_rows.size();
}

DisplacementViaMarketRowsRange DisplacementViaMarketRecord::GetRows() const
{
	return DisplacementViaMarketRowsRange(m_rows);
}

/////////////////////////////////////////////////////////////////////////////////////////

DisplacementViaMarketDocument::DisplacementViaMarketDocument(bkprint::Template* templ)
{
	m_pTemplate = templ;
}
	
bool DisplacementViaMarketDocument::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(typeid(KeysInputInterface) == interfase_type)
	{
		if (interfase)
			*interfase = (KeysInputInterface*)this;
		return true;
	}	
	*interfase = NULL;
	return false;
}

DisplacementViaMarketDocument::~DisplacementViaMarketDocument()
{
	DeleteRecords();
}

bkprint::PrintOut DisplacementViaMarketDocument::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;

	DisplacementViaMarketOptions options(profile);

	HyperDisplacementViaMarketNotificationDS ds(this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}

void DisplacementViaMarketDocument::DeleteRecords()
{
	DeepDelete(m_records);
	m_records.clear();
}


void DisplacementViaMarketDocument::AddRecord(DisplacementViaMarketRecord* record)
{
	m_records.push_back(record);
}

void DisplacementViaMarketDocument::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

void DisplacementViaMarketDocument::Feed(SeasonSession* pSession, const UniquePresenceKeys& keys)
{
				
	DeleteRecords();

	DisplacementViaMarketRecord* record = new DisplacementViaMarketRecord();

	const DocumentTerritory* territory = m_pContext->GetTeritory();
	HerdRegistry* pRegistry = pSession->GetHerdRegistry(territory->GetHerd()->GetId());
	
	
		CowPresenceViewCreator viewCreator(pSession);
		BOOST_FOREACH(CowPresenceKey key, keys)
		{
			CowPresenceView view(key);
			HentNo buyFromHentNo, sellToHentNo;
			viewCreator.RefreshView(&view,pRegistry->GetCowPresence(key));
			const HentView* buyFromHent = view.GetBuyFromHent();

			uint32_t cowId = key.GetCowId();

			ICowTrail* trail = pSession->GetCowTrail(cowId);
			
			if (buyFromHent != NULL)
			{
				buyFromHentNo = buyFromHent->GetHentNo();
			}
			else
			{
				const ICowFootprint* entryFootprint = trail->GetEntry();
				IDoc* entryDoc = entryFootprint->GetDoc();
				if (entryDoc->GetDocType() == DocType_Buy)
				{
					Hent* buyDocHent = pSession->GetHent(((IBuyDoc*)entryDoc)->GetHentId())->Object();
					buyFromHentNo = buyDocHent->GetHentNo();
				}
				else
				{
					ICow* cow = pSession->GetCow(cowId)->Object();
					if (cow->GetFirstOwnerId() != NULL_ID)
					{
						Hent* firstOwnerHent = pSession->GetHent(cow->GetFirstOwnerId())->Object();
						buyFromHentNo = firstOwnerHent->GetHentNo();
					}
				}
				
			}

			const HentView* sellToHent = view.GetSellToHent();
			if (sellToHent != NULL)
			{
				sellToHentNo = sellToHent->GetHentNo();
			}
			else
			{
				const ICowFootprint* exitFootprint = trail->GetExit();
				if (exitFootprint != NULL)
				{
					IDoc* exitDoc = exitFootprint->GetDoc();

					if (exitDoc->GetDocType() == DocType_Sell)
					{
						Hent* sellDocHent = pSession->GetHent(((ISellDoc*)exitDoc)->GetHentId())->Object();
						sellToHentNo = sellDocHent->GetHentNo();
					}
				}

			}

			record->AddRow(view.GetCowNo(), buyFromHentNo, sellToHentNo);

		}

		AddRecord(record);


}


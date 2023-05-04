#include "stdafx.h"
#include "WeterinarianBook.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include "../../view/CowPresenceViewCreator.h"
#include "../options/PrintOptionSerializer.h"
#include <string\IntString.h>
#include <compare/TypesCompare.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../../view/FarmHentViewCreator.h"
#include "../../context/object/HentClientObject.h"
#include <string/TextUtils.h>

using namespace TypesCompare;


const std::wstring CowPresenceViewCowNoFieldComparator::FIELD_CODE = L"COWNO";

COMPARE_RESULT CowPresenceViewCowNoFieldComparator::Compare(const CowPresenceView* record1, const CowPresenceView* record2)
{
	return CompareNullable(record1->GetCowNo(), record2->GetCowNo());
}

std::wstring CowPresenceViewCowNoFieldComparator::GetFieldCode() const
{
	return FIELD_CODE;
}


const std::wstring CowPresenceViewInDateFieldComparator::FIELD_CODE = L"INDATE";

COMPARE_RESULT CowPresenceViewInDateFieldComparator::Compare(const CowPresenceView* record1, const CowPresenceView* record2)
{
	return CompareDateTime(record1->GetHerdInDate(), record2->GetHerdInDate());
}

std::wstring CowPresenceViewInDateFieldComparator::GetFieldCode() const
{
	return FIELD_CODE;
}


const std::wstring CowPresenceViewOutDateFieldComparator::FIELD_CODE = L"OUTDATE";

COMPARE_RESULT CowPresenceViewOutDateFieldComparator::Compare(const CowPresenceView* record1, const CowPresenceView* record2)
{
	return CompareDateTime(record1->GetHerdOutDate(), record2->GetHerdOutDate());
}

std::wstring CowPresenceViewOutDateFieldComparator::GetFieldCode() const
{
	return FIELD_CODE;
}


WetBookOptions::WetBookOptions(IPrintProfile* profile) : m_recordIndex(1)
{
	if (!profile) return;
	m_recordIndex = max(1, GetIndex(L"RECORDINDEX", profile).ToInt());	
}

NullInt WetBookOptions::GetIndex(const std::wstring& indexName, IPrintProfile* profile)
{
	NullInt result;

	if (profile)
	{
		PrintOption* option = profile->GetOption(indexName);
		if (option)
		{
			PrintOptionSerializer serializer;
			result = serializer.GetIntegerValue(*option);
		}
	}
	return result;
}

int WetBookOptions::recordIndex() const
{
	return m_recordIndex;
}


//#######################################################################################
WeterinarianBook::WetBookDS::ItemsBandEvaluator::ItemsBandEvaluator(WetBookDS* parent) : m_parent(parent)
{

}

bkprint::Values WeterinarianBook::WetBookDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values WeterinarianBook::WetBookDS::ItemsBandEvaluator::Footer()
{
	return std::move(bkprint::Values());
}

size_t WeterinarianBook::WetBookDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_parent->m_rows.size();
}

bkprint::Values WeterinarianBook::WetBookDS::ItemsBandEvaluator::At(int index)
{
	WeterinarianBookRow* row = m_parent->m_parent->m_rows[index];
	return std::move(CreateRowValues(index, row));
}

std::wstring WeterinarianBook::WetBookDS::ItemsBandEvaluator::FormatCow(const CowPresenceView *pCow)
{
	std::wstring result = pCow->GetCowNo().ToString();
	std::wstring sPassNo = pCow->GetPassNo();
	std::wstring sHealthCertNo = pCow->GetHealthCertNo();

	if (!sPassNo.empty())
	{
		result += L"\n(";
		result += sPassNo;
		result += L')';
	}

	if (!sHealthCertNo.empty())
	{
		result += L"\n[";
		result += sHealthCertNo;
		result += L']';
	}
	return result;
}


std::wstring WeterinarianBook::WetBookDS::ItemsBandEvaluator::FormatHent(const HentView* pHent) const
{
	std::wstring result;

	if (pHent)
	{

		result += pHent->GetName();
		result += L'\n';
		result += pHent->GetStreet();
		result += L' ';
		result += pHent->GetPOBox();
		result += L'\n';
		result += pHent->GetZip();
		result += L' ';
		result += pHent->GetCity();
		result += L'\n';
		result += pHent->GetHentNo().ToString();

		std::wstring wetNo = pHent->GetWetNo();
		if(!wetNo.empty() && !TextUtils::IsAllWhitespace(wetNo))
		{
		result += L'\n';
		result += L"(";
		result += pHent->GetWetNo();
		result += L")";
		}
		/*
		if(pHent->HasWetLicenceNo())
		{
		result += '\n';
		result += "[";
		result += pHent->GetWetLicenceNo();
		result += "]";
		}*/
	}
	return result;
}


bkprint::Values WeterinarianBook::WetBookDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const WeterinarianBookRow* row)
{
	bkprint::Values values;
	values.Set(L"INDEX", IntString(nindex + m_parent->m_options.recordIndex()));
	CowPresenceView* cow = row->cow;	
	
	if (row->fromPlace)
	{
		values.Set(L"FROMPLACE", FormatHent(row->fromPlace));
		values.Set(L"FROMHENTNO", row->fromPlace->GetHentNo().ToString());
		values.Set(L"INWETIDNO", row->inWetNo);
	}

	values.Set(L"COW", FormatCow(cow));

	const HentView* firstOwner = cow->GetFirstOwner();
	
	const HentView* pBuyFromHent = cow->GetBuyFromHent();
	if (!pBuyFromHent)
		pBuyFromHent = firstOwner;

	if (!pBuyFromHent)
		pBuyFromHent = row->fromPlace;

	values.Set(L"FSTOWNER", FormatHent(pBuyFromHent));
		
	values.Set(L"INDATE", cow->GetHerdInDate().GetDayDate());

	if (!cow->IsInStock())
	{
		values.Set(L"OUTDATE", cow->GetHerdOutDate().GetDayDate());
		values.Set(L"PLATEOUT", cow->GetHerdOutPlateNo());

		if (row->toPlace)
		{
			values.Set(L"OUTPLACE", FormatHent(row->toPlace));
		}
		else
		{
			values.Set(L"OUTPLACE", row->toPlaceStr);
		}
		
		values.Set(L"OUTWETIDNO", row->outWetNo);
	}

	values.Set(L"PLATEIN", cow->GetHerdInPlateNo());
	

	return std::move(values);
}

//#######################################################################################

WeterinarianBook::WetBookDS::WetBookDS(WeterinarianBook* parent, IPrintProfile* profile) : m_parent(parent), m_options(profile)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);
}

WeterinarianBook::WetBookDS::~WetBookDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values WeterinarianBook::WetBookDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	//heaaderValues.Set(L"INVOICETITLE", m_options.title());


	return std::move(heaaderValues);
}

bkprint::Values WeterinarianBook::WetBookDS::GetFooterValues()
{
	bkprint::Values values;

	return std::move(values);
}

bkprint::Values WeterinarianBook::WetBookDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values WeterinarianBook::WetBookDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::BandEvaluator* WeterinarianBook::WetBookDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}



//#######################################################################################

WeterinarianBook::WeterinarianBook(bkprint::Template* templ) 
{
	m_pTemplate = templ;
}

WeterinarianBook::~WeterinarianBook()
{
	DeleteRows();
}

bool WeterinarianBook::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(interfase_type == typeid(KeysInputInterface))
	{
		if(interfase)
			*interfase = (KeysInputInterface*)this;
		return true;
	}

	return false;
}


void WeterinarianBook::DeleteRows()
{
	BOOST_FOREACH(WeterinarianBookRow* row, m_rows)
	{
		delete row->cow;
		delete row->fromPlace;
		delete row->toPlace;
		delete row;
	}

	m_rows.clear();
}

void WeterinarianBook::Feed(SeasonSession* pSession,const UniquePresenceKeys& keys)
{
	DeleteRows();

	const DocumentTerritory* territory = m_pContext->GetTeritory();
	HerdRegistry* pRegistry = pSession->GetHerdRegistry(territory->GetHerd()->GetId());
	CowPresenceViewCreator viewCreator(pSession);
	FarmHentViewCreator farmViewCreator(pSession);
	HentViewCreator hentViewCreator(pSession);
	BOOST_FOREACH(CowPresenceKey key,keys)
	{
		WeterinarianBookRow* row = new  WeterinarianBookRow;
		row->fromPlace = NULL;
		row->toPlace = NULL;
		ICowPresence* presence = pRegistry->GetCowPresence(key);
		CowPresenceView* presenceView = viewCreator.CreateView(presence);
		row->cow = presenceView;
		const IDoc* herdInDoc = presence->GetHerdInDoc();
		if (herdInDoc->GetDocType() != DocType_Move)
		{			
			const HentView* buyFromHent = presenceView->GetBuyFromHent();

			if (buyFromHent)
			{
				row->fromPlace = new HentView(*buyFromHent);
			}
			else
			{
				if (herdInDoc->GetDocType() == DocType_Buy)
				{
					const IBuyDoc* buyDoc = (const IBuyDoc*)herdInDoc;
					HentClientObject* hentCO = pSession->GetHent(buyDoc->GetHentId());
					row->fromPlace = hentViewCreator.CreateView(hentCO);
				}
				else
				{
					uint32_t firstOwnerId = presence->GetFirstOwnerId();
					if (firstOwnerId != NULL_ID)
					{					
						HentClientObject* firstOwnerCO = pSession->GetHent(firstOwnerId);
						row->fromPlace = hentViewCreator.CreateView(firstOwnerCO);
					}
				}
			}

			switch (herdInDoc->GetDocType())
			{
				case DocType_Buy:
				{
					const IBuyDoc* buyDoc = (const IBuyDoc*)herdInDoc;
					HentClientObject* hentCO = pSession->GetHent(buyDoc->GetHentId());
					row->inWetNo = hentCO->Object()->GetWetNo();
					break;
				}
				case DocType_In:
				{
					row->inWetNo = pSession->GetFarm()->GetWetIdNo();
					break;
				}
			}

			//if (row->inWetNo.empty())
			//	row->inWetNo = pSession->GetFarm()->GetWetIdNo();
		}
		else
		{
			const IMoveDoc* moveDoc = (const IMoveDoc*)herdInDoc;			
			row->fromPlace = farmViewCreator.CreateView(moveDoc->GetSrcHerdId());	
			row->inWetNo = row->fromPlace->GetWetNo();
		}

		const IDoc* herdOutDoc = presence->GetHerdOutDoc();
		if (herdOutDoc)
		{
			if (herdOutDoc->GetDocType() != DocType_Move)
			{				
				const HentView* sellToHent = presenceView->GetSellToHent();

				if (sellToHent)
					row->toPlace = new HentView(*sellToHent);
				else
				{
					if (herdOutDoc->GetDocType() == DocType_Sell)
					{
						const ISellDoc* sellDoc = (const ISellDoc*)herdOutDoc;
						HentClientObject* hentCO = pSession->GetHent(sellDoc->GetHentId());
						row->toPlace = hentViewCreator.CreateView(hentCO);
					}
					else
					{
						row->toPlaceStr = herdOutDoc->GetExtras();
					}
				}

				switch (herdOutDoc->GetDocType())
				{
					case DocType_Sell:
					{
						const ISellDoc* sellDoc = (const ISellDoc*)herdOutDoc;
						HentClientObject* hentCO = pSession->GetHent(sellDoc->GetHentId());
						row->outWetNo = hentCO->Object()->GetWetNo();
						break;
					}
					case DocType_Out:
					{
						row->outWetNo = pSession->GetFarm()->GetWetIdNo();
						break;
					}
				}

				if(row->outWetNo.empty())
					row->outWetNo = pSession->GetFarm()->GetWetIdNo();
			}
			else
			{
				const IMoveDoc* moveDoc = (const IMoveDoc*)herdOutDoc;
				row->toPlace = farmViewCreator.CreateView(moveDoc->GetDstHerdId());
				row->outWetNo = row->toPlace->GetWetNo();
			}
		}


		m_rows.push_back(row);
	}
}



bkprint::PrintOut WeterinarianBook::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	
	WetBookDS ds(this, profile);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));

	return std::move(printOut);
}

void WeterinarianBook::SetContext(DocumentContext* context)
{
	m_pContext = context;
}

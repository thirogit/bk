#include "stdafx.h"
#include "HerdBook.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include "../runtime/Label.h"
#include <string/IntString.h>
#include "../../text/CowSexText.h"
#include "../../text/MotiveString.h"
#include <string\TextUtils.h>
#include "../../view/CowPresenceViewCreator.h"
#include "../options/PrintOptionSerializer.h"
#include "../../bkprint/renderer/TemplateRenderer.h"

std::map<Motive, std::wstring> _reason_translation = {
	{Motive_Birth, L"U"},
	{Motive_Buy, L"Do"},
	{Motive_Sell, L"Z"},
	{Motive_Import, L"P"},
	{Motive_Export, L"W"},
	{Motive_Death, L"PD"},
	{Motive_Kill, L"UB" },
	{Motive_Slaugher, L"UBRZ"}
};


HerdBookOptions::HerdBookOptions(IPrintProfile* profile) : m_recordIndex(1), m_pageIndex(1), m_useSellDocHent(false)
{
	if (!profile) return;	
	m_recordIndex = max(1, GetIndex(L"RECORDINDEX", profile).ToInt());
	m_pageIndex = max(1, GetIndex(L"PAGEINDEX", profile).ToInt());
	if (profile)
	{
		PrintOption* option = profile->GetOption(L"USESELLDOCHENTIFNOINVOICE");
		if (option)
		{
			PrintOptionSerializer serializer;
			m_useSellDocHent = serializer.GetBooleanValue(*option);
		}
	}
}

NullInt HerdBookOptions::GetIndex(const std::wstring& indexName, IPrintProfile* profile)
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

int HerdBookOptions::recordIndex() const
{
	return m_recordIndex;
}

int HerdBookOptions::pageIndex() const
{
	return m_pageIndex;
}

bool HerdBookOptions::useSellDocHent() const
{
	return m_useSellDocHent;
}

//#######################################################################################
HerdBook::HerdBookDS::ItemsBandEvaluator::ItemsBandEvaluator(HerdBookDS* parent) : m_parent(parent)
{

}

bkprint::Values HerdBook::HerdBookDS::ItemsBandEvaluator::Header()
{
	return std::move(bkprint::Values());
}

bkprint::Values HerdBook::HerdBookDS::ItemsBandEvaluator::Footer()
{
	 return std::move(bkprint::Values());
}

size_t HerdBook::HerdBookDS::ItemsBandEvaluator::Count() const
{
	return m_parent->m_parent->m_cows.size();
}

bkprint::Values HerdBook::HerdBookDS::ItemsBandEvaluator::At(int index)
{
	CowPresenceView* row = m_parent->m_parent->m_cows[index];
	return std::move(CreateRowValues(index, row));
}

std::wstring HerdBook::HerdBookDS::ItemsBandEvaluator::FormatHent(const HentView* pHent)
{
	return TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
		pHent->GetName().c_str(),
		pHent->GetHentNo().ToString().c_str(),
		pHent->GetStreet().c_str(),
		pHent->GetPOBox().c_str(),
		pHent->GetZip().c_str(),
		pHent->GetCity().c_str());
}

std::wstring HerdBook::HerdBookDS::ItemsBandEvaluator::TranslateMotive(Motive motive) const
{
	auto it = _reason_translation.find(motive);
	if (it != _reason_translation.end())
	{
		return it->second;
	}
	return L"###";
}


bkprint::Values HerdBook::HerdBookDS::ItemsBandEvaluator::CreateRowValues(size_t nindex, const CowPresenceView* cow)
{
	bkprint::Values values;
	values.Set(L"INDEX", IntString(nindex + 1 + m_parent->m_options.recordIndex()));
	values.Set(L"COWNO", cow->GetCowNo().ToString());
	values.Set(L"BIRTHDT", cow->GetBirthDate().GetDayDate());
	values.Set(L"SEX", CowSexText(cow->GetSex()).ToString());
	values.Set(L"STOCK", cow->GetOwnStockCd());
	const HentView* pFromOwner = cow->GetBuyFromHent();

	if (!pFromOwner)
	{
		pFromOwner = cow->GetBuyDocHent();
	}

	if (pFromOwner)
	{
		values.Set(L"FROMPLACE", FormatHent(pFromOwner));
	}
	
	//values.Set(L"EXTRAS", );
	values.Set(L"MOTHERNO", cow->GetMotherNo().ToString());
	//values.Set(L"FATHERNO", );
	values.Set(L"INDT", cow->GetHerdInDate().GetDayDate());
	values.Set(L"INREASON", TranslateMotive(cow->GetHerdInMotive()));

	if (!cow->IsInStock())
	{
		const HentView* pSellToHent = cow->GetSellToHent();
		if (pSellToHent)
		{
			values.Set(L"TOPLACE", FormatHent(pSellToHent));
		}
		else
		{
			const HentView* pSellDocHent = cow->GetSellDocHent();
			if (m_parent->m_options.useSellDocHent() && pSellDocHent != NULL)
			{
				values.Set(L"TOPLACE", FormatHent(pSellDocHent));
			}
			else
			{
				values.Set(L"TOPLACE", cow->toPlaceStr);
			}
		}

		values.Set(L"CARRIER", cow->carrierStr);
		values.Set(L"OUTDT", cow->GetHerdOutDate().GetDayDate());
		values.Set(L"OUTREASON", TranslateMotive(cow->GetHerdOutMotive()));
	}
	
	
	return std::move(values);
}

//#######################################################################################

HerdBook::HerdBookDS::HerdBookDS(HerdBook* parent, IPrintProfile* profile) : m_parent(parent), m_options(profile)
{
	m_itemsBandEvaluator = new ItemsBandEvaluator(this);	
}

HerdBook::HerdBookDS::~HerdBookDS()
{
	delete m_itemsBandEvaluator;
}

bkprint::Values HerdBook::HerdBookDS::GetHeaderValues()
{
	bkprint::Values heaaderValues;

	//heaaderValues.Set(L"INVOICETITLE", m_options.title());
	

	return std::move(heaaderValues);
}

bkprint::Values HerdBook::HerdBookDS::GetFooterValues()
{	
	bkprint::Values values;
	
	return std::move(values);
}

bkprint::Values HerdBook::HerdBookDS::GetPageHeaderValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::Values HerdBook::HerdBookDS::GetPageFooterValues(int pageCount, int page)
{
	bkprint::Values values;
	return std::move(values);
}

bkprint::BandEvaluator* HerdBook::HerdBookDS::GetBandValues(const std::wstring& bandName)
{
	if (L"items" == bandName)
		return m_itemsBandEvaluator;

	return NULL;
}



//#######################################################################################

HerdBook::HerdBook(bkprint::Template* templ)
{
	m_pTemplate = templ;
}

HerdBook::~HerdBook()
{
	DeepDelete(m_cows);
}

bool HerdBook::QueryInputInterface(const type_info& interfase_type,void** interfase)
{
	if(interfase_type == typeid(KeysInputInterface))
	{
		if(interfase)
			*interfase = (KeysInputInterface*)this;
		return true;
	}
	return false;
}

void HerdBook::Feed(SeasonSession* pSession,const UniquePresenceKeys& keys)
{
	DeepDelete(m_cows);
	m_cows.clear();

	const DocumentTerritory* territory = m_pContext->GetTeritory();
	HerdRegistry* pRegistry = pSession->GetHerdRegistry(territory->GetHerd()->GetId());
	CowPresenceViewCreator viewCreator(pSession);
	BOOST_FOREACH(CowPresenceKey key, keys)
	{
		ICowPresence* presence = pRegistry->GetCowPresence(key);
		CowPresenceView* view = viewCreator.CreateView(presence);


		if (!presence->IsInStock())
		{
			const IDoc* exitDoc = presence->GetHerdOutDoc();

			if (exitDoc->GetDocType() == DocType_Out)
			{
				IOutDoc* outDoc = (IOutDoc*)exitDoc;
				view->toPlaceStr = outDoc->GetExtras();
			}
			else if(exitDoc->GetDocType() == DocType_Sell)
			{
				ISellDoc* sellDoc = (ISellDoc*)exitDoc;
				uint32_t hentId = sellDoc->GetHentId();

				HentClientObject* hentCO = pSession->GetHent(hentId);
				if (hentCO != NULL)
				{
					Hent* pHent = hentCO->Object();
					view->carrierStr = TextUtils::FormatA(L"%s, %s\n%s %s,%s %s",
						pHent->GetName().c_str(),
						pHent->GetWetLicenceNo().c_str(),
						pHent->GetStreet().c_str(),
						pHent->GetPOBox().c_str(),
						pHent->GetZip().c_str(),
						pHent->GetCity().c_str());
					 
				}
			}

		}

		m_cows.push_back(view);
	}

}

bkprint::PrintOut HerdBook::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	
	HerdBookDS ds(this, profile);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
		
	return std::move(printOut);
	
}

void HerdBook::SetContext(DocumentContext* context)
{
	m_pContext = context;
}


#include "stdafx.h"
#include "Foodchain1.h"
#include <arrays\DeepDelete.h>
#include <boost\foreach.hpp>
#include <string\TextUtils.h>
#include "../../bkprint/renderer/TemplateRenderer.h"
#include "../options/PrintOptionSerializer.h"
#include "../../view/CowPresenceViewCreator.h"
#include <data\utils\GetDocInvoice.h>


Foodchain1Options::Foodchain1Options(IPrintProfile* profile) 
{
	if (!profile) return;

	PrintOptionSerializer optionSerializer;

}
//
//const std::wstring& DisinfectionOptions::chemicalAgent() const
//{
//	return m_chemicalAgent;
//}
//
//bool DisinfectionOptions::dontPutPlateNo() const
//{
//	return m_dontPutPlateNo;
//}



//#################################################################################################

Foodchain1::Foodchain1DS::Foodchain1DS(const Foodchain1Record* record, Foodchain1Options* options) : m_record(record), m_options(options)
{
}

int Foodchain1::Foodchain1DS::GetSheetCount()
{
	return 1;
}

bkprint::Values Foodchain1::Foodchain1DS::GetValuesForSheet(int sheet)
{
	bkprint::Values values;
	
	boost::optional<InvoiceHent> buyFromHent = m_record->buyFromHent;

	if (buyFromHent.is_initialized())
	{
		values.Set(L"BUYFROMHENTNAME", buyFromHent->GetName());
		values.Set(L"BUYFROMHENTZIP", buyFromHent->GetZip());
		values.Set(L"BUYFROMHENTCITY", buyFromHent->GetCity());
		values.Set(L"BUYFROMHENTSTREETADDR", buyFromHent->GetStreet() + L" " + buyFromHent->GetPOBox());
		values.Set(L"BUYFROMHENTHENTNO", buyFromHent->GetHentNo().ToString());
	}

	//values.Set(L"DELIVERNO", );
	//values.Set(L"SLAUCHTER,);

	//values.Set(L"DATE", DateTime::now().GetDayDate());
	//values.Set(L"PLACE", L"");
	values.Set(L"COWNO", m_record->cowNo.ToString());
	
	return std::move(values);
}

//#################################################################################################

Foodchain1::HyperFoodchain1DS::HyperFoodchain1DS(Foodchain1* parent, Foodchain1Options* options)
{
	BOOST_FOREACH(const Foodchain1Record* record, parent->m_records)
	{
		m_dataSrcs.push_back(new Foodchain1DS(record, options));
	};
}

Foodchain1::HyperFoodchain1DS::~HyperFoodchain1DS()
{
	DeepDelete(m_dataSrcs);
}

int Foodchain1::HyperFoodchain1DS::GetDSCount()
{
	return m_dataSrcs.size();
}

bkprint::FixedTemplateDataSource* Foodchain1::HyperFoodchain1DS::GetDS(int index)
{
	return m_dataSrcs[index];
}



//#################################################################################################



Foodchain1::Foodchain1(bkprint::Template* templ)
{
	m_pTemplate = templ;	
}

Foodchain1::~Foodchain1()
{
	DeepDelete(m_records);
}


void Foodchain1::Feed(SeasonSession* pSession, const UniquePresenceKeys& keys)
{
	DeepDelete(m_records);
	m_records.clear();
		
	BOOST_FOREACH(CowPresenceKey key, keys)
	{
		uint32_t cowId = key.GetCowId();
		Cow* cow = pSession->GetCow(cowId)->Object();
		uint32_t buyInvoiceId = cow->GetBuyInvoiceId();
		BuyInvoice* buyInvoice = NULL;

		if (buyInvoiceId != NULL_ID)
		{
			buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
		}
		else
		{
			ICowTrail* trail = pSession->GetCowTrail(cowId);
			const ICowFootprint* entryFootprint = trail->GetEntry();
			IDoc* entryDoc = entryFootprint->GetDoc();
			buyInvoiceId = GetDocInvoice(entryDoc);
			if (buyInvoiceId != NULL_ID)
			{
				buyInvoice = pSession->GetBuyInvoice(buyInvoiceId)->Object();
			}
		}

		Foodchain1Record* record = new Foodchain1Record();
		record->cowNo = cow->GetCowNo();
		if (buyInvoice != NULL)
		{
			InvoiceHent invoicehent;
			invoicehent.CopyFrom(buyInvoice->GetInvoiceHent());
			record->buyFromHent = invoicehent;
		}
		m_records.push_back(record);
	}
}

bool Foodchain1::QueryInputInterface(const type_info& interfase_type, void** interfase)
{
	if (typeid(KeysInputInterface) == interfase_type)
	{
		*interfase = (KeysInputInterface*)this;
		return true;
	}

	return false;
}

bkprint::PrintOut Foodchain1::Print(IPrintProfile* profile)
{
	bkprint::PrintOut printOut;
	Foodchain1Options options(profile);
	HyperFoodchain1DS ds(this, &options);
	bkprint::TemplateRenderer renderer(m_pTemplate);
	printOut = std::move(renderer.Print(&ds));
	return std::move(printOut);
}

void Foodchain1::SetContext(DocumentContext* context)
{
	m_pContext = context;
}





#include "stdafx.h"
#include "DocumentManagerService.h"
#include "../bkprint/template/unmarshaler/TemplateUnmarshaller.h"
#include <filesystem\paths.h>
#include <string\TextUtils.h>
#include "DocumentInterface.h"
#include "DocumentPredicate.h"
#include <boost\functional\factory.hpp>
#include <boost\function.hpp>
#include <boost\foreach.hpp>

#include "documents\WeterinarianBook.h"
#include "documents\OwnDocSpecification.h"
#include "documents\HerdBook.h"
#include "documents\Displacement.h"
#include "documents\HentsDocSpecification.h"
#include "documents\TransportRegistry.h"
#include "documents\TransportManifest.h"
#include "documents\InvoiceDocument.h"
#include "documents\WeightSpec.h"
#include "documents\Disinfection.h"
#include "documents/PlusDocReport.h"
#include "documents\HealthCertAttachment.h"
#include "documents\InvoiceSpecification.h"
#include "documents\InvoicesSummary.h"
#include "documents\Foodchain1.h"
#include "documents\Foodchain2.h"
#include "documents\Foodchain3.h"
#include "documents\NormalizedFoodchain.h"
#include "documents\TransportManifest2.h"
#include "documents\DisplacementViaMarket.h"
#include "documents\Contract.h"
#include "documents\ContractAttachment.h"

#include "options\DocumentOptionType.h"
#include "../resource.h"
#include "../utils/StringLoader.h"
#include <arrays/DeepDelete.h>

typedef boost::function<DocumentInterface* (bkprint::Template* const &)> DocumentFactory;
typedef boost::function<DocumentPredicate* ()> PredicateFactory;

#define FACTORY(TYPE) boost::factory<TYPE*>()

typedef struct tagDOCUMENT_OPTION
{
	const wchar_t* code;
	DocumentOptionType type;
	const wchar_t* name;
	const wchar_t* value;
	const wchar_t* hint;
} DOCUMENT_OPTION;

typedef struct tagDOCUMENT_SORTCRITERION
{
	const wchar_t* field_code;
	UINT field_name;
} DOCUMENT_SORTCRITERION;

typedef struct tagDOUMENT_DEFINITION
{
	const wchar_t* documentCode;
	const wchar_t* documentName;
	DocumentFactory factory;
	PredicateFactory predicate;
	const wchar_t* templateName;
	DocumentInput inputType;
	const DOCUMENT_OPTION* options;
	const DOCUMENT_SORTCRITERION* order;

} DOCUMENT_DEFINITION;

typedef struct tagTEMPLATE_VARABLE
{
	const wchar_t* name;
	const wchar_t* value;
} TEMPLATE_VARABLE;

const DOCUMENT_SORTCRITERION _wetbook_order[] =
{
	{ L"COWNO", IDS_HEADER_COWNO },
	{ L"INDATE", IDS_HEADER_HERDINDATE },
	{ L"OUTDATE", IDS_HEADER_HERDOUTDATE },
	NULL
};

const DOCUMENT_OPTION _wetbook_options[] =
{
	{ L"RECORDINDEX",		DocumentOptionType_Integer,		L"Numer kolejny", L"1" },	
	NULL
};

const DOCUMENT_OPTION _transreg_options[] =
{
	{ L"DONOTFILLCARRIER", DocumentOptionType_Boolean, L"Nie wypełniaj przewoźnika", L"n" },
	NULL
};

const DOCUMENT_OPTION _weighting_options[] =
{
	{ L"DONOTFILLDELIVER", DocumentOptionType_Boolean, L"Nie wypełniaj dostawcy", L"n" },
	NULL
};

const DOCUMENT_OPTION _herdbook_options[] =
{
	{ L"RECORDINDEX", DocumentOptionType_Integer, L"Numer kolejny", L"1" },
	{ L"PAGEINDEX", DocumentOptionType_Integer, L"Numer strony", L"1" },
    { L"USESELLDOCHENTIFNOINVOICE", DocumentOptionType_Boolean, L"Użyj kontrahenta z WZtki", L"n" },
	NULL
};

const DOCUMENT_OPTION _displacement_options[] =
{
	{ L"EVENTS", DocumentOptionType_DisplacementEvents, L"Typ zdarzenia", L"0" },	
	{ L"NOFROM", DocumentOptionType_Boolean, L"Nie wpisuj Z", L"n" },
	{ L"NOTO", DocumentOptionType_Boolean, L"Nie wpisuj Do", L"n" },
	{ L"NOPRINTDATE", DocumentOptionType_Boolean, L"Nie wpisuj Daty wystawienia", L"n" },
	{ L"PRINTDATE", DocumentOptionType_DayDate, L"Data wystawienia", L"" },
	NULL
};

const DOCUMENT_SORTCRITERION _displacement_order[] =
{
	{ L"COWNO", IDS_HEADER_COWNO },
	NULL
};


const DOCUMENT_OPTION _invoice_options[] =
{
	{ L"TITLE",					DocumentOptionType_String,			L"Tytuł faktury",					L"" },
	{ L"PRICEFMT",				DocumentOptionType_DecimalFormat,	L"Format ceny",						L".2" },
	{ L"WEIGHTFMT",				DocumentOptionType_DecimalFormat,	L"Format wagi",						L".3" },
	{ L"PRICEPERKGFMT",			DocumentOptionType_DecimalFormat,	L"Format ceny za kg",				L".5" },
	{ L"ENTRYFMT", DocumentOptionType_String, L"Format pozycji faktury", L"{TOWAR} {KOLCZYK}, {PLEC}", L"Obsługiwane elementy formatu: {TOWAR},{KOLCZYK},{PLEC},{ILOSC},{RASA}" },
	{ L"EXTRASTEMPLATE",		DocumentOptionType_String,			L"Szablon adnotacji",				L"" },
	{ L"GOODSCATEGORY",			DocumentOptionType_String,			L"PKWiU",							L"" },
	{ L"GOODSDECLINATION1",		DocumentOptionType_String,			L"Liczebnik (1) nazwy towaru",		L"" },
	{ L"GOODSDECLINATION2_4",	DocumentOptionType_String,			L"Liczebnik (2-4) nazwy towaru",	L"" },
	{ L"GOODSDECLINATION4+",	DocumentOptionType_String,			L"Liczebnik (>4) nazwy towaru",		L"" },	
	NULL
};


const DOCUMENT_OPTION _transportmanifest_options[] =
{
	{ L"SPIECIE", DocumentOptionType_String, L"Gatunek", L"" },
	NULL
};

const DOCUMENT_OPTION _normfoodchain_options[] =
{
	{ L"SPIECIE", DocumentOptionType_String, L"Gatunek", L"" },	
	NULL
};

const DOCUMENT_OPTION _foodchain2_options[] =
{
	{ L"SPIECIE", DocumentOptionType_String, L"Gatunek", L"" },
	{ L"SLAUGHTER", DocumentOptionType_String, L"Rzeźnia", L"" },
	{ L"PLATENO", DocumentOptionType_String, L"Numer rejestracyjny", L"" },
	NULL
};

const DOCUMENT_OPTION _foodchain3_options[] =
{
	{ L"SPIECIE", DocumentOptionType_String, L"Gatunek", L"" },
	{ L"SLAUGHTER", DocumentOptionType_String, L"Rzeźnia", L"" },
	//{ L"PLATENO", DocumentOptionType_String, L"Numer rejestracyjny", L"" },
	NULL
};

const DOCUMENT_OPTION _transmainf2_options[] =
{
	{ L"SPIECIE", DocumentOptionType_String, L"Gatunek", L"" },
	NULL
};


const DOCUMENT_OPTION _displviamarket_options[] =
{
	{ L"MARKETNO", DocumentOptionType_HentNo, L"Numer targu", L"" },
	{ L"FROMNO", DocumentOptionType_HentNo, L"Źródłowy numer stada", L"" },
	{ L"TONO", DocumentOptionType_HentNo, L"Docelowy numer stada", L"" },
	NULL
};

const DOCUMENT_OPTION _disinfection_options[] =
{
	{ L"CHEMAGENT", DocumentOptionType_String, L"Środek chemiczny", L"" },
	{ L"NOPLATENO", DocumentOptionType_Boolean, L"Nie wpisuj numeru rejestracyjnego", L"" },
	NULL
};

const DOCUMENT_OPTION _contractattachment_options[] =
{
	{ L"CONTRACTDT", DocumentOptionType_DayDate, L"Data umowy", L"" },
	{ L"DELIVERYDT", DocumentOptionType_DayDate, L"Data dostawy", L"" },
	{ L"FARMER", DocumentOptionType_String, L"Kontraktujący", L"" },
NULL
};


const DOCUMENT_DEFINITION _DocumentDefinitions[] =
{
	
	{ L"HERDBOOK",					L"Ksiega stada",								FACTORY(HerdBook),										NULL, L"herdbook", DocumentInput_Cow, _herdbook_options },
	{ L"WETBOOK",					L"Ksiazka weterynaryjna",						FACTORY(WeterinarianBook),								NULL,	L"wetbook",		DocumentInput_Cow,
		_wetbook_options,
		_wetbook_order
	},
	{ L"DISPLACE4FROM",				L"Zgloszenie przemieszczenia (pocztowka)",		FACTORY(SellDocDisplacementNotificationPostcard),		NULL, L"ZPBOK_1_11", DocumentInput_SellDoc, _displacement_options },
	{ L"DISPLACE4TO",				L"Zgloszenie przemieszczenia (pocztowka)",		FACTORY(BuyDocDisplacementNotificationPostcard),		NULL, L"ZPBOK_1_11", DocumentInput_BuyDoc, _displacement_options },
	{ L"DISPLACE4MOVE",				L"Zgloszenie przemieszczenia (pocztowka)",		FACTORY(MoveDocDisplacementNotificationPostcard),		NULL, L"ZPBOK_1_11", DocumentInput_MoveDoc, _displacement_options },
	{ L"DISPLACE60FROM",			L"Zgloszenie przemieszczenia 60",				FACTORY(SellDocDisplacementNotification60),				NULL, L"ZPBOK_3_11", DocumentInput_SellDoc, _displacement_options },
	{ L"DISPLACE60TO",				L"Zgloszenie przemieszczenia 60",				FACTORY(BuyDocDisplacementNotification60),				NULL, L"ZPBOK_3_11", DocumentInput_BuyDoc, _displacement_options },
	{ L"DISPLACE60MOVE",			L"Zgloszenie przemieszczenia 60",				FACTORY(MoveDocDisplacementNotification60),				NULL, L"ZPBOK_3_11", DocumentInput_MoveDoc, _displacement_options },
	{ L"DISPLACE30FROM",			L"Zgloszenie przemieszczenia 30",				FACTORY(SellDocDisplacementNotification30),				NULL, L"ZPBOK_4_11", DocumentInput_SellDoc, _displacement_options },
	{ L"DISPLACE30TO",				L"Zgloszenie przemieszczenia 30",				FACTORY(BuyDocDisplacementNotification30),				NULL, L"ZPBOK_4_11", DocumentInput_BuyDoc, _displacement_options },
	{ L"DISPLACE30MOVE",			L"Zgloszenie przemieszczenia 30",				FACTORY(MoveDocDisplacementNotification30),				NULL, L"ZPBOK_4_11", DocumentInput_MoveDoc, _displacement_options },
	{ L"DISPLACE4BUY",				L"Zgloszenie przemieszczenia (pocztowka)",		FACTORY(BuyInvoiceDisplacementNotificationPostcard),	NULL, L"ZPBOK_1_11", DocumentInput_BuyInvoice, _displacement_options },
	{ L"DISPLACE4SELL",				L"Zgloszenie przemieszczenia (pocztowka)",		FACTORY(SellInvoiceDisplacementNotificationPostcard),	NULL, L"ZPBOK_1_11", DocumentInput_SellInvoice, _displacement_options },
	{ L"DISPLACE30BUY",				L"Zgloszenie przemieszczenia 30",				FACTORY(BuyInvoiceDisplacementNotification30),			NULL, L"ZPBOK_4_11", DocumentInput_BuyInvoice, _displacement_options },
	{ L"DISPLACE30SELL",			L"Zgloszenie przemieszczenia 30",				FACTORY(SellInvoiceDisplacementNotification30),			NULL, L"ZPBOK_4_11", DocumentInput_SellInvoice, _displacement_options },
	{ L"DISPLACE60BUY",				L"Zgloszenie przemieszczenia 60",				FACTORY(BuyInvoiceDisplacementNotification60),			NULL, L"ZPBOK_3_11", DocumentInput_BuyInvoice, _displacement_options },
	{ L"DISPLACE60SELL",			L"Zgloszenie przemieszczenia 60",				FACTORY(SellInvoiceDisplacementNotification60),			NULL, L"ZPBOK_3_11", DocumentInput_SellInvoice, _displacement_options },	
	{ L"BUYDOCSPEC",				L"Specyfikacja przyjecia zew.",					FACTORY(BuyDocSpecification),							NULL, L"hentsdoc", DocumentInput_BuyDoc },
	{ L"SELLDOCSPEC",				L"Specyfikacja wydania zew.",					FACTORY(SellDocSpecification),							NULL, L"hentsdoc", DocumentInput_SellDoc },
	{ L"INDOCSPEC",					L"Specyfikacja przyjecia wew.",					FACTORY(InDocSpecification),							NULL, L"owndoc", DocumentInput_InDoc },
	{ L"OUTDOCSPEC",				L"Specyfikacja wydania wew.",					FACTORY(OutDocSpecification),							NULL, L"owndoc", DocumentInput_OutDoc },
	{ L"BUYDOCTRANSREG",			L"Rejestr przewozonych zwierzat",				FACTORY(BuyDocTransportRegistry),						NULL, L"transreg", DocumentInput_BuyDoc, _transreg_options },
	{ L"SELLDOCTRANSREG",			L"Rejestr przewozonych zwierzat",				FACTORY(SellTransportRegistry),							NULL, L"transreg", DocumentInput_SellDoc, _transreg_options },
	{ L"MOVDOCTRANSMANIF",			L"Informacje dot. przesylki zwierzat",			FACTORY(MoveDocTransportManifest),						NULL, L"transmanif", DocumentInput_MoveDoc, _transportmanifest_options },
	{ L"SELLDOCTRANSREG",			L"Informacje dot. przesylki zwierzat",			FACTORY(SellTransportManifest),							NULL, L"transmanif", DocumentInput_SellDoc, _transportmanifest_options },
	{ L"BUYVATINVOICE",				L"Faktura VAT szczegolowa na sztuki",			FACTORY(BuyDetailedPieceInvoiceDocument),				NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"BUYVATINVOICE_KG",			L"Faktura VAT szczegolowa na wage",				FACTORY(BuyDetailedWeightInvoiceDocument),				NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"SELLVATINVOICE",			L"Faktura VAT szczegolowa",						FACTORY(SellDetailedPieceInvoiceDocument),				NULL, L"regularinvoice_sell", DocumentInput_SellInvoice, _invoice_options },
	{ L"REFWEIGHTSPECBUYDOC",		L"Wazenie referencyjne",						FACTORY(BuyDocWeightSpecification),						NULL, L"refweightspec", DocumentInput_BuyDoc, _weighting_options },
	{ L"REFWEIGHTSPECSELLDOC",		L"Wazenie referencyjne",						FACTORY(SellDocWeightSpecification),					NULL, L"refweightspec", DocumentInput_SellDoc, _weighting_options },
	{ L"WEIGHTSPECBUYDOC",			L"Wazenie",										FACTORY(BuyDocWeightSpecification),						NULL, L"weightspec", DocumentInput_BuyDoc, _weighting_options },
	{ L"REFWEIGHTSPECSELLDOC",		L"Wazenie",										FACTORY(SellDocWeightSpecification),					NULL, L"weightspec", DocumentInput_SellDoc, _weighting_options },
	{ L"DISINFECBUY",				L"Dezynfekcja",									FACTORY(BuyDocDisinfectionDocument),					NULL, L"disinfection", DocumentInput_BuyDoc, _disinfection_options },
	{ L"DISINFECSELL",				L"Dezynfekcja",									FACTORY(SellDocDisinfectionDocument),					NULL, L"disinfection", DocumentInput_SellDoc, _disinfection_options },
	{ L"INDOCREPORT",				L"Raport z przyjecia",							FACTORY(InDocReport),									NULL, L"PLUSDOCREPORT", DocumentInput_InDoc, NULL },
	{ L"BUYDOCREPORT",				L"Raport z przyjecia",							FACTORY(BuyDocReport),									NULL, L"PLUSDOCREPORT", DocumentInput_BuyDoc, NULL },
	{ L"OUTDOCHEALTHCERT",			L"Zalacznik do swiadectwa zdrowia",				FACTORY(SellDocHealthCertAttachment),					NULL, L"HEALTHCERT", DocumentInput_SellDoc, NULL },
	{ L"SELLDOCHEALTHCERT",			L"Zalacznik do swiadectwa zdrowia",				FACTORY(OutDocHealthCertAttachment),					NULL, L"HEALTHCERT", DocumentInput_OutDoc, NULL },
	{ L"BUYINVOICEINTSPEC",			L"Specyfikacja faktury wew.",					FACTORY(BuyInvoiceSpecification),						NULL, L"BUYINVOICESPECINTERNAL", DocumentInput_BuyInvoice, NULL },
	{ L"SELLLINVOICEINTSPEC",		L"Specyfikacja faktury wew.",					FACTORY(SellInvoiceSpecification),						NULL, L"SELLINVOICESPECINTERNAL", DocumentInput_SellInvoice, NULL },
	{ L"BUYINVSUMMARY",				L"Zestawienie faktur kupna",					FACTORY(BuyInvoicesSummaryDocument),					NULL, L"INVSUMMARY", DocumentInput_BuyInvoice, NULL },
	{ L"SELLLINVSUMMARY",			L"Zestawienie faktur sprzedazy",				FACTORY(SellInvoicesSummaryDocument),					NULL, L"INVSUMMARY", DocumentInput_SellInvoice, NULL },
	{ L"BUYINVOICEEXTSPEC",			L"Specyfikacja faktury zew.",					FACTORY(BuyInvoiceSpecification),						NULL, L"INVOICESPECEXTERNAL", DocumentInput_BuyInvoice, NULL },
	{ L"SELLLINVOICEEXTSPEC",		L"Specyfikacja faktury zew.",					FACTORY(SellInvoiceSpecification),						NULL, L"INVOICESPECEXTERNAL", DocumentInput_SellInvoice, NULL },
	{ L"BUYVATINVGRPPIECE",			L"Faktura VAT grupowa na sztuki",				FACTORY(BuyClassPieceInvoiceDocument),					NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"SELLVATINVGRPPIECE",		L"Faktura VAT grupowa na sztuki",				FACTORY(SellClassPieceInvoiceDocument),					NULL, L"regularinvoice_sell", DocumentInput_SellInvoiceRegular, _invoice_options },
	{ L"BUYVATINVGRPWEIGHT",		L"Faktura VAT grupowa na wage",					FACTORY(BuyClassWeightInvoiceDocument),					NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"SELLVATINVGRPWEIGHT",		L"Faktura VAT grupowa na wage",					FACTORY(SellClassWeightInvoiceDocument),				NULL, L"regularinvoice_sell", DocumentInput_SellInvoiceRegular, _invoice_options },
	{ L"BUYVATINVCOMPACTPIECE",		L"Faktura VAT zespolona na sztuki",				FACTORY(BuyCompactPieceInvoiceDocument),				NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"SELLVATINVCOMPACTPIECE",	L"Faktura VAT zaspolona na sztuki",				FACTORY(SellCompactPieceInvoiceDocument),				NULL, L"regularinvoice_sell", DocumentInput_SellInvoiceRegular, _invoice_options },
	{ L"BUYVATINVCOMPACTWEIGHT",	L"Faktura VAT zespolona na wage",				FACTORY(BuyCompactWeightInvoiceDocument),				NULL, L"regularinvoice_buy", DocumentInput_BuyInvoiceRegular, _invoice_options },
	{ L"SELLVATINVCOMPACTWEIGHT",	L"Faktura VAT grupowa na wage",					FACTORY(SellCompactWeightInvoiceDocument),				NULL, L"regularinvoice_sell", DocumentInput_SellInvoiceRegular, _invoice_options },
	{ L"FOODCHAIN1",				L"Łańcuch żywieniowy - Sokołów",				FACTORY(Foodchain1),									NULL, L"foodchain1", DocumentInput_Cow, NULL },
	{ L"NORMFOODCHAIN",				L"Znormalizowany łańcuch żywieniowy",			FACTORY(BuyNormalizedFoodchain),						NULL, L"normfoodchain", DocumentInput_BuyInvoice, _normfoodchain_options },
	{ L"BUYLUMPINVPIECE",			L"Faktura RR sztuki",							FACTORY(BuyDetailedPieceInvoiceDocument),				NULL, L"lumpinvoice", DocumentInput_BuyInvoiceLump, _invoice_options },
	{ L"BUYLUMPINVWEIGHT",			L"Faktura RR waga",								FACTORY(BuyDetailedWeightInvoiceDocument),				NULL, L"lumpinvoice", DocumentInput_BuyInvoiceLump, _invoice_options },
	{ L"SELLFOODCHAIN2",			L"Informacje dot. łańcucha żywnościowego",		FACTORY(SellDocFoodchain2),								NULL, L"foodchain2", DocumentInput_SellDoc, _foodchain2_options },
	{ L"BUYFOODCHAIN2",				L"Informacje dot. łańcucha żywnościowego",		FACTORY(BuyDocFoodchain2),								NULL, L"foodchain2", DocumentInput_BuyDoc, _foodchain2_options },
	{ L"MOVDOCTRANSMANIF2",			L"Dokument przewozowy",							FACTORY(MoveDocTransportManifest2),						NULL, L"transmanif2", DocumentInput_MoveDoc, _transmainf2_options },
	{ L"SELLDOCTRANSMAIN2",			L"Dokument przewozowy",							FACTORY(SellTransportManifest2),						NULL, L"transmanif2", DocumentInput_SellDoc, _transmainf2_options },
	{ L"BUYDOCTRANSMAIN2",			L"Dokument przewozowy",							FACTORY(BuyTransportManifest2),							NULL, L"transmanif2", DocumentInput_BuyDoc, _transmainf2_options },
	{ L"SELLFOODCHAIN3",			L"Łańcuch żywieniowy",							FACTORY(SellDocFoodchain3),								NULL, L"foodchain3", DocumentInput_SellDoc, _foodchain3_options },
	{ L"DISPLACEVIAMARKET",			L"Zgłoszenie przemieszczenia przez targ",		FACTORY(DisplacementViaMarketDocument),					NULL, L"ZPBPTA", DocumentInput_Cow, _displviamarket_options },
	{ L"CONTRACTLUMPINVOICE",		L"Umowa na dostarczenie produktow rolnych",		FACTORY(ContractDocument),								NULL, L"contract", DocumentInput_BuyInvoice, NULL },
	{ L"CONTRACTATTACHMENT",		L"Załącznik do umowy",							FACTORY(OutDocContractAttachment),						NULL, L"contractattachment", DocumentInput_OutDoc, _contractattachment_options },
	{ L"BUYLUMPINVCOMPACTPIECE",	L"Faktura RR zespolona na sztuki",				FACTORY(BuyCompactPieceInvoiceDocument),				NULL, L"lumpinvoice", DocumentInput_BuyInvoiceLump, _invoice_options },
	{ L"BUYLUMPINVCOMPACTWEIGHT",	L"Faktura RR zespolona na wage",				FACTORY(BuyCompactWeightInvoiceDocument),				NULL, L"lumpinvoice", DocumentInput_BuyInvoiceLump, _invoice_options },























//	{ID_DOC_BUYVATINVGRPPRICEPIECE,		IDS_DOCNAME_VATINVGRPPRICEPIECE,	RUNTIME_CLASS(CFakturaVATGroupPricePerKgPieceA4),	"FAKTURAVAT",			DATATYPE_BUYVATINVS,	CFGID_BUYINVVATGRPPRICEPIECE,	RUNTIME_CLASS(CInvoiceDocOptionsDlg),		RUNTIME_CLASS(CCowDrwDocView), false}, 
//	{ID_DOC_SELLVATINVGRPPRICEPIECE,	IDS_DOCNAME_VATINVGRPPRICEPIECE,	RUNTIME_CLASS(CFakturaVATGroupPricePerKgPieceA4),	"FAKTURAVAT",			DATATYPE_SELLVATINVS,	CFGID_SELLINVVATGRPPRICEPIECE,	RUNTIME_CLASS(CInvoiceDocOptionsDlg),		RUNTIME_CLASS(CCowDrwDocView), false}, 
//	{ID_DOC_BUYVATINVGRPPRICEWEIGHT,	IDS_DOCNAME_VATINVGRPPRICEWEIGHT,	RUNTIME_CLASS(CFakturaVATGroupPricePerKgWeightA4),	"FAKTURAVAT",			DATATYPE_BUYVATINVS,	CFGID_BUYINVVATGRPPRICEWEIGHT,	RUNTIME_CLASS(CInvoiceDocOptionsDlg),		RUNTIME_CLASS(CCowDrwDocView), false}, 
//	{ID_DOC_SELLVATINVGRPPRICEWEIGHT,	IDS_DOCNAME_VATINVGRPPRICEWEIGHT,	RUNTIME_CLASS(CFakturaVATGroupPricePerKgWeightA4),	"FAKTURAVAT",			DATATYPE_SELLVATINVS,	CFGID_SELLINVVATGRPPRICEWEIGHT,	RUNTIME_CLASS(CInvoiceDocOptionsDlg),		RUNTIME_CLASS(CCowDrwDocView), false}, 

	{ NULL, NULL, NULL, NULL, NULL, DocumentInput_None, { NULL } }
};

void DocumentManagerService::Start()
{
	const DOCUMENT_DEFINITION* definition = _DocumentDefinitions;
	for (; definition->documentCode != NULL; definition++)
	{		
		bkprint::Template* templ = ObtainTemplate(definition->templateName);

		if (!templ) continue;

		DocumentArchetype* archetype = new DocumentArchetype();
		archetype->m_code = definition->documentCode;
		archetype->m_name = definition->documentName;
		archetype->m_inputType = definition->inputType;
		archetype->m_template = templ;
		archetype->m_implFactory = definition->factory;

		if (!definition->predicate.empty())
		{
			archetype->m_predicate.reset(definition->predicate());
		}

		archetype->m_cfgDescPtr.reset(new DocumentCfgDescription());
		boost::shared_ptr<DocumentCfgDescription> cfgDescPtr = archetype->m_cfgDescPtr;

		if (definition->options)
		{
			const DOCUMENT_OPTION* option = definition->options;
			while (option->code != NULL)
			{
				DocumentOptionDefinition optionDefinition(option->code, option->type);
				
				optionDefinition.SetName(option->name);
				optionDefinition.SetDefualtValue(option->value);
				cfgDescPtr->AddOptionDefinition(optionDefinition);
				
				option++;
			}

			const DOCUMENT_SORTCRITERION* sortcriterion = definition->order;
			if (sortcriterion)
			{
				while (sortcriterion->field_code != NULL)
				{
					ComparableFieldDefinition cmdFieldDefinition(sortcriterion->field_code);
					cmdFieldDefinition.SetName(StringLoader(sortcriterion->field_name));
					cfgDescPtr->AddComparableFieldDefinition(cmdFieldDefinition);
					sortcriterion++;
				}
			}
		}

		m_archetypes.push_back(archetype);
		
	}

}

void DocumentManagerService::Init(ServiceRegistry* registry)
{
	m_logoService = registry->FindByType<LogoService>((LogoService*)0);
}

void DocumentManagerService::DestroyArchetypes()
{
	DeepDelete(m_archetypes);
	m_archetypes.clear();
}

void DocumentManagerService::Stop()
{
	DestroyArchetypes();
}

DocumentSelection DocumentManagerService::FindDocuments(uint32_t inpuTypeMask)
{
	DocumentSelection selection;

	BOOST_FOREACH(DocumentArchetype* archetype, m_archetypes)
	{
		if (archetype->m_inputType & inpuTypeMask)
		{
			selection.m_archetypes.push_back(archetype);
		}
	}

	return std::move(selection);
}

DocumentManagerService::~DocumentManagerService()
{
	DestroyArchetypes();
	DeepDelete(m_templates);
}

bkprint::Template* DocumentManagerService::LoadTemplate(const std::wstring& templateName)
{
	bkprint::TemplateUnmarshaller unmarshaller;

	if (m_logoService != NULL)
	{
		unmarshaller.SetLogo(m_logoService->GetLogo());
	}
	
	std::wstring templateDir = paths_ConcatPath(paths_GetApplicationDir(), L"drw");
	std::wstring templateFile = templateName;
	templateFile += L".XML";

	std::wstring templatePath = paths_ConcatPath(templateDir, templateFile);

	if (paths_DoesPathExist(templatePath))
	{
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_file(templatePath.c_str());
		bkprint::Template* templ = unmarshaller.Unmarshal(doc.root().first_child());

		return templ;
	}
	return NULL;
}

bkprint::Template* DocumentManagerService::ObtainTemplate(const std::wstring& templateName)
{
	auto it = m_templates.find(templateName);
	if (it != m_templates.end())
	{
		return it->second;
	}
	else
	{
		bkprint::Template* templ = LoadTemplate(templateName);
		if (templ != NULL)
		{
			m_templates[templateName] = templ;
			return templ;
		}
	}
	return NULL;
}
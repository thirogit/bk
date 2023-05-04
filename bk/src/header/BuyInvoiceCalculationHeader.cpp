#include "stdafx.h"
#include "BuyInvoiceCalculationHeader.h"
#include "../utils/StringLoader.h"
#include "../resource.h"
#include "../invoice/view/CalculationElementView.h"

BuyInvoiceCalculationHeader::BuyInvoiceCalculationHeader()
{
	operator()  (CalculationElementViewMember::CalculationElementView_HasTerm,L"*")
				(CalculationElementViewMember::CalculationElementView_CowNo,StringLoader(IDS_HEADER_COWNO))
				(CalculationElementViewMember::CalculationElementView_Class,StringLoader(IDS_HEADER_CLASS))
				(CalculationElementViewMember::CalculationElementView_Weight,StringLoader(IDS_HEADER_WEIGHT))
				(CalculationElementViewMember::CalculationElementView_OtherHent,StringLoader(IDS_HEADER_SELLTOHENT))
				(CalculationElementViewMember::CalculationElementView_OtherPrice,StringLoader(IDS_HEADER_SELLPRICE))
			//	(CalculationElementViewMember::CalculationElementView_Rule,StringLoader(IDS_))
				(CalculationElementViewMember::CalculationElementView_NetPricePerKg,StringLoader(IDS_HERADER_NETPRICEPERKG))
				(CalculationElementViewMember::CalculationElementView_NetPrice,StringLoader(IDS_HEADER_NETPRICE))
				(CalculationElementViewMember::CalculationElementView_TaxValue,StringLoader(IDS_HEADER_TAXVALUE))
				(CalculationElementViewMember::CalculationElementView_GrossPrice,StringLoader(IDS_HEADER_GROSSPRICE));	
				;
}

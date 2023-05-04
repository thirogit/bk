#include "stdafx.h"
#include "CowPresenceListHeader.h"
#include "../view/CowPresenceView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

CowPresenceListHeader::CowPresenceListHeader()
{
	operator() (CowPresenceViewMember::State, StringLoader(IDS_HEADER_STATE))
				(CowPresenceViewMember::CowNo,			StringLoader(IDS_HEADER_COWNO))
				(CowPresenceViewMember::PassNo,			StringLoader(IDS_HEADER_PASSNO))	
				(CowPresenceViewMember::FirstOwner,		StringLoader(IDS_HEADER_FSTOWNER))
				(CowPresenceViewMember::BirthDate,		StringLoader(IDS_HEADER_BIRTHDATE))
				(CowPresenceViewMember::Weight,			StringLoader(IDS_HEADER_WEIGHT))
				(CowPresenceViewMember::Sex,			StringLoader(IDS_HEADER_SEX))
				(CowPresenceViewMember::StockCd,		StringLoader(IDS_HEADER_STOCK))
				(CowPresenceViewMember::ClassCd,		StringLoader(IDS_HEADER_CLASS))
				(CowPresenceViewMember::Extras,			StringLoader(IDS_HEADER_EXTRAS))
				(CowPresenceViewMember::BuyFromHent,	StringLoader(IDS_HEADER_BUYFROMHENT))
				(CowPresenceViewMember::BuyStock,		StringLoader(IDS_HEADER_BUYSTOCK))
				(CowPresenceViewMember::BuyPrice,		StringLoader(IDS_HEADER_BUYPRICE))
				(CowPresenceViewMember::BuyWeight,		StringLoader(IDS_HEADER_BUYWEIGHT))
				(CowPresenceViewMember::HerdInDocNo,	StringLoader(IDS_HEADER_HERDINDOCNO))
				(CowPresenceViewMember::HerdInDate,		StringLoader(IDS_HEADER_HERDINDATE))
				(CowPresenceViewMember::HerdInGrp,		StringLoader(IDS_HEADER_HERDINGRP))
				(CowPresenceViewMember::HerdInClass,	StringLoader(IDS_HEADER_HERDINCLASS))
				(CowPresenceViewMember::HerdInWeight,	StringLoader(IDS_HEADER_HERDINWEIGHT))			
				(CowPresenceViewMember::SellToHent, StringLoader(IDS_HEADER_SELLTOHENT))
				(CowPresenceViewMember::SellStock, StringLoader(IDS_HEADER_SELLSTOCK))
				(CowPresenceViewMember::SellPrice, StringLoader(IDS_HEADER_SELLPRICE))
				(CowPresenceViewMember::SellWeight, StringLoader(IDS_HEADER_SELLWEIGHT))
				(CowPresenceViewMember::HerdOutDocNo, StringLoader(IDS_HEADER_HERDOUTDOCNO))
				(CowPresenceViewMember::HerdOutDate, StringLoader(IDS_HEADER_HERDOUTDATE))
				(CowPresenceViewMember::HerdOutGrp, StringLoader(IDS_HEADER_HERDOUTGRP))
				(CowPresenceViewMember::HerdOutClass, StringLoader(IDS_HEADER_HERDOUTCLASS))
				(CowPresenceViewMember::HerdOutWeight, StringLoader(IDS_HEADER_HERDOUTWEIGHT))
				(CowPresenceViewMember::BuyInvoiceNo, StringLoader(IDS_HEADER_BUYINVOICE))
				(CowPresenceViewMember::SellInvoiceNo, StringLoader(IDS_HEADER_SELLINVOICE))
				(CowPresenceViewMember::TermBuyClass, StringLoader(IDS_HEADER_TERMBUYCLASS))
				(CowPresenceViewMember::TermSellClass, StringLoader(IDS_HEADER_TERMSELLCLASS))
				(CowPresenceViewMember::TermBuyWeight, StringLoader(IDS_HEADER_TERMBUYWEIGHT))
				(CowPresenceViewMember::TermSellWeight, StringLoader(IDS_HEADER_TERMSELLWEIGHT))
				(CowPresenceViewMember::TermBuyPrice, StringLoader(IDS_HEADER_TERMBUYPRICE))
				(CowPresenceViewMember::TermSellPrice, StringLoader(IDS_HEADER_TERMSELLPRICE))
				(CowPresenceViewMember::HerdInMotive, StringLoader(IDS_HEADER_HERDINMOTIVE))
				(CowPresenceViewMember::HerdOutMotive, StringLoader(IDS_HEADER_HERDOUTMOTIVE))
				(CowPresenceViewMember::BuyDocHent, StringLoader(IDS_HEADER_BUYDOCHENT))
		;	
				

}

#include "stdafx.h"
#include "CowInStockPresenceListHeader.h"
#include "../view/CowPresenceView.h"
#include "../utils/StringLoader.h"
#include "../resource.h"

CowInStockPresenceListHeader::CowInStockPresenceListHeader()
{
	operator()	(CowPresenceViewMember::CowNo, StringLoader(IDS_HEADER_COWNO))
		(CowPresenceViewMember::PassNo, StringLoader(IDS_HEADER_PASSNO))
		(CowPresenceViewMember::FirstOwner, StringLoader(IDS_HEADER_FSTOWNER))
		(CowPresenceViewMember::BirthDate, StringLoader(IDS_HEADER_BIRTHDATE))
		(CowPresenceViewMember::Weight, StringLoader(IDS_HEADER_WEIGHT))
		(CowPresenceViewMember::Sex, StringLoader(IDS_HEADER_SEX))
		(CowPresenceViewMember::StockCd, StringLoader(IDS_HEADER_STOCK))
		(CowPresenceViewMember::ClassCd, StringLoader(IDS_HEADER_CLASS))
		(CowPresenceViewMember::Extras, StringLoader(IDS_HEADER_EXTRAS))
		(CowPresenceViewMember::BuyFromHent, StringLoader(IDS_HEADER_BUYFROMHENT))
		(CowPresenceViewMember::BuyStock, StringLoader(IDS_HEADER_BUYSTOCK))
		(CowPresenceViewMember::BuyPrice, StringLoader(IDS_HEADER_BUYPRICE))
		(CowPresenceViewMember::BuyWeight, StringLoader(IDS_HEADER_BUYWEIGHT))
		(CowPresenceViewMember::HerdInDocNo, StringLoader(IDS_HEADER_HERDINDOCNO))
		(CowPresenceViewMember::HerdInDate, StringLoader(IDS_HEADER_HERDINDATE))
		(CowPresenceViewMember::HerdInGrp, StringLoader(IDS_HEADER_HERDINGRP))
		(CowPresenceViewMember::HerdInClass, StringLoader(IDS_HEADER_HERDINCLASS))
		(CowPresenceViewMember::HerdInWeight, StringLoader(IDS_HEADER_HERDINWEIGHT));
}

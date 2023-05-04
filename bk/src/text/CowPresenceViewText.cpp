#include "stdafx.h"
#include "CowPresenceViewText.h"
#include "../text/CowSexText.h"
#include "../uiconstants/PrecisionContstants.h"
#include "../text/MotiveString.h"

std::wstring CowPresenceViewText::Text(const CowPresenceView *pView,uint32_t memberId)
{
	std::wstring text;
   
	switch(memberId)
	{
	case CowPresenceViewMember::State:
		text = pView->IsInStock() ?  '+' : '-';
		break;

	case CowPresenceViewMember::CowNo:
		text = pView->GetCowNo().ToString();
		break;

	case CowPresenceViewMember::PassNo:
		text = pView->GetPassNo();
		break;
	
	case CowPresenceViewMember::FirstOwner:
		{
			const HentView* pFirstOwner = pView->GetFirstOwner();
			if(pFirstOwner)
				text = pFirstOwner->GetAlias();
		}
		break;

	case CowPresenceViewMember::BirthDate:
		text = pView->GetBirthDate().GetDayDate();
		break;

	case CowPresenceViewMember::Weight:
		text = pView->GetWeight().ToString(WEIGHT_PREC);
		break;

	case CowPresenceViewMember::Sex:
		text = CowSexText(pView->GetSex()).ToString();
		break;

	case CowPresenceViewMember::StockCd:
		text = pView->GetOwnStockCd();			
		break;

	case CowPresenceViewMember::ClassCd:
		text = pView->GetOwnClassCd();		
		break;

	case CowPresenceViewMember::Extras:
		
		text = pView->GetExtras();
		//text.Replace('\n',';');
		//text.Remove('\r');
		
			
		
		break;
	case CowPresenceViewMember::HerdInDate:
		text = pView->GetHerdInDate().GetDayDate();
		break;	
	
	case CowPresenceViewMember::HerdInDocNo:
		text = pView->GetHerdInDocNo();
		break;

	case CowPresenceViewMember::HerdInGrp:
		text = pView->GetHerdInGrp().ToString();
		break;

	case CowPresenceViewMember::HerdInClass:
		text = pView->GetHerdInClassCd();
		break;

	case CowPresenceViewMember::HerdInWeight:
		text = pView->GetHerdInWeight().ToString(WEIGHT_PREC);
		break;

	case CowPresenceViewMember::BuyFromHent:
		{
			const HentView* pBuyFromHent = pView->GetBuyFromHent();
			if(pBuyFromHent)
				text = pBuyFromHent->GetAlias();
		}		
		break;

	case CowPresenceViewMember::BuyStock:
		text = pView->GetBuyStockCd();
		break;

	case CowPresenceViewMember::BuyWeight:
		text = pView->GetBuyWeight().ToString(WEIGHT_PREC);
		break;

	case CowPresenceViewMember::BuyPrice:
		text = pView->GetBuyPrice().ToString(PRICE_PREC);		
		break;

	case CowPresenceViewMember::HerdOutDocNo:		
		text = pView->GetHerdOutDocNo();		
		break;

	case CowPresenceViewMember::HerdOutDate:
		text = pView->GetHerdOutDate().GetDayDate();
		break;

	case CowPresenceViewMember::HerdOutGrp:
		text = pView->GetHerdOutGrp().ToString();
		break;

	case CowPresenceViewMember::HerdOutClass:
		text = pView->GetHerdOutClassCd();
		break;

	case CowPresenceViewMember::HerdOutWeight:
		text = pView->GetHerdOutWeight().ToString(WEIGHT_PREC);
		break;			
	
	case CowPresenceViewMember::SellToHent:
		{
			const HentView* pSellToHent = pView->GetSellToHent();
			if(pSellToHent)
				text = pSellToHent->GetAlias();
		}
		break;

	case CowPresenceViewMember::SellStock:
		text = pView->GetSellStockCd();
		break;		

	case CowPresenceViewMember::SellPrice:
		text = pView->GetSellPrice().ToString(PRICE_PREC);		
		break;

	case CowPresenceViewMember::SellWeight:
		text = pView->GetSellWeight().ToString(WEIGHT_PREC);
		break;

	case CowPresenceViewMember::TermBuyClass:		
		text = pView->GetTermBuyClassCd();
		break;

	case CowPresenceViewMember::TermBuyWeight:
		text = pView->GetTermBuyWeight().ToString(WEIGHT_PREC);		
		break;

	case CowPresenceViewMember::TermBuyPrice:
		text = pView->GetTermBuyPrice().ToString(PRICE_PREC);				
		break;

	case CowPresenceViewMember::TermSellClass:
		text = pView->GetTermSellClassCd();
		break;

	case CowPresenceViewMember::TermSellWeight:
		text = pView->GetTermSellWeight().ToString(WEIGHT_PREC);
		break;

	case CowPresenceViewMember::TermSellPrice:
		text = pView->GetTermSellPrice().ToString(WEIGHT_PREC);								
		break;

	case CowPresenceViewMember::BuyInvoiceNo:
		text = pView->GetBuyInvoiceNo();
		break;

	case CowPresenceViewMember::SellInvoiceNo:
		text = pView->GetSellInvoiceNo();
		break;
	
	case CowPresenceViewMember::HerdInMotive:
		text = MotiveString(pView->GetHerdInMotive()).ToString();
		break;

	case CowPresenceViewMember::HerdOutMotive:
		text = MotiveString(pView->GetHerdOutMotive()).ToString();
		break;	

	case CowPresenceViewMember::BuyDocHent:
	{
		const HentView* buyDocHent = pView->GetBuyDocHent();
		if (buyDocHent != NULL) {
			text = buyDocHent->GetAlias();
		}
		break;
	}
	default:
		ASSERT(0);
		break;
	}
	return text;
}

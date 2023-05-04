#include "stdafx.h"
#include "CowPresenceViewComparator.h"
#include <compare/TypesCompare.h>
#include <assert.h>

using namespace TypesCompare;

COMPARE_RESULT CowPresenceViewComparator::CompareBy(uint32_t dataMember,const CowPresenceView* DataLeft,const CowPresenceView* DataRight) const
{
	const CowPresenceView *left = DataLeft,*right = DataRight;
		
	switch(dataMember)
	{
		default:
			ASSERT(0);		
		case CowPresenceViewMember::State:
			return CompareBool(left->IsInStock(),right->IsInStock());
		case CowPresenceViewMember::CowNo:
			return CompareNullable<CowNo>(left->GetCowNo(),right->GetCowNo());
		case CowPresenceViewMember::PassNo:
			return CompareLocaleString(left->GetPassNo(), right->GetPassNo());
		case CowPresenceViewMember::FirstOwner:
			return m_HentCmp.CompareBy(HentViewMember::Alias,left->GetFirstOwner(),right->GetFirstOwner());
		case CowPresenceViewMember::BirthDate:
			return CompareDateTime(left->GetBirthDate(),right->GetBirthDate());
		case CowPresenceViewMember::Weight:
			return CompareType<Decimal>(left->GetWeight(),right->GetWeight());
		case CowPresenceViewMember::Sex:
			return CompareType<CowSex>(left->GetSex(),right->GetSex());
		case CowPresenceViewMember::StockCd:
			return CompareLocaleString(left->GetOwnStockCd(), right->GetOwnStockCd());
		case CowPresenceViewMember::ClassCd:
			return CompareLocaleString(left->GetOwnClassCd(), right->GetOwnClassCd());
		case CowPresenceViewMember::Extras:
			return CompareLocaleString(left->GetExtras(), right->GetExtras());
		case CowPresenceViewMember::BuyFromHent:
			return m_HentCmp.CompareBy(HentViewMember::Alias,left->GetBuyFromHent(),right->GetBuyFromHent());
		case CowPresenceViewMember::BuyStock:
			return CompareLocaleString(left->GetBuyStockCd(), right->GetBuyStockCd());
		case CowPresenceViewMember::BuyPrice:
			return CompareNullable<NullDecimal>(left->GetBuyPrice(),right->GetBuyPrice());
		case CowPresenceViewMember::BuyWeight:
			return CompareNullable<NullDecimal>(left->GetBuyWeight(),right->GetBuyWeight());
		case CowPresenceViewMember::HerdInDocNo:
			return CompareLocaleString(left->GetHerdInDocNo(), right->GetHerdInDocNo());
		case CowPresenceViewMember::HerdInDate:
			return CompareDateTime(left->GetHerdInDate(),right->GetHerdInDate());
		case CowPresenceViewMember::HerdInGrp:
			return CompareNullInt(left->GetHerdInGrp(),right->GetHerdInGrp());
		case CowPresenceViewMember::HerdInClass:
			return CompareLocaleString(left->GetHerdInClassCd(), right->GetHerdInClassCd());
		case CowPresenceViewMember::HerdInWeight:
			return CompareNullable<NullDecimal>(left->GetHerdInWeight(),right->GetHerdInWeight());
		case CowPresenceViewMember::SellToHent:
			return m_HentCmp.CompareBy(HentViewMember::Alias,left->GetSellToHent(),right->GetSellToHent());
		case CowPresenceViewMember::SellStock:
			return CompareLocaleString(left->GetSellStockCd(), right->GetSellStockCd());
		case CowPresenceViewMember::SellPrice:
			return CompareNullable<NullDecimal>(left->GetSellPrice(),right->GetSellPrice());
		case CowPresenceViewMember::SellWeight:
			return CompareNullable<NullDecimal>(left->GetSellWeight(),right->GetSellWeight());
		case CowPresenceViewMember::HerdOutDocNo:
			return CompareLocaleString(left->GetHerdOutDocNo(), right->GetHerdOutDocNo());
		case CowPresenceViewMember::HerdOutDate:
			return CompareDateTime(left->GetHerdOutDate(),right->GetHerdOutDate());
		case CowPresenceViewMember::HerdOutGrp:
			return CompareNullInt(left->GetHerdOutGrp(),right->GetHerdOutGrp());
		case CowPresenceViewMember::HerdOutClass:
			return CompareLocaleString(left->GetHerdOutClassCd(), right->GetHerdOutClassCd());
		case CowPresenceViewMember::HerdOutWeight:
			return CompareNullable<NullDecimal>(left->GetHerdOutWeight(),right->GetHerdOutWeight());	  
		case CowPresenceViewMember::BuyInvoiceNo:
			return CompareLocaleString(left->GetBuyInvoiceNo(), right->GetBuyInvoiceNo());
		case CowPresenceViewMember::SellInvoiceNo:
			return CompareLocaleString(left->GetSellInvoiceNo(), right->GetSellInvoiceNo());
		case CowPresenceViewMember::TermBuyClass:
			return CompareLocaleString(left->GetTermBuyClassCd(), right->GetTermBuyClassCd());
		case CowPresenceViewMember::TermSellClass:
			return CompareLocaleString(left->GetTermSellClassCd(), right->GetTermSellClassCd());
		case CowPresenceViewMember::TermBuyWeight:
			return CompareNullable<NullDecimal>(left->GetTermBuyWeight(),right->GetTermBuyWeight());
		case CowPresenceViewMember::TermSellWeight:
			return CompareNullable<NullDecimal>(left->GetTermSellWeight(),right->GetTermSellWeight());
		case CowPresenceViewMember::TermBuyPrice:
			return CompareNullable<NullDecimal>(left->GetTermBuyPrice(),right->GetTermBuyPrice());
		case CowPresenceViewMember::TermSellPrice:
			return CompareNullable<NullDecimal>(left->GetTermSellPrice(),right->GetTermSellPrice());
		case CowPresenceViewMember::HerdInMotive:
			return CompareType<Motive>(left->GetHerdInMotive(), right->GetHerdInMotive());
		case CowPresenceViewMember::HerdOutMotive:
			return CompareType<Motive>(left->GetHerdOutMotive(), right->GetHerdOutMotive());
	}

	ASSERT(0);
	return COMPARE_LESS;
}

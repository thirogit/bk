#include "stdafx.h"
#include "HentComparator.h"
#include <compare/TypesCompare.h>

using namespace TypesCompare;

COMPARE_RESULT HentComparator::CompareBy(uint32_t memberId,const HentView* DataLeft,const HentView* DataRight) const
{
	const HentView *left = DataLeft,*right = DataRight;
	switch(memberId)
	{
		default:
			_ASSERT(0);
		case HentViewMember::Name:
			return CompareLocaleString(left->GetName(), right->GetName());
		case HentViewMember::HentNo:
			return CompareType<HentNo>(left->GetHentNo(),right->GetHentNo());
		case HentViewMember::Alias:
			return CompareLocaleString(left->GetAlias(), right->GetAlias());
		case HentViewMember::Street:
			return CompareLocaleString(left->GetStreet(), right->GetStreet());
		case HentViewMember::POBox:
			return CompareLocaleString(left->GetPOBox(), right->GetPOBox());
		case HentViewMember::City:
			return CompareLocaleString(left->GetCity(), right->GetCity());
		case HentViewMember::Zip:
			return CompareLocaleString(left->GetZip(), right->GetZip());
		case HentViewMember::Type:
			return CompareType<HentType>(left->GetHentType(),right->GetHentType());
		case HentViewMember::PlateNo:
			return CompareLocaleString(left->GetPlateNo(), right->GetPlateNo());
		case HentViewMember::SyncIt:
			return CompareBool(left->GetSyncIt() , right->GetSyncIt());
		case HentViewMember::PhoneNo:
			return CompareLocaleString(left->GetPhoneNo(), right->GetPhoneNo());
		case HentViewMember::FiscalNo:
			return CompareLocaleString(left->GetFiscalNo(), right->GetFiscalNo());
		case HentViewMember::WetNo:
			return CompareLocaleString(left->GetWetNo(), right->GetWetNo());
		case HentViewMember::Extras:
			return CompareLocaleString(left->GetExtras(), right->GetExtras());
		case HentViewMember::PersonalNo:
			return CompareLocaleString(left->GetPESEL(), right->GetPESEL());
		case HentViewMember::StatNo:
			return CompareLocaleString(left->GetREGON(), right->GetREGON());
		case HentViewMember::PersonalIdNo:
			return CompareLocaleString(left->GetIdNo(), right->GetIdNo());
		case HentViewMember::PersonalIdIssueDt:
			return CompareDateTime(left->GetIssueDate(),right->GetIssueDate());
		case HentViewMember::PersonalIdIssuePost:
			return CompareLocaleString(left->GetIssuePost(), right->GetIssuePost());
		case HentViewMember::AccountNo:
			return CompareType<AccountNo>(left->GetAccountNo(),right->GetAccountNo());
		case HentViewMember::BankName:
			return CompareLocaleString(left->GetBankName(), right->GetBankName());
		case HentViewMember::WetLicenceNo:
			return CompareLocaleString(left->GetWetLicenceNo(), right->GetWetLicenceNo());
		case HentViewMember::CellPhoneNo:
			return CompareLocaleString(left->GetCellPhoneNo(), right->GetCellPhoneNo());
		case HentViewMember::EmailAddress:
			return CompareLocaleString(left->GetEmailAddress(), right->GetEmailAddress());
		case HentViewMember::Latitude:
			return CompareNullable<Latitude>(left->GetLatitude(),right->GetLatitude());
		case HentViewMember::Longitude:
			return CompareNullable<Longitude>(left->GetLongitude(),right->GetLongitude());
	}
	_ASSERT(0);
	return COMPARE_LESS;
}

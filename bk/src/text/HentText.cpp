#include "stdafx.h"
#include "HentText.h"
#include "../compare/HentComparator.h"
#include "HentTypeString.h"
#include "YesNoText.h"

std::wstring HentText::Text(const HentView *pHent,uint32_t memberId)
{
	std::wstring text;
   	switch(memberId)
	{
		case HentViewMember::Name:
			text =  pHent->GetName();
			break;
		case HentViewMember::HentNo:
			text = pHent->GetHentNo().ToString();
			break;
		case HentViewMember::Alias:
			text = pHent->GetAlias();
			break;
		case HentViewMember::Street:
			text = pHent->GetStreet();
			break;
		case HentViewMember::POBox:
			text = pHent->GetPOBox();
			break;
		case HentViewMember::City:
			text = pHent->GetCity();
			break;
		case HentViewMember::Zip:
			text = pHent->GetZip();
			break;
		case HentViewMember::PlateNo:
			text = pHent->GetPlateNo();
			break;
		case HentViewMember::Type:
			text = HentTypeString(pHent->GetHentType()).ToString();
			break;
		case HentViewMember::SyncIt:
			text = YesNoText(pHent->GetSyncIt()).ToString();
			break;		
		case HentViewMember::PhoneNo:
			text = pHent->GetPhoneNo();
			break;
		case HentViewMember::FiscalNo:
			text = pHent->GetFiscalNo();
			break;
		case HentViewMember::WetNo:
			text = pHent->GetWetNo();
			break;
		case HentViewMember::Extras:
			text = pHent->GetExtras();
			break;		
		case HentViewMember::PersonalNo:
			text = pHent->GetPESEL();
			break;
		case HentViewMember::StatNo:
			text = pHent->GetREGON();
			break;
		case HentViewMember::PersonalIdNo:
			text = pHent->GetIdNo();
			break;
		case HentViewMember::PersonalIdIssueDt:
			text = pHent->GetIssueDate().GetDayDate();
			break;
		case HentViewMember::PersonalIdIssuePost:
			text = pHent->GetIssuePost();
			break;
		case HentViewMember::AccountNo:
			text = pHent->GetAccountNo().ToString();
			break;
		case HentViewMember::BankName:
			text = pHent->GetBankName();
			break;
		case HentViewMember::WetLicenceNo:
			text = pHent->GetWetLicenceNo();
			break;
		case HentViewMember::CellPhoneNo:
			text = pHent->GetCellPhoneNo();
			break;
		case HentViewMember::EmailAddress:
			text = pHent->GetEmailAddress();
			break;
		case HentViewMember::Latitude:
			text = pHent->GetLatitude().ToString();
			break;
		case HentViewMember::Longitude:
			text = pHent->GetLongitude().ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return text;
}

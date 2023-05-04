#include "stdafx.h"
#include "PurchaseHentViewText.h"
#include "../../../text/HentTypeString.h"

std::wstring PurchaseHentViewText::Text(const PurchaseHentView *pHent,uint32_t memberId)
{
	std::wstring text;
   	switch(memberId)
	{
		case PurchaseHentViewMember::Name:
				text =  pHent->GetName();
				break;
		case PurchaseHentViewMember::HentNo:
				text = pHent->GetHentNo().ToString();
				break;
		case PurchaseHentViewMember::Alias:
				text = pHent->GetAlias();
				break;
		case PurchaseHentViewMember::Street:
				text = pHent->GetStreet();
				break;
		case PurchaseHentViewMember::POBox:
				text = pHent->GetPOBox();
				break;
		case PurchaseHentViewMember::City:
				text = pHent->GetCity();
				break;
		case PurchaseHentViewMember::Zip:
				text = pHent->GetZip();
				break;
		case PurchaseHentViewMember::PlateNo:
				text = pHent->GetPlateNo();
				break;
		case PurchaseHentViewMember::Type:
				text = HentTypeString(pHent->GetHentType()).ToString();
				break;		
		case PurchaseHentViewMember::PhoneNo:
				text = pHent->GetPhoneNo();
				break;
		case PurchaseHentViewMember::FiscalNo:
				text = pHent->GetFiscalNo();
				break;
		case PurchaseHentViewMember::WetNo:
				text = pHent->GetWetNo();
				break;		
		case PurchaseHentViewMember::PersonalNo:
				text = pHent->GetPersonalNo();
				break;
		case PurchaseHentViewMember::StatNo:
				text = pHent->GetStatsNo();
				break;
		case PurchaseHentViewMember::PersonalIdNo:
				text = pHent->GetIdNo();
				break;
		case PurchaseHentViewMember::PersonalIdIssueDt:
				text = pHent->GetIssueDate().GetDayDate();
				break;
		case PurchaseHentViewMember::PersonalIdIssuePost:
				text = pHent->GetIssuePost();
				break;
		case PurchaseHentViewMember::AccountNo:
				text = pHent->GetAccountNo().ToString();
				break;
		case PurchaseHentViewMember::BankName:
				text = pHent->GetBankName();
				break;
		case PurchaseHentViewMember::WetLicenceNo:
			text = pHent->GetWetLicenceNo();
			break;
		case PurchaseHentViewMember::CellPhoneNo:
			text = pHent->GetCellPhoneNo();
			break;
		case PurchaseHentViewMember::EmailAddress:
			text = pHent->GetEmailAddress();
			break;
		case PurchaseHentViewMember::Latitude:
			text = pHent->GetLatitude().ToString();
			break;
		case PurchaseHentViewMember::Longitude:
			text = pHent->GetLongitude().ToString();
			break;
		default:
			ASSERT(0);
			break;
	}	
	return text;
}

#include "stdafx.h"
#include "ToString.h"
#include <sstream>
#include "../data/henttypes/HentTypes.h"
#include "../utils/BoolUtil.h"
#include <data/motives/Motives.h>

std::wstring ToString(const DocDetails& details)
{
	std::wstringstream s;
	
	s << L"docDt=" << details.GetDocDate().GetDayDate();
	s << L", loadDt=" << details.GetLoadDate().GetDayDate();
	s << L", agentId=" << details.GetAgentId();
	s << L", plate#=" << details.GetPlateNo();
	s << L", motive=" << Motives::ToString(details.GetMotive());
	s << L", loadStrTm=" << details.GetLoadStartDtTm().GetTime(Time::HHcolonMI);
	s << L", loadEndTm=" << details.GetLoadEndDtTm().GetTime(Time::HHcolonMI);
	s << L", extras=" << details.GetExtras();
	
	return s.str();
}

std::wstring ToString(const CowEntryDetails& details)
{
	std::wstringstream s;

	s << L"weight=" << details.GetWeight().ToString(3);
	s << L", stockId=" << details.GetStockId();
	s << L", classId=" << details.GetClassId();
	s << L", group=" << details.GetGroup().ToString();
	
	return s.str();
}

std::wstring ToString(const CowDetails& details)
{
	const int price_prec = 5;
	const int weight_prec = 3;

	std::wstringstream s;
	
	s << L"weight=" << details.GetWeight().ToString(3);
	s << L", stockId=" << details.GetStockId();
	s << L", classId=" << details.GetClassId();
	s << L", cowNo=" << details.GetCowNo().ToString();
	s << L", motherNo=" << details.GetMotherNo().ToString();
	s << L", birthDt=" << details.GetBirthDate().GetDayDate();
	s << L", passNo=" << details.GetPassNo();
	s << L", healthCertNo=" << details.GetHealthCertNo();
	s << L", passDt=" << details.GetPassDate().GetDayDate();
	s << L", birthPlace=" << details.GetBirthPlace();
	s << L", fstOwnerId=" << details.GetFirstOwnerId();
	s << L", sex=" << details.GetSex().ToString();
	s << L", termBuyPrice=" << details.GetTermBuyPrice().ToString(price_prec);
	s << L", termSellPrice=" << details.GetTermSellPrice().ToString(price_prec);
	s << L",termBuyPrice=" << details.GetTermBuyWeight().ToString(weight_prec);
	s << L", termSellWeight=" << details.GetTermSellWeight().ToString(weight_prec);
	s << L", termBuyClassId=" << details.GetTermBuyClassId();
	s << L", termSellClassId=" << details.GetTermSellClassId();
	s << L", extras=" << details.GetExtras();
	return s.str();
}

std::wstring ToString(const IHent* hent)
{
	if(!hent) return L"";
	
	std::wstringstream s;
	
	s << L"name=" << hent->GetName();
	s << L", alias=" << hent->GetAlias();
	s << L", street=" << hent->GetStreet();
	s << L", pobox=" << hent->GetPOBox();
	s << L", city=" << hent->GetCity();
	s << L", zip=" << hent->GetZip();
	s << L", phone#=" << hent->GetPhoneNo();
	s << L", fiscal#=" << hent->GetFiscalNo();
	s << L", hent#=" << hent->GetHentNo().ToString();
	s << L", wet#=" << hent->GetWetNo();	
	s << L", plate#=" << hent->GetPlateNo();
	s << L", type=" << HentTypes::ToString(hent->GetHentType());
	s << L", syncit=" << BoolUtil::ToString(hent->GetSyncIt());
	s << L", personal#=" << hent->GetPESEL();
	s << L", stats#=" << hent->GetREGON();
	s << L", personalId#=" << hent->GetIdNo();
	s << L", personalIdIssueDt=" << hent->GetIssueDate().GetDayDate();
	s << L", issuepost=" << hent->GetIssuePost();
	s << L", account#=" << hent->GetAccountNo().ToString();
	s << L", bankname=" << hent->GetBankName();
	s << L", welicence#=" << hent->GetWetLicenceNo();
	s << L", cell#=" << hent->GetCellPhoneNo();
	s << L", email=" << hent->GetEmailAddress();
	s << L", latitude=" << hent->GetLatitude().ToString();
	s << L", longitude=" << hent->GetLongitude().ToString();
	s << L", extras=" << hent->GetExtras();

	return s.str();
}

std::wstring ToString(const IDeductionDefinition* definition)
{
	if(!definition) return L"";
	
	std::wstringstream s;
		
	s << L"code=" << definition->GetCode();
	s << L", fraction=" << definition->GetFraction().ToString(2);
	s << L", enabled=" << BoolUtil::ToString(definition->GetIsEnabledByDefault());
	s << L", reason='" << definition->GetReason() << L"'";
	
	return s.str();
}

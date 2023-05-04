#include "stdafx.h"
#include "CompanyNameAndAddrField.h"

std::wstring CompanyNameAndAddrField::GetFieldValue(MoneyTransferContext* pContext,MoneyTransferEntry* pMoneyTransfer)
{
	std::wstring sCompanyNameAndAddr;
	const CompanyView& companyView = pContext->GetCompany();	

	sCompanyNameAndAddr += companyView.GetName();
	sCompanyNameAndAddr += L" (" + companyView.GetFarmNo().ToString() + L") ";
	sCompanyNameAndAddr += companyView.GetStreet() + L' ' + companyView.GetPOBox() + L", ";
	sCompanyNameAndAddr += companyView.GetCity() + L' ' + companyView.GetZip();
	return sCompanyNameAndAddr;
}


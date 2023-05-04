#include "stdafx.h"
#include "MoneyTransferContext.h"


MoneyTransferContext::MoneyTransferContext() 
{
}

const DateTime& MoneyTransferContext::GetTransferDate() const
{
	return m_dtTransfer;
}

void MoneyTransferContext::SetTransferDate(const DateTime& dtTransfer)
{
	m_dtTransfer = dtTransfer;
}


const CompanyView& MoneyTransferContext::GetCompany() const
{
	return m_company;
}

void MoneyTransferContext::SetCompany(const CompanyView& company)
{
	m_company.CopyFrom(company);
}

const AccountNo& MoneyTransferContext::GetSrcAccountNo() const
{
	return m_srcAccountNo;
}

void MoneyTransferContext::SetSrcAccountNo(const AccountNo& srcAccountNo)
{
	m_srcAccountNo = srcAccountNo;
}

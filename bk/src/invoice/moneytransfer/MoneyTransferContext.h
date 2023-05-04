#ifndef __MONEYTRANSFERCONTEXT_H__
#define __MONEYTRANSFERCONTEXT_H__

#include <types/DateTime.h>
#include "../../view/CompanyView.h"

class MoneyTransferContext
{
public:
	MoneyTransferContext();

	const DateTime& GetTransferDate() const;
	void SetTransferDate(const DateTime& dtTransfer);
	
	const CompanyView& GetCompany() const;
	void SetCompany(const CompanyView& company);

	const AccountNo& GetSrcAccountNo() const;
	void SetSrcAccountNo(const AccountNo& srcAccountNo);

private:
	DateTime     m_dtTransfer;
	CompanyView m_company;
	AccountNo m_srcAccountNo;

};


#endif
#ifndef __COMPANYDAO_H__
#define __COMPANYDAO_H__

#include <progress/SimpleProgress.h>
#include "db/Transaction.h"
#include "BaseDAO.h"
#include <arrays/NewPtrFlushArray.h>

#include "../data/impl/Company.h"

class CompanyDAO : public BaseDAO
{
public:
	bool LoadCompanies(NewPtrFlushArray<Company>& result,SimpleProgress *progress = NULL);
	void UpdateCompany(Transaction transaction,const ICompany* pCompany);
	void DeleteCompany(Transaction transaction,const ICompany* pCompany);
	Company* InsertCompany(Transaction transaction,const ICompany* pCompany);

};

#endif
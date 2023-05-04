#include "stdafx.h"
#include "BaseDAO.h"
#include "../db/Transaction.h"
#include "../db/query/SQLQuery.h"

Transaction BaseDAO::OpenTransaction()
{
	return Transaction(new TransactionObject());
}

uint32_t BaseDAO::NextSeqValue(Transaction transaction, const std::wstring& sSequenceName)
{
	SQLQuery stmt;
	stmt << L"SELECT nextval(%0q)";
	stmt.parse();

	return (uint32_t)transaction->QueryScalar(stmt.str(sSequenceName).c_str(), 0);
}


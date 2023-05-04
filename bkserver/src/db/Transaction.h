#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include "DbConnection.h"
#include <boost/shared_ptr.hpp>

class BaseDAO;

class TransactionObject
{
protected:
	TransactionObject();	
public:
	bool Delete(const std::wstring& stmt,bool commit = false);
	bool Insert(const std::wstring& stmt, bool commit = false);
	bool Update(const std::wstring& stmt, bool commit = false);
	Cursor Query(const std::wstring& query);
	int QueryScalar(const std::wstring& query, int null);
	void Commit();
	void Rollback();

	~TransactionObject();
protected:
	DbConnection* AcquireConnection();

	DbConnection* m_pDbConnection;

	friend class BaseDAO;
	friend class SQLSelect;
};


typedef boost::shared_ptr<TransactionObject> Transaction;

#endif
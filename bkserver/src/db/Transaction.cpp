#include "stdafx.h"
#include "Transaction.h"
#include "SQLSingleConnectionPool.h"
#include "SQLException.h"


TransactionObject::TransactionObject() : m_pDbConnection(NULL)
{
}

bool TransactionObject::Delete(const std::wstring& stmt, bool commit/* = false*/)
{
	return AcquireConnection()->Delete(stmt, commit);
}

bool TransactionObject::Insert(const std::wstring& stmt, bool commit/* = false*/)
{
	return AcquireConnection()->Insert(stmt, commit);
}

bool TransactionObject::Update(const std::wstring& stmt, bool commit/* = false*/)
{
	return AcquireConnection()->Update(stmt, commit);
}

void TransactionObject::Commit()
{
	return AcquireConnection()->Commit();
}

void TransactionObject::Rollback()
{
	return AcquireConnection()->Rollback();
}

DbConnection* TransactionObject::AcquireConnection()
{
	if(!m_pDbConnection)
	{
		m_pDbConnection = SQLSingleConnectionPool::GetInstance()->GetConnectionFromPool();
		if(!m_pDbConnection)
			BOOST_THROW_EXCEPTION(SQLException(L"cannot get db connection."));
	}
	return m_pDbConnection;
}

TransactionObject::~TransactionObject()
{
	if(m_pDbConnection)
	{
		m_pDbConnection->Rollback();
		SQLSingleConnectionPool::GetInstance()->ReleaseConnectionToPool(m_pDbConnection);
	}
}

Cursor TransactionObject::Query(const std::wstring& query)
{
	return AcquireConnection()->Query(query);
}

int TransactionObject::QueryScalar(const std::wstring& query, int null)
{
	Cursor cursor = Query(query);
	if (cursor.nextRow())
	{	
		return cursor.getColInt(0, null);
	}

	return null;
}

	


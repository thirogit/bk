#ifndef PGSVRPOOL_H
#define PGSVRPOOL_H

#include "../CommonDatabasePool.h"
#include "../DbConnection.h"
#include <libpq-fe.h>


class PGSvrPool;
class PgDbConnection;

class PgResult : public Result
{
public:
	PgResult(PGresult* result);
	~PgResult();

	virtual uint32_t getColUInt(int nCol, uint32_t uiNullValue);

	virtual int getColInt(int nCol, int iNullValue);

	virtual NullInt getColNullInt(int nCol);

	virtual std::wstring getColString(int nCol);

	virtual DateTime getColDateTime(int nCol);

	virtual  double getColFloat(int nCol, double fNull);

	virtual NullDouble getColNullDouble(int nCol);

	virtual Decimal getColDecimal(int nCol, const Decimal& valueIfNull);

	virtual NullDecimal getColNullDecimal(int nCol);

	virtual int colIndex(const wchar_t* name);

	virtual bool nextRow();

protected:
	void throwIfColumnOutOfRange(int nCol);

private:
	PGresult* m_result;
	int m_row;

	friend class PgDbConnection;
};


class PgDbConnection : public DbConnection
{
	PgDbConnection();
	int Execute(const wstring& stmt, bool commit);
	PgResult* Select(const wstring& query);

public:
	virtual ~PgDbConnection();
	virtual bool Delete(const wstring& stmt, bool commit = false);
	virtual bool Insert(const wstring& stmt, bool commit = false);
	virtual bool Update(const wstring& stmt, bool commit = false);
	virtual Cursor Query(const wstring& query);
	virtual void Commit();
	virtual void Rollback();
	
private:
	 PGconn     *m_pgConnection;

	 friend class PGSvrPool;
};


class PGSvrPool : public CommonDatabasePool<PgDbConnection>{
public:
	PGSvrPool();
protected:
	//void throwSQLError(const unsigned int, const SQLHANDLE&);
	PgDbConnection* MyGetConcreteConnection();
	void MyReleaseConcreteConnection(PgDbConnection*);
	int MyPingConcreteConnection(PgDbConnection*);
	int MyGetPingTimeout();
	
private:
	std::wstring keepalivequery;
	unsigned int keepalivetimeout;
};

#endif

#include "stdafx.h"
#include "PGSvrPool.h"
#include "../SQLException.h"
#include <string/TextUtils.h>

using namespace std;

PgResult::PgResult(PGresult* result) : m_result(result), m_row(-1)
{
}

PgResult::~PgResult()
{
	PQclear(m_result);
}

void PgResult::throwIfColumnOutOfRange(int nCol)
{
	int nfields = PQnfields(m_result);
	if (nCol < 0 || nCol >= nfields)
		BOOST_THROW_EXCEPTION(SQLException(TextUtils::Format(L"column %d out of range %d..%d", nCol, 0, nfields)));
}

uint32_t PgResult::getColUInt(int nCol, uint32_t uiNullValue/* = 0*/)
{
	throwIfColumnOutOfRange(nCol);
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		char *end;
		return std::strtoul(value, &end, 10);		
	}
	return uiNullValue;
}

int PgResult::getColInt(int nCol, int iNullValue/* = 0*/)
{
	return 	getColNullInt(nCol).ToInt(iNullValue);
}

NullInt PgResult::getColNullInt(int nCol)
{
	throwIfColumnOutOfRange(nCol);

	NullInt r;
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		int integer = atoi(value);
		r = integer;
	}	
	return r;	
}

std::wstring PgResult::getColString(int nCol)
{
	throwIfColumnOutOfRange(nCol);

	std::wstring r;
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		r = TextUtils::FromUTF8(value);
	}
	return r;
}

DateTime PgResult::getColDateTime(int nCol)
{
	throwIfColumnOutOfRange(nCol);

	DateTime r;
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		r = DateTime::ParseDate(DateTime::YYYYdashMMdashDD, TextUtils::FromUTF8(value));		
	}
	return r;
}

double PgResult::getColFloat(int nCol, double fNull/* = 0.0*/)
{
	return getColNullDouble(nCol).GetDouble(fNull);
}

NullDouble PgResult::getColNullDouble(int nCol)
{
	throwIfColumnOutOfRange(nCol);
	NullDouble r;
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		char* end;
		r = std::strtod(value, &end);
	}
	return r;
}

Decimal PgResult::getColDecimal(int nCol, const Decimal& valueIfNull)
{
	return getColNullDecimal(nCol).ToDecimal(valueIfNull);
}

NullDecimal PgResult::getColNullDecimal(int nCol)
{
	throwIfColumnOutOfRange(nCol);
	NullDecimal r;
	if (!PQgetisnull(m_result, m_row, nCol))
	{
		char* value = PQgetvalue(m_result, m_row, nCol);
		r = Decimal::FromString(TextUtils::FromUTF8(value));
	}
	return r;
}

int PgResult::colIndex(const wchar_t* name)
{
	return PQfnumber(m_result, TextUtils::ToUTF8(name).c_str());
}

bool PgResult::nextRow()
{
	m_row++;
	return PQntuples(m_result) > m_row;
}

//-------------------------------------------------------------------------------------------------

PgDbConnection::PgDbConnection() : m_pgConnection(NULL)
{
}

PgDbConnection::~PgDbConnection()
{
}

int PgDbConnection::Execute(const wstring& stmt, bool commit)
{
	PGresult* result = PQexec(m_pgConnection, TextUtils::ToUTF8(stmt).c_str());
	
	ExecStatusType status = PQresultStatus(result);
	if (status != PGRES_COMMAND_OK)
	{
		BOOST_THROW_EXCEPTION(SQLException(TextUtils::FromUTF8(PQerrorMessage(m_pgConnection))));
	}
		
	char* szTuplesAffected = PQcmdTuples(result);
	int rowsAffected = atoi(szTuplesAffected);

	PQclear(result);
	wprintf(L"Execute %s\n", stmt.c_str());

	if (commit)
		Commit();

	return rowsAffected;
}

Cursor PgDbConnection::Query(const wstring& query)
{
	return Cursor(Select(query));
}

PgResult* PgDbConnection::Select(const wstring& query)
{
	PGresult* result = PQexecParams(m_pgConnection, TextUtils::ToUTF8(query).c_str(),0,NULL,NULL,NULL,NULL,0);

	ExecStatusType status = PQresultStatus(result);
	if (status != PGRES_TUPLES_OK)
	{
		SQLException e(TextUtils::FromUTF8(PQerrorMessage(m_pgConnection)));
		PQclear(result);
		BOOST_THROW_EXCEPTION(e);
	}

	//wprintf(L"Select %s\n", query.c_str());
	return new PgResult(result);
}

bool PgDbConnection::Delete(const wstring& stmt, bool commit/* = false*/)
{
	return Execute(stmt, commit) > 0;
}

bool PgDbConnection::Insert(const wstring& stmt, bool commit/* = false*/)
{
	return Execute(stmt, commit) > 0;
}

bool PgDbConnection::Update(const wstring& stmt, bool commit/* = false*/)
{
	return Execute(stmt, commit) > 0;
}

void PgDbConnection::Commit()
{
	PGresult* result = PQexec(m_pgConnection, "COMMIT");
	PQclear(result);
	printf("COMMIT\n");
}

void PgDbConnection::Rollback()
{
	PGresult* result = PQexec(m_pgConnection, "ROLLBACK");
	PQclear(result);
	printf("ROLLBACK\n");
}

//-------------------------------------------------------------------------------------------------

PGSvrPool::PGSvrPool()
{
	this->keepalivequery=L"select 0";
	this->keepalivetimeout=300000;	
}

PgDbConnection* PGSvrPool::MyGetConcreteConnection()
{
	/* Make a connection to the database */
	boost::format connStrFmtr("host=%1% port=%2% dbname=bk connect_timeout=10000 user=%3% password=%4%");
	DbConnectionOptions connOptions = GetConnectionOptions();
	//std::string connectionStr = "host=localhost port=5432 dbname=bk connect_timeout=1 user=bkserver password=b0$k13Wym|0No";
	std::string connectionStr = (connStrFmtr % connOptions.Host() % connOptions.Port() % connOptions.User() % connOptions.Password()).str();

	PGconn *pgConnection = PQconnectdb(connectionStr.c_str());

	/* Check to see that the backend connection was successfully made */
	if (PQstatus(pgConnection) != CONNECTION_OK)
	{
		SQLException e(TextUtils::FromUTF8(PQerrorMessage(pgConnection)));		
		PQfinish(pgConnection);
		BOOST_THROW_EXCEPTION(e);
	}

	PGresult* result = PQexec(pgConnection, "BEGIN");
	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		SQLException e(TextUtils::FromUTF8(PQerrorMessage(pgConnection)));
		PQclear(result);
		PQfinish(pgConnection);
		BOOST_THROW_EXCEPTION(e);
	}

	PQclear(result);

	PgDbConnection* connection = new PgDbConnection();
	connection->m_pgConnection = pgConnection;
	return connection;
}

void PGSvrPool::MyReleaseConcreteConnection(PgDbConnection* dbConnection)
{
	if (PQstatus(dbConnection->m_pgConnection) == CONNECTION_OK)
	{
		/* end the transaction */
		PGresult* result = PQexec(dbConnection->m_pgConnection, "END");
		PQclear(result);
	}	
	PQfinish(dbConnection->m_pgConnection);	
	delete dbConnection;
}

int PGSvrPool::MyPingConcreteConnection(PgDbConnection* dbConnection)
{
	PGresult   *result = PQexec(dbConnection->m_pgConnection, TextUtils::ToUTF8(keepalivequery).c_str());

	if (PQresultStatus(result) != PGRES_COMMAND_OK)
	{
		PQclear(result);
		return -1;		
	}

	PQclear(result);
	return 0;
}

int PGSvrPool::MyGetPingTimeout(){
	return keepalivetimeout;
}


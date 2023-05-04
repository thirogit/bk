#ifndef __DBCONNECTION_H__
#define __DBCONNECTION_H__

#include "Cursor.h"
#include <string>

class DbConnection
{
public:
	virtual ~DbConnection() = 0;
	virtual bool Delete(const std::wstring& stmt, bool commit = false) = 0;
	virtual bool Insert(const std::wstring& stmt, bool commit = false) = 0;
	virtual bool Update(const std::wstring& stmt, bool commit = false) = 0;
	virtual Cursor Query(const std::wstring& query) = 0;
	virtual void Commit() = 0;;
	virtual void Rollback() = 0;
};

#endif
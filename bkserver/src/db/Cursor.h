#ifndef __DBCURSOR_H__
#define __DBCURSOR_H__

#include <boost/shared_ptr.hpp>
#include "Result.h"

class Cursor
{
public:
	Cursor(Result* result);
	Cursor(const Cursor& cursor);
	Cursor& operator=(const Cursor& cursor);
	~Cursor();

	uint32_t getColUInt(int nCol, uint32_t uiNullValue = 0);
	int getColInt(int nCol, int iNullValue = 0);
	NullInt getColNullInt(int nCol);
	std::wstring getColString(int nCol);
	DateTime getColDateTime(int nCol);
	double getColFloat(int nCol, double fNull = 0.0);
	NullDouble getColNullDouble(int nCol);
	Decimal getColDecimal(int nCol, const Decimal& valueIfNull);
	NullDecimal getColNullDecimal(int nCol);

	bool nextRow();
	int colIndex(const wchar_t* colName);
	
private:
	boost::shared_ptr<Result> m_result;
};


#endif
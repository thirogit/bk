#ifndef __DBRESULT_H__
#define __DBRESULT_H__

#include <types/NullInt.h>
#include <types/DateTime.h>
#include <types/NullDouble.h>
#include <types/Decimal.h>
#include <types/NullDecimal.h>

class Result
{
public:
	virtual ~Result() = 0;

	virtual uint32_t getColUInt(int nCol, uint32_t uiNullValue) = 0;

	virtual int getColInt(int nCol, int iNullValue) = 0;

	virtual NullInt getColNullInt(int nCol) = 0;

	virtual std::wstring getColString(int nCol) = 0;

	virtual DateTime getColDateTime(int nCol) = 0;

	virtual  double getColFloat(int nCol, double fNull) = 0;

	virtual NullDouble getColNullDouble(int nCol) = 0;

	virtual Decimal getColDecimal(int nCol, const Decimal& valueIfNull) = 0;

	virtual NullDecimal getColNullDecimal(int nCol) = 0;

	virtual int colIndex(const wchar_t* name) = 0;

	virtual bool nextRow() = 0;
};


#endif

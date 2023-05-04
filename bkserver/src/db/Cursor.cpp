#include "stdafx.h"
#include "Cursor.h"

Cursor::Cursor(Result* result)
{
	m_result.reset(result);
}

Cursor::Cursor(const Cursor& cursor) : m_result(cursor.m_result)
{
}

Cursor& Cursor::operator=(const Cursor& cursor)
{
	m_result = cursor.m_result;
	return *this;
}

Cursor::~Cursor()
{
}

uint32_t Cursor::getColUInt(int nCol, uint32_t uiNullValue/* = 0*/)
{
	return m_result->getColUInt(nCol,uiNullValue);
}

int Cursor::getColInt(int nCol, int iNullValue/* = 0*/)
{
	return m_result->getColInt(nCol,iNullValue);
}

NullInt Cursor::getColNullInt(int nCol)
{
	return m_result->getColNullInt(nCol);
}

std::wstring Cursor::getColString(int nCol)
{
	return m_result->getColString(nCol);
}

DateTime Cursor::getColDateTime(int nCol)
{
	return m_result->getColDateTime(nCol);
}

double Cursor::getColFloat(int nCol, double fNull/* = 0.0*/)
{
	return m_result->getColFloat(nCol,fNull);
}

NullDouble Cursor::getColNullDouble(int nCol)
{
	return m_result->getColNullDouble(nCol);
}

Decimal Cursor::getColDecimal(int nCol, const Decimal& valueIfNull)
{
	return m_result->getColDecimal(nCol,valueIfNull);
}

NullDecimal Cursor::getColNullDecimal(int nCol)
{
	return m_result->getColNullDecimal(nCol);
}

bool Cursor::nextRow()
{
	return m_result->nextRow();
}

int Cursor::colIndex(const	wchar_t* colName)
{
	return m_result->colIndex(colName);
}
	
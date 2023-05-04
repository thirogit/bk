#ifndef _SQL_STRING_H_
#define _SQL_STRING_H_

#include "defs.h"
#include "mysql_func.h"
#include <types/DateTime.h>
#include <types/GeoCoordinate.h>
#include <types/Time.h>
#include <data/types/HentNo.h>
#include <data/types/AccountNo.h>
#include <data/types/CowSex.h>
#include <data/types/CowNo.h>
#include <types/Time.h>
#include <types/Decimal.h>
#include <types/NullDecimal.h>

#include <stdio.h>
#include <string>

#define STREAM_FLOAT_PREC 15


/// \brief A specialized \c std::string that will convert from any
/// valid MySQL type.

class SQLString : public std::wstring 
{
public:
	bool zerolen2null;
	/// \brief If true, the object's string data is a copy of another
	/// string.  Otherwise, it's the string form of an integral type.
	bool is_string;

	/// \brief If true, the string data doesn't need to be SQL-escaped
	/// when building a query.
	bool dont_escape;

	/// \brief If true, one of the MySQL++ manipulators has processed
	/// the string data.
	///
	/// "Processing" is escaping special SQL characters, and/or adding
	/// quotes.  See the documentation for manip.h for details.
	///
	/// This flag is used by the template query mechanism, to prevent a
	/// string from being re-escaped or re-quoted each time that query
	/// is reused.
	bool processed;

	/// \brief Default constructor; empty string
	SQLString();

	//copy constructor
	SQLString(const SQLString &copy);

	/// \brief Create object as a copy of a C++ string
	SQLString(const std::wstring& str,bool zerolen = false);

	/// \brief Create object as the string form of a \c char value
	SQLString(wchar_t ch);

	/// \brief Create object as the string form of a \c short \c int
	/// value
	SQLString(short int i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c short \c int value
	SQLString(unsigned short int i);

	/// \brief Create object as the string form of an \c int value
	SQLString(int i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c int value
	SQLString(unsigned int i);

	/// \brief Create object as the string form of a \c longlong
	/// value
	SQLString(longlong i);

	/// \brief Create object as the string form of an \c unsigned
	/// \c longlong value
	SQLString(ulonglong i);

	/// \brief Create object as the string form of a \c float
	/// value
	SQLString(float i);

	/// \brief Create object as the string form of a \c double
	/// value
	SQLString(double i);

	//creates object as string from NullInt object
	SQLString(const NullInt &nullint);

	SQLString(const CowSex &cowsex);

	SQLString(const NullDouble& nulldouble);

	SQLString(const HentNo& hentNo);

	SQLString(const CowNo& cowNo);

	SQLString(const AccountNo& accountNo);

	SQLString(const Decimal& decimal);

	SQLString(const NullDecimal& decimal);

	SQLString(const DateTime &datetime,bool bShortDate = false);

	SQLString(const Time& time);

	/// \brief Copy a C string into this object
	SQLString& operator =(const wchar_t* str);	

	/// \brief Copy a C++ \c string into this object
	SQLString& operator =(const std::wstring& str);	

	//std::wstring quotedstr(void);
	
private:
	void AssignDouble(double d);
	

};

class SQLStringNull : public SQLString
{
public:
	SQLStringNull();
};

class SQLIdable : public SQLString
{
public:
	SQLIdable(uint32_t id);
};

#endif


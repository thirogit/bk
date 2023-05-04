#include "stdafx.h"

//#define MYSQLPP_NOT_HEADER
//#include "platform.h"

#include "SQLQuery.h"

#include "exceptions.h"
#include "mysql_func.h"

using namespace std;



SQLQuery::SQLQuery(const SQLQuery& q) : wstringstream(const_cast<SQLQuery&>(q).str()),def(q.def)
{
}

void SQLQuery::reset()
{
	seekg(0);
	seekp(0);
	clear();
	wstringstream::str(L"");
	parsed.clear();
	def.clear();
}

wchar_t* SQLQuery::preview_char()
{
	*this << std::ends;
	size_t length = rdbuf()->str().size();
	wchar_t *s = new wchar_t[length + 1];
	get(s, length, L'\0');
	seekg(0, std::ios::beg);
	seekp(-1, std::ios::cur);
	return s;
}

static SQLString* pprepare(wchar_t option, SQLString& S, bool replace = true)
{
	SQLString *ss;
	if (S.processed) return &S;

	wchar_t optchr = option;

	if(optchr == L'n') //put NULL if string length is 0
	{
		if(!S.length())
		{
			ss = new SQLStringNull();
			if (replace) 
			{
				S = *ss;
				S.processed = true;
				return &S;
			}
			else return ss;
		}
		else optchr = L'q';
	}  
		
	//TRACE("S = %s\n",S.c_str());
		
	if (optchr == L'r' || (optchr == L'q' && S.is_string)) 
	{
		wchar_t *s = new wchar_t[S.size() * 2 + 1];
		escape_string(s, S.c_str(),	static_cast<unsigned long>(S.size()));
		ss = new SQLString(L"'");
		*ss += s;
		*ss += L"'";
		delete[] s;

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else if (optchr == L'R' || (optchr == L'Q' && S.is_string)) 
	{
		SQLString *ss = new SQLString(L"'" + S + L"'");

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else 
	{
		if (replace) S.processed = true;
		return &S;
	}
}

	/*
	if (option == 'r' || (option == 'q' && S.is_string)) 
	{
		char *s = new char[S.size() * 2 + 1];
		escape_string(s, S.c_str(),
				static_cast<unsigned long>(S.size()));
		SQLString *ss = new SQLString("'");
		*ss += s;
		*ss += "'";
		delete[] s;

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else if (option == 'R' || (option == 'Q' && S.is_string)) 
	{
		SQLString *ss = new SQLString("'" + S + "'");

		if (replace) 
		{
			S = *ss;
			S.processed = true;
			return &S;
		}
		else return ss;
		
	}
	else 
	{
		if (replace) S.processed = true;
		return &S;
	}*/

void SQLQuery::proc(SQLQueryParms& p)
{
	seekg(0, std::ios::beg);
	seekp(0, std::ios::beg);
	int num;
	SQLString *ss;
	SQLQueryParms *c;

	for (std::vector<SQLParseElement>::iterator i = parsed.begin();
			i != parsed.end(); ++i) {
		*this << i->before;
		num = i->num;
		if (num != -1) {
			if (num < static_cast<int>(p.size()))
				c = &p;
			else if (num < static_cast<int>(def.size()))
				c = &def;
			else {
				*this << " ERROR";
				throw
					SQLQueryNEParms
					("Not enough parameters to fill the template.");
			}
			ss = pprepare(i->option, (*c)[num], c->bound());
			*this << *ss;
			if (ss != &(*c)[num]) {
				delete ss;
			}
		}
	}
}

std::wstring SQLQuery::str(SQLQueryParms& p)
{
	if (!parsed.empty()) {
		proc(p);
	}

	*this << std::ends;

	return wstringstream::str();
}

std::wstring SQLQuery::str(SQLQueryParms& p, query_reset r)
{
	std::wstring tmp = str(p);
	if (r == RESET_QUERY) {
		reset();
	}
	return tmp;
}

SQLQueryParms SQLQueryParms::operator +(const SQLQueryParms & other) const
{
	if (other.size() <= size()) {
		return *this;
	}
	SQLQueryParms New = *this;
	size_t i;
	for (i = size(); i < other.size(); i++) {
		New.push_back(other[i]);
	}

	return New;
}

void SQLQuery::parse()
{
	std::wstring str = L"";
	wchar_t num[4];
	int n;
	wchar_t option;
	std::wstring name;
	wchar_t *s, *s0;
	s0 = s = preview_char();
	while (*s) {
		if (*s == L'%') {
			s++;
			if (*s == L'%') {
				str += *s++;
			}
			else if (*s >= L'0' && *s <= L'9') {
				num[0] = *s;
				s++;
				if (*s >= L'0' && *s <= L'9') {
					num[1] = *s;
					num[2] = 0;
					s++;
					if (*s >= L'0' && *s <= L'9') {
						num[2] = *s;
						num[3] = 0;
						s++;
					}
					else {
						num[2] = 0;
					}
				}
				else {
					num[1] = 0;
				}

				n = wcstol(num, 0, 10);
				option = L' ';

				if (*s == L'n' || *s == L'q' || *s == L'Q' || *s == L'r' || *s == L'R') {
					option = *s++;
				}

				if (*s == L':') {
					s++;
					for ( /* */ ; (*s >= L'A' && *s <= L'Z') ||
						 *s == L'_' || (*s >= L'a' && *s <= L'z'); s++) {
						name += *s;
					}

					if (*s == L':') {
						s++;
					}

					if (n >= static_cast<long int>(parsed_names.size())) {
						parsed_names.insert(parsed_names.end(),
								static_cast<vector<wstring>::size_type>(n + 1) - parsed_names.size(),
								wstring());
					}
					parsed_names[n] = name;
					parsed_nums[name] = n;
				}

				parsed.push_back(SQLParseElement(str, option,n));
				str = L"";
				name = L"";
			}
			else {
				str += L'%';
			}
		}
		else {
			str += *s++;
		}
	}

	parsed.push_back(SQLParseElement(str, L' ', -1));
	delete[] s0;
}



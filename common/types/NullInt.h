#ifndef __NULLINT_H__
#define __NULLINT_H__

#include "Nullable.h"
#include <string>
#include <boost/optional.hpp>

class NullInt : public Nullable
{
public:
	NullInt();
	NullInt(int Value);
	NullInt(const NullInt& copy);
	NullInt& operator=(const int right);
	NullInt& operator=(const NullInt& right);

	void CopyFrom(const NullInt& copy);
	bool IsNull() const;
	void MakeNull();
	int ToInt(int whatIfNull = 0) const;
	//void ToString(char *pszDest,size_t size,const char* whatIfNull = NULL) const;
	std::wstring ToString() const;
	static NullInt FromString(const std::wstring& s);

	friend bool operator>=(const NullInt& left,const NullInt& right);
	friend bool operator>(const NullInt& left,const NullInt& right);
	friend bool operator<(const NullInt& left,const NullInt& right);
	friend bool operator<=(const NullInt& left,const NullInt& right);
	friend bool operator!=(const NullInt &left ,const NullInt &right);
	friend bool operator==(const NullInt &left ,const NullInt &right);

	friend bool operator<=(const int left,const NullInt& right);
	friend bool operator>=(const int left, const NullInt& right);
	friend bool operator==(const int left ,const NullInt &right);
	friend bool operator!=(const int left ,const NullInt &right);
	friend bool operator<(const int left,const NullInt& right);
	friend bool operator>(const int left, const NullInt& right);
	

	friend bool operator>=(const NullInt& left,const int right);
	friend bool operator<=(const NullInt& left,const int right);
	friend bool operator==(const NullInt &left ,const int right);
	friend bool operator!=(const NullInt &left ,const int right);
	friend bool operator>(const NullInt& left,const int right);
	friend bool operator<(const NullInt& left,const int right);
	
	friend NullInt& operator +=(NullInt &left ,NullInt &right);
	
	
protected:
	
	boost::optional<int> m_Value;
	
};


#endif

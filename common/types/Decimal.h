#ifndef __DECIMAL_H
#define __DECIMAL_H

#include <mpdecimal.h>
#include <string>
#include "../exception/TagException.h"

struct tag_DecimalError;
typedef TagException<tag_DecimalError> decimal_error;

class DecimalFormat
{
public:
	DecimalFormat();
	DecimalFormat(int precision,wchar_t decimalpoint);
	DecimalFormat(int precision);
	DecimalFormat(const DecimalFormat& src);
	wchar_t GetDecimalPoint() const;
	int GetPrecision() const;	
protected:
	int  m_Precision;
	wchar_t m_DecimalPoint;
};

class Decimal 
{
	class DecimalContext
	{
	public:
		DecimalContext();
		operator mpd_context_t*();
		mpd_context_t* operator->();
	private:
		mpd_context_t m_ctx;
	};

	Decimal(mpd_t* decimal);
public:
	Decimal();	
	Decimal(const Decimal& src);
	
	explicit Decimal(long value);
	explicit Decimal(unsigned long value);	
	explicit Decimal(unsigned long long value);

	std::wstring ToString(int prec)  const;
	std::wstring ToString(const DecimalFormat& format) const;
	static Decimal FromString(const std::wstring& sDecimal);

	~Decimal();
	
	Decimal& operator=(const Decimal& right);
	Decimal& operator=(long right);
	Decimal& operator=(unsigned long right);
	Decimal& operator+=(const Decimal& right);
	Decimal& operator-=(const Decimal& right);
	Decimal  operator- ()   const;
		
	void CopyFrom(const Decimal& src);
	int CompareTo(const Decimal& reference) const;

	long long Integer() const;
	Decimal Fraction() const;
	Decimal Round(int decimalPlaces) const;

	friend bool operator==(const Decimal &left ,const Decimal &right);
	friend bool operator!=(const Decimal &left ,const Decimal &right);
	
	friend bool operator>(const Decimal &left ,const Decimal &right);
	friend bool operator<(const Decimal &left ,const Decimal &right);
	friend bool operator>=(const Decimal &left ,const Decimal &right);
	friend bool operator<=(const Decimal &left ,const Decimal &right);
	

	friend Decimal operator*(const Decimal &left, const Decimal& right);
	friend Decimal operator+(const Decimal &left, const Decimal& right);
	friend Decimal operator/(const Decimal &left, const Decimal& right);
	friend Decimal operator-(const Decimal &left, const Decimal& right);

protected:
	void Create();
	void Destroy();
	void AssignTo(long value);
	void AssignTo(unsigned long value);
	void AssignTo(uint8_t sign,mpd_ssize_t exp,const mpd_uint_t* value, mpd_ssize_t valuesize);
	
protected:
	mpd_t* m_pDecimal;
#if _DEBUG
	std::wstring m_stringValue;	
#endif
	void SetStringValue();
	static DecimalContext m_Context;
	
};

Decimal operator "" _d(unsigned long long number);



#endif

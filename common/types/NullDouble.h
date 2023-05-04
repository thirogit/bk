#ifndef __NULLDOUBLE_H__
#define __NULLDOUBLE_H__

#include "Nullable.h"
#include <boost/optional.hpp>

class FloatFormat
{
public:
	FloatFormat();
	FloatFormat(int precision, wchar_t sperator);
	FloatFormat(int precision);
	FloatFormat(const FloatFormat& src);

	wchar_t GetDecimalPoint() const;
	int GetPrecision() const;
protected:
	int  m_Precision;
	wchar_t m_Separator;
};

#define NULLDOUBLE_DEFAULT_PREC 6

class NullDouble : public Nullable
{
public:
	NullDouble();
	explicit NullDouble(double dValue);
	NullDouble(const NullDouble& copy);
	virtual void CopyFrom(const NullDouble& copy);
	virtual void	SetDouble(double dValue);
	virtual double  GetDouble(const double dIfNull = 0.0) const;
	virtual std::wstring ToString(unsigned int precision = NULLDOUBLE_DEFAULT_PREC) const;
    virtual std::wstring ToString(const FloatFormat& fmt) const;
	static NullDouble FromString(const std::wstring& s);

	void MakeNull();
	bool IsNull() const;

	NullDouble& operator=(const double right); 

	friend bool operator==(const NullDouble &left ,const NullDouble &right);
	friend bool operator==(const NullDouble &left ,const double right);
	friend bool operator!=(const NullDouble &left ,const NullDouble &right);
	friend bool operator!=(const NullDouble &left ,const double right);

	friend bool operator>(const NullDouble &left ,const double right);
	friend bool operator<(const NullDouble &left ,const double right);
	friend bool operator>(const NullDouble &left ,const NullDouble &right);
	friend bool operator<(const NullDouble &left ,const NullDouble &right);
	friend bool operator>=(const NullDouble &left ,const double right);
	friend bool operator<=(const NullDouble &left ,const double right);
	friend bool operator>=(const NullDouble &left ,const NullDouble &right);
	friend bool operator<=(const NullDouble &left ,const NullDouble &right);
	friend bool operator>=(const double left,const NullDouble &right);
	friend bool operator<=(const double left,const NullDouble &right);
	
	friend NullDouble operator*(const NullDouble &left, const double right);

	friend NullDouble& operator+=(NullDouble &left ,NullDouble &right);
	friend NullDouble& operator+=(NullDouble &left ,const double right);

protected:
	static void cfltcvt(double value, wchar_t *buffer, int precision, wchar_t decimal_point);
	static wchar_t * cvt(double arg, int ndigits, int *decpt, int *sign, wchar_t *buf, int eflag);


protected:
	boost::optional<double> m_dValue;
};

#endif

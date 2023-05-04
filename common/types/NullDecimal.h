#ifndef __NULLDECIMAL_H__
#define __NULLDECIMAL_H__

#include "Decimal.h"
#include "Nullable.h"
#include <boost/optional.hpp>

class NullDecimal : public Nullable
{
public:
	NullDecimal();
	NullDecimal(const NullDecimal& copy);
	explicit NullDecimal(const Decimal& src);
	explicit NullDecimal(long value);
	
	//short MoneyCents() const;
	//long MoneyInt() const;

	virtual void MakeNull();
	virtual bool IsNull() const;

	Decimal ToDecimal(const Decimal& valueIfNull) const;

	NullDecimal& operator+= (const NullDecimal& rs);
	NullDecimal& operator-= (const NullDecimal& rs);
	NullDecimal  operator- ()   const;
	NullDecimal  operator+ ()   const;
	NullDecimal  operator+ (const NullDecimal& rs) const;
	NullDecimal  operator- (const NullDecimal& rs) const;
	NullDecimal& operator= (const Decimal& rs);


    std::wstring ToString(int prec) const;
	std::wstring ToString(const DecimalFormat& format) const;
	
	static NullDecimal FromString(const std::wstring& sDecimal);

	friend bool operator==(const NullDecimal &left ,const NullDecimal &right);
	friend bool operator!=(const NullDecimal &left ,const NullDecimal &right);
	
	friend bool operator>(const NullDecimal &left ,const NullDecimal &right);
	friend bool operator<(const NullDecimal &left ,const NullDecimal &right);
	friend bool operator>=(const NullDecimal &left ,const NullDecimal &right);
	friend bool operator<=(const NullDecimal &left ,const NullDecimal &right);
	friend NullDecimal operator*(const NullDecimal &left, const NullDecimal& right);
	friend NullDecimal operator/(const NullDecimal &left, const NullDecimal& right);	

protected:
	boost::optional<Decimal> m_Value;
};

//NullDecimal  operator*  (const Percent& ls, const NullDecimal&  rs);
//NullDecimal  operator*  (const NullDecimal& ls, const Percent& rs);
//NullDecimal  operator*  (const double ls, const NullDecimal&  rs);


#endif

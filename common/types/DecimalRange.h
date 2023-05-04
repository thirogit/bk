#ifndef __DECIMALRANGE_H__
#define __DECIMALRANGE_H__

#include "NullDecimal.h"

class DecimalRange 
{
public:

	DecimalRange();
	DecimalRange(const NullDecimal& low,const NullDecimal& hi);
	DecimalRange(const DecimalRange& src);
	DecimalRange& operator=(const DecimalRange& right);
	void CopyFrom(const DecimalRange& src);
	
	const NullDecimal& GetLow() const;
	const NullDecimal& GetHi() const;
	
	bool IsWithin(const Decimal& d) const;
	
protected:

	NullDecimal m_low;
	NullDecimal m_hi;

};




#endif

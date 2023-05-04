#ifndef __INTRANGE_H__
#define __INTRANGE_H__

#include "NullInt.h"

class IntRange 
{
public:

	IntRange();
	IntRange(const NullInt& low,const NullInt& hi);
	IntRange(const IntRange& src);
	IntRange& operator=(const IntRange& right);
	void CopyFrom(const IntRange& src);
	
	const NullInt& GetLow() const;
	const NullInt& GetHi() const;
	
	bool IsWithin(int i) const;
	
protected:

	NullInt m_low;
	NullInt m_hi;

};




#endif

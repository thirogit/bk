#ifndef __DECIMALEDIT_H__
#define __DECIMALEDIT_H__

#include <types/NullDecimal.h>
#include "../numericedit/NumericEdit.h"

class CDecimalEdit : public CNumericEdit
{
public :
	CDecimalEdit(int precision = 0);
	NullDecimal GetDecimal() const;
	void SetDecimal(const NullDecimal& decimal);
	void SetDecimal(const Decimal& decimal);	
private:
	int m_Precision;
};


#endif
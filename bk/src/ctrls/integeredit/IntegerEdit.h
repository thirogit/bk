#ifndef __INTEGEREDIT_H__
#define __INTEGEREDIT_H__

#include <types/NullInt.h>
#include "../numericedit/NumericEdit.h"

class CIntegerEdit : public CNumericEdit
{
public :
	CIntegerEdit();
	NullInt GetInteger() const;
	void SetInteger(const NullInt& integer);
	void SetInteger(int integer);	
};


#endif
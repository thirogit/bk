#ifndef __HOURMINUTEEDIT_H__
#define __HOURMINUTEEDIT_H__

#include <types/Time.h>
#include "../tipedit/TipEdit.h"
#include "../maskededit/OXMaskedEdit.h"

class CHourMinuteEdit : public CTipEditWrapper<COXMaskedEdit>
{
public:
	CHourMinuteEdit();
	bool IsValidTime();
	void SetTime(const Time& t);
	Time GetTime() const;
protected:	
	virtual BOOL OnValidate();
	

};


#endif
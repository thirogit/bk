#ifndef __IDOC_H__
#define __IDOC_H__

#include "IIdable.h"
#include "../types/DateTime.h"
#include <string>
#include <arrays/Enumerator.h>
#include "ICowEntry.h"
#include "Motive.h"
#include "../types/Time.h"
#include "DocType.h"

class IDoc : public IIdable
{
public:
	virtual const DateTime& 		GetDocDate() const = 0;
	virtual const DateTime& 		GetLoadDate() const = 0;
	virtual const std::wstring& 	GetExtras() const = 0;
	virtual const std::wstring& 	GetPlateNo() const = 0;
	virtual Motive 					GetMotive() const = 0;	
	virtual const Time& 			GetLoadStartDtTm() const = 0;
	virtual const Time& 			GetLoadEndDtTm() const = 0;	
	virtual uint32_t 				GetAgentId() const = 0;
	virtual int 					GetCowCount() const = 0;	
	virtual DocType					GetDocType() const = 0;
	virtual PtrEnumerator<ICowEntry> EnumCows() const = 0;
	virtual ICowEntry*				GetCow(uint32_t cowId) const = 0;
	virtual ICowEntry*				GetCowAt(int index) const = 0;
};

#endif

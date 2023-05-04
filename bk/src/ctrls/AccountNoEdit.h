#ifndef __ACCOUNTNOEDIT_H__
#define __ACCOUNTNOEDIT_H__

#include "tipedit/tipedit.h"
#include <data/types/AccountNo.h>
#include <boost/optional.hpp>
#include "maskededit/OXMaskedEdit.h"

class CAccountNoEdit : public CTipEditWrapper<COXMaskedEdit>
{
public :
	CAccountNoEdit();
	bool IsValidAccountNo();
	AccountNo GetAccountNo();
	void SetAccountNo(const AccountNo& accountNo);
	void SetAccountCountry(const Country& country);	
protected:	
	virtual void PreSubclassWindow();
	void ResetMask();
	virtual BOOL OnValidate();


	Country m_country;
};


#endif
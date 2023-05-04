#ifndef __BUYINVOICELOCKER_H__
#define __BUYINVOICELOCKER_H__

#include "../../locking/RecordLocker.h"

class BuyInvoiceLocker : public RecordLocker
{
public:
	BuyInvoiceLocker(SeasonSession* session);
	virtual void CreateAttempts(const UniqueIds& invoiceIds);
	virtual void Lock(uint32_t invoiceId) override;
	virtual void Unlock(uint32_t invoiceId) override;
private:
	SeasonSession* m_session;	
};

#endif
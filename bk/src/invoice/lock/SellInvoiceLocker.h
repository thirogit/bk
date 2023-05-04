#ifndef __SELLINVOICELOCKER_H__
#define __SELLINVOICELOCKER_H__

#include "../../locking/RecordLocker.h"

class SellInvoiceLocker : public RecordLocker
{
public:
	SellInvoiceLocker(SeasonSession* session);
	virtual void CreateAttempts(const UniqueIds& invoiceIds);
	virtual void Lock(uint32_t invoiceId) override;
	virtual void Unlock(uint32_t invoiceId) override;
private:
	SeasonSession* m_session;
	
};

#endif
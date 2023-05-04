#ifndef __IPURCHASE_H__
#define __IPURCHASE_H__

#include "../IIdable.h"
#include <string>
#include <arrays/Enumerator.h>
#include <types/DateTime.h>
#include "IPurchaseInvoice.h"
#include "IPurchaseHent.h"
#include <CommonTypes.h>

class IPurchase
{
public:	
	virtual const std::wstring& GetId() const = 0;
	virtual int GetHerd() const = 0;
	virtual const std::wstring& GetAgentCode() const = 0;	
	virtual const std::wstring& GetPlateNo() const = 0;
	virtual const DateTime& GetStartDate() const = 0;
	virtual const DateTime& GetEndDate() const = 0;
	virtual bool  GetWasDownloaded() const = 0;	
	virtual count_t GetInvoiceCount() const = 0;	
	virtual IPurchaseInvoice* GetInvoiceAt(int index) const = 0;
	virtual IPurchaseInvoice* GetInvoice(const std::wstring& id) const = 0;
	virtual PtrEnumerator<IPurchaseInvoice> EnumInvoices() const = 0;
	virtual count_t GetHentCount() const = 0;
	virtual PtrEnumerator<IPurchaseHent> EnumHents() const = 0;
	virtual IPurchaseHent* GetHentAt(int index) const = 0;
	virtual IPurchaseHent* GetHent(const HentNo& hentNo) const = 0;

};
	

#endif

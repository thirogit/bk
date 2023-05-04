#ifndef __DISPATCH_H__
#define __DISPATCH_H__

class Dispatch 
{		
public:	
	(uint32_t Id = NULL_ID);
	Purchase(const Purchase& src);
	Purchase(const IPurchase* src);
	~Purchase();
	void CopyFrom(const Purchase& src);
	void CopyFrom(const IPurchase* src);
	Purchase& operator=(const Purchase& right);

	virtual uint32_t GetId() const;
	virtual int GetHerd() const;
	virtual const std::wstring& GetAgentCode() const;	
	virtual const std::wstring& GetPlateNo() const;
	virtual const DateTime& GetStartDate() const;
	virtual const DateTime& GetEndDate() const;
	virtual bool  GetWasDownloaded() const;	
	virtual IPurchaseInvoice* GetInvoiceAt(int index) const;
	virtual IPurchaseInvoice* GetInvoice(uint32_t id) const;

	PurchaseInvoice* GetInvoiceAt(int index);
	PurchaseInvoice* GetInvoice(uint32_t id);
	
	virtual count_t GetInvoiceCount() const;
	virtual PtrEnumerator<IPurchaseInvoice> EnumInvoices() const;	
	
	virtual count_t GetHentCount() const;
	virtual PtrEnumerator<IPurchaseHent> EnumHents() const;
	virtual IPurchaseHent* GetHentAt(int index) const;
	virtual IPurchaseHent* GetHent(const HentNo& hentNo) const;

		
	PurchaseHent* AddHent(IPurchaseHent* pHent);
	PurchaseInvoice* AddInvoice(IPurchaseInvoice* pInvoice);
	PurchaseInvoice* AddInvoice();
	PurchaseHent* AddHent();
			
	void SetHerd(int herd);
	void SetAgentCode(const std::wstring& sAgentCode);
	void SetPlateNo(const std::wstring& sPlateNo);
	void SetStartDate(const DateTime& startDt);
	void SetEndDate(const DateTime& endDt);
	void SetWasDownloaded(bool bDownloaded);
private:
	CowIndex m_Cows;
	InDocIndex m_
	
	
};
	

#endif
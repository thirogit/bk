#ifndef __PURCHASE_H__
#define __PURCHASE_H__

#include "IPurchase.h"
#include "PurchaseHent.h"
#include "PurchaseInvoice.h"
#include "../../CommonTypes.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <SF/SerializeAny.hpp>
#include <utils/IdGenerator.h>

class SerializablePurchase;

class Purchase : public IPurchase
{
	struct index_tag_id{};
	struct index_tag_hentNo{};

	typedef boost::multi_index_container
		<
		PurchaseInvoice*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_id>, boost::multi_index::const_mem_fun<PurchaseInvoice, const std::wstring&, &PurchaseInvoice::GetId> >
		>
		> InvoicesIndex;

	typedef boost::multi_index_container
		<
		PurchaseHent*,
		boost::multi_index::indexed_by<
		boost::multi_index::random_access<>,
		boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_hentNo>, boost::multi_index::const_mem_fun<PurchaseHent, const HentNo&, &PurchaseHent::GetHentNo> >
		>
		> PurchaseHentIndex;
		
public:	
	Purchase();
	Purchase(const std::wstring& id);
	Purchase(const Purchase& src);
	Purchase(Purchase&& src);
	Purchase(const IPurchase* src);
	~Purchase();
	void CopyFrom(const Purchase& src);
	void CopyFrom(const IPurchase* src);
	void CopyDetailsFrom(const IPurchase* src);
	Purchase& operator=(const Purchase& right);

	virtual const std::wstring& GetId() const;
	virtual int GetHerd() const;
	virtual const std::wstring& GetAgentCode() const;	
	virtual const std::wstring& GetPlateNo() const;
	virtual const DateTime& GetStartDate() const;
	virtual const DateTime& GetEndDate() const;
	virtual bool  GetWasDownloaded() const;	
	virtual IPurchaseInvoice* GetInvoiceAt(int index) const;
	virtual IPurchaseInvoice* GetInvoice(const std::wstring& id) const;

	PurchaseInvoice* GetInvoiceAt(int index);
	PurchaseInvoice* GetInvoice(const std::wstring& id);
	
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
	std::wstring ObtainNewHentId() const;
	std::wstring ObtainNewInvoiceId() const;

private:
	int m_Herd;
	std::wstring m_AgentCode;
	std::wstring m_PlateNo;
	DateTime m_StartDate;
	DateTime m_EndDate;
	std::wstring m_PuracheId;
	bool m_Downloaded;

	PurchaseHentIndex m_Hents;
	InvoicesIndex m_Invoices;

	IdGenerator m_idGenerator;

	friend class SerializablePurchase;	
};
	

#endif
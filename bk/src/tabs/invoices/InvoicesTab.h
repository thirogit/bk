#pragma once
#include "../TabPage.h"
#include "../ListTab.h"
#include "../../content/BuyInvoicesListContent.h"
#include "../../content/SellInvoicesListContent.h"
#include "../../print/DocumentArchetype.h"
#include "../../ctrls/xtoolbar/XToolBar.h"
#include "../../invoice/invoiceexport/InvoiceInterface.h"

namespace predicate_ops
{
	bool _and(IInvoice* invoice,std::function<bool(IInvoice*)>& oper1, std::function<bool(IInvoice*)>& oper2);
}


struct invoice_predicate
{
	bool operator()(IInvoice* invoice);

	std::function<bool(IInvoice*)> m_fn;
};


template<class F> invoice_predicate wrap(F& f)
{
	invoice_predicate p;
	p.m_fn = f;
	return p;
}

invoice_predicate and (const invoice_predicate& left, const invoice_predicate& right);


class InvoicesTab : public TabPage
{
	class InvoiceTabInvoiceSearchInterfaceImpl : public InvoiceInterface
	{
	public:
		InvoiceTabInvoiceSearchInterfaceImpl(SeasonSession* session,const InvoiceKind& kind);

		virtual UniqueIds Search(const DateRange& range, PayWay payway);
		virtual InvoiceKind GetInvoiceKind() const;
		virtual IInvoice* GetInvoice(uint32_t id);
	protected:
		IInvoice* GetBuyInvoice(uint32_t id);
		IInvoice* GetSellInvoice(uint32_t id);
	private:
		InvoiceKind m_kind;
		SeasonSession* m_session;
	};

public:
	InvoicesTab();	
protected:
	CMFCTabCtrl m_Navigator;
	XToolBar m_toolbar;
protected:
	ListTab m_BuyRegularInvoicesTab;
	BuyInvoicesListContent m_BuyRegularInvoices;

	ListTab m_BuyLumpInvoicesTab;
	BuyInvoicesListContent m_BuyLumpInvoices;

	ListTab m_SellRegularInvoicesTab;
	SellInvoicesListContent m_SellRegularInvoices;
		
	void OnBuyInvoicesRClick(int col,int row, ListTab* list);
	void OnSellInvoicesRClick(int col,int row, ListTab* list);

	void OnEditBuyInvoice(ListTab* list);
	void EditBuyInvoice(uint32_t invoiceId);

	void OnDeleteBuyInvoice(ListTab* list);
	void DeleteBuyInvoice(uint32_t invoiceId);

	void OnEditSellInvoice(ListTab* list);
	void EditSellInvoice(uint32_t invoiceId);

	void OnDeleteSellInvoice(ListTab* list);
	void DeleteSellInvoice(uint32_t invoiceId);

	uint32_t GetSelectedInvoice(ListTab* list);
	UniqueIds GetSelectedInvoices(ListTab* list);
	
	void OnMarkPaidBuyInvoice(ListTab* list);
	void OnMoneyTransfer(ListTab* list);
	void OnExportInvoices(ListTab* list);

	void OnMarkPaidSellInvoice(ListTab* list);
	
	void OnPrintSellInvoice(const DocumentArchetype& archetype, ListTab* list);
	void OnPrintBuyInvoice(const DocumentArchetype& archetype, ListTab* list);

	bool ConfirmInvoiceDelete();
	void OnExportToExcel(ListTab* list);

public:
	virtual ~InvoicesTab();
	
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize( UINT nType,  int cx,  int cy  );
	afx_msg void OnDestroy();
	
	DECLARE_MESSAGE_MAP()
};


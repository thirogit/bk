#ifndef __INSERTPURCHASEOPERATION_H__
#define __INSERTPURCHASEOPERATION_H__

#include <data/repo/IPurchase.h>
#include <data/impl/CowDetails.h>
#include <data/impl/DocDetails.h>
#include <data/impl/Hent.h>
#include <data/impl/InvoiceDetails.h>
#include <data/impl/InvoiceHent.h>
#include <data/impl/BuyInvoice.h>
#include "../../db/Transaction.h"
#include "../../index/IdableIndexes.h"
#include "../../index/HentsIndex.h"
#include "LoggerAwareSeasonOperation.h"

class SeasonSession;

class InsertPurchaseOperation : public LoggerAwareSeasonOperation
{
public:
	InsertPurchaseOperation(IPurchase* purchase,SeasonSession* session, LogInterface* logger);
	virtual void execute();
	void SetDocDate(const DateTime& docDate);
private:
	void Cleanup();
	CowDetails ExtractCowDetails(const IPurchaseCow* cow);
	DocDetails ExtractDocDetails(const IPurchase* purchase);
	Hent* InsertPurchaseHent(Transaction transaction, IPurchaseHent* purchaseHent);
	InvoiceDetails ExtractInvoiceDetails(IPurchaseInvoice* purchaseInvoice);
	Hent* CreateHent(IPurchaseHent* purchaseHent);
	InvoiceHent* CreateInvoiceHent(IPurchaseHent* purchaseHent);
	InvoiceDeduction* CreateDeduction(IPurchaseInvoiceDeduction* deduction);
private:
	std::vector<BuyInvoice*> m_invoices;
	CowsIndex m_cows;
	std::vector<Hent*> m_newHents;
	HentsIndex m_hents;
	InDoc* m_InDoc;
	IPurchase* m_purchase;
	DateTime m_docDate;
};

#endif

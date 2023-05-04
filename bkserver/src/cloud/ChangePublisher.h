#pragma once

#include "GoogleServiceAccount.h"
#include "FirebaseTopicPublisher.h"
#include <data/IInDoc.h>
#include <data/IOutDoc.h>
#include <data/IBuyDoc.h>
#include <data/ISellDoc.h>
#include <data/IMoveDoc.h>
#include <data/IBuyInvoice.h>
#include <data/ISellInvoice.h>
#include <data/ICow.h>
#include <casajson/json.h>
#include "../object/CowServerObject.h"

class SeasonSession;
class FarmSession;
class ChangePublisher;


class ChangePublisherFactory
{
public:
	ChangePublisherFactory(const gcp::GoogleServiceAccount& account, FarmSession* farmSession);
	ChangePublisher* CreatePublisher(SeasonSession* session);
private:
	gcp::GoogleServiceAccount m_account;
	FarmSession* m_farmSession;	
};	

class ChangePublisher
{	
public:
	ChangePublisher(const gcp::GoogleServiceAccount& account, SeasonSession* session, FarmSession* farmSession);

	void PublishCowUpdate(const ICow* cow);

	void PublishInDocInsert(const IInDoc* indoc);
	void PublishBuyDocInsert(const IBuyDoc* buydoc);
	void PublishInDocUpdate(const IInDoc* indoc);
	void PublishBuyDocUpdate(const IBuyDoc* buydoc);
	void PublishInDocDelete(uint32_t indocid);
	void PublishBuyDocDelete(uint32_t buydocid);
	
	void PublishBuyInvoiceInsert(const IBuyInvoice* buyinvoice);
	void PublishBuyInvoiceUpdate(const IBuyInvoice* buyinvoice);
	void PublishBuyInvoiceDelete(uint32_t buyinvoiceid);
	
	void PublishMoveDocInsert(const IMoveDoc* movedoc);
	void PublishMoveDocUpdate(const IMoveDoc* movedoc);
	void PublishMoveDocDelete(uint32_t movedocid);

	void PublishSellDocInsert(const ISellDoc* selldoc);
	void PublishSellDocUpdate(const ISellDoc* selldoc);
	void PublishSellDocDelete(uint32_t selldocid);

	void PublishOutDocInsert(const IOutDoc* outdoc);
	void PublishOutDocUpdate(const IOutDoc* outdoc);
	void PublishOutDocDelete(uint32_t outdocid);
	   
	void PublishSellInvoiceInsert(const ISellInvoice* sellinvoice);
	void PublishSellInvoiceUpdate(const ISellInvoice* sellinvoice);
	void PublishSellInvoiceDelete(uint32_t sellinvoiceid);


private:
	void PublishMessage(const std::wstring& changeType, const std::wstring& entityType, json::value& body);
	void DocDetailsToJson(const IDoc* doc, json::value& result);
	void CowDetailsToJson(const ICow* cow, json::value& result);
	void InvoiceDetailsToJson(const IInvoice* invoice, json::value& result);

	json::value DocContentToJson(const IDoc* doc);
	json::value HerdToJson(uint32_t herdId);
	json::value HentToJson(uint32_t hentId);
	json::value AgentToJson(uint32_t agentId);
	json::value StockToJson(uint32_t stockId);
	json::value ClassToJson(uint32_t classId);
	json::value InDocToJson(const IInDoc* indoc);
	json::value OutDocToJson(const IOutDoc* outdoc);
	json::value SellDocToJson(const ISellDoc* selldoc);
	json::value MoveDocToJson(const IMoveDoc* movedoc);
	json::value BuyDocToJson(const IBuyDoc* buydoc);
	json::value CowEntryToJson(const ICowEntry* entry);
	json::value CowInvoiceEntryToJson(const ICowInvoiceEntry* entry);
	json::value CowToJson(const ICow* cow);
	json::value CowSOsToJson(const std::vector<CowServerObject*>& cows);
	json::value BuyInvoiceToJson(const IBuyInvoice* buyinvoice);
	json::value SellInvoiceToJson(const ISellInvoice* sellinvoice);
	json::value InvoiceContentToJson(const IInvoice* invoice);
	json::value InvoiceDeductionsToJson(const IInvoice* invoice);
	json::value InvoiceHentToJson(const IInvoiceHent* invoiceHent);
	json::value InvoiceDeductionToJson(const IInvoiceDeduction* deduction);
	
private:
	FirebaseTopicPublisher m_fcmPublisher;
	SeasonSession* m_session;
	FarmSession* m_farmSession;
};


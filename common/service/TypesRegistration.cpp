#include "stdafx.h"
#include "TypesRegistration.h"

#include "notification/NotificationEnvelope.h"
#include "notification/LockNotifications.h"
#include "notification/UnlockNotifications.h"
#include "notification/HentNotifications.h"
#include "notification/CowNotifications.h"
#include "notification/DeleteNotifications.h"
#include "notification/DocNotifications.h"
#include "../data/impl/serialization/DataSerialization.h"
#include "../data/runtime/serialization/RuntimeTypesSerialization.h"
#include "../data/runtime/DocTransactionItemTuple.h"
#include "../data/runtime/InvoiceTransactionItemTuple.h"
#include "../data/runtime/DeductionItemTuple.h"
#include "../data/types/serialization/TypesSerialization.h"
#include "notification/InvoiceNotifications.h"
#include "../data/repo/Purchase.h"
#include "../data/repo/serialization/RepoTypesSerialization.h"
#include <SF/tuple.hpp> 
#include <SF/map.hpp> 
#include <SF/vector.hpp> 
#include "../CommonTypes.h"
#include "../data/runtime/SolutionParameters.h"

void RegisterTypes()
{
	SF::registerType<NotificationEnvelope>("NotificationEnvelope");
	SF::registerBaseAndDerived<Notification,NotificationEnvelope >();
	
	//hent notifications
	
	SF::registerType<HentUpdateNotification>("HentUpdateNotification");
	SF::registerBaseAndDerived<Notification,HentUpdateNotification >();

	SF::registerType<HentInsertNotification>("HentInsertNotification");
	SF::registerBaseAndDerived<Notification,HentInsertNotification >();

	SF::registerType<HentDeleteNotification>("HentDeleteNotification");
	SF::registerBaseAndDerived<Notification,HentDeleteNotification >();

	SF::registerType<HentLockNotification>("HentLockNotification");
	SF::registerBaseAndDerived<Notification,HentLockNotification >();

	SF::registerType<HentUnlockNotification>("HentUnlockNotification");
	SF::registerBaseAndDerived<Notification,HentUnlockNotification >();

	//cow notifications
	SF::registerType<CowLockNotification>("CowLockNotification");
	SF::registerBaseAndDerived<Notification,CowLockNotification >();

	SF::registerType<CowUnlockNotification>("CowUnlockNotification");
	SF::registerBaseAndDerived<Notification,CowUnlockNotification >();

	SF::registerType<CowUpdateNotification>("CowUpdateNotification");	
	SF::registerBaseAndDerived<Notification,CowUpdateNotification >();

	SF::registerType<CowInsertNotification>("CowInsertNotification");
	SF::registerBaseAndDerived<Notification,CowInsertNotification >();

	SF::registerType<CowDeleteNotification>("CowDeleteNotification");
	SF::registerBaseAndDerived<Notification,CowDeleteNotification >();
		

	//indoc notification
	SF::registerType<InDocInsertNotification>("InDocInsertNotification");
	SF::registerBaseAndDerived<Notification,InDocInsertNotification >();

	SF::registerType<InDocDeleteNotification>("InDocDeleteNotification");
	SF::registerBaseAndDerived<Notification,InDocDeleteNotification >();

	SF::registerType<InDocUpdateNotification>("InDocUpdateNotification");
	SF::registerBaseAndDerived<Notification,InDocUpdateNotification >();
	
	SF::registerType<InDocLockNotification>("InDocLockNotification");
	SF::registerBaseAndDerived<Notification,InDocLockNotification >();
		
	SF::registerType<InDocUnlockNotification>("InDocUnlockNotification");
	SF::registerBaseAndDerived<Notification,InDocUnlockNotification >();	

	//buydoc notifcations

	SF::registerType<BuyDocInsertNotification>("BuyDocInsertNotification");
	SF::registerBaseAndDerived<Notification,BuyDocInsertNotification >();

	SF::registerType<BuyDocUpdateNotification>("BuyDocUpdateNotification");
	SF::registerBaseAndDerived<Notification,BuyDocUpdateNotification >();

	SF::registerType<BuyDocDeleteNotification>("BuyDocDeleteNotification");
	SF::registerBaseAndDerived<Notification,BuyDocDeleteNotification >();

	SF::registerType<BuyDocLockNotification>("BuyDocLockNotification");
	SF::registerBaseAndDerived<Notification,BuyDocLockNotification >();

	SF::registerType<BuyDocUnlockNotification>("BuyDocUnlockNotification");
	SF::registerBaseAndDerived<Notification,BuyDocUnlockNotification >();

	//movedoc notificaitons

	SF::registerType<MoveDocInsertNotification>("MoveDocInsertNotification");
	SF::registerBaseAndDerived<Notification,MoveDocInsertNotification >();

	SF::registerType<MoveDocDeleteNotification>("MoveDocDeleteNotification");
	SF::registerBaseAndDerived<Notification,MoveDocDeleteNotification >();

	SF::registerType<MoveDocUpdateNotification>("MoveDocUpdateNotification");
	SF::registerBaseAndDerived<Notification,MoveDocUpdateNotification >();

	SF::registerType<MoveDocLockNotification>("MoveDocLockNotification");
	SF::registerBaseAndDerived<Notification,MoveDocLockNotification >();

	SF::registerType<MoveDocUnlockNotification>("MoveDocUnlockNotification");
	SF::registerBaseAndDerived<Notification,MoveDocUnlockNotification >();

	//selldoc notifications

	SF::registerType<SellDocInsertNotification>("SellDocInsertNotification");
	SF::registerBaseAndDerived<Notification,SellDocInsertNotification >();

	SF::registerType<SellDocDeleteNotification>("SellDocDeleteNotification");
	SF::registerBaseAndDerived<Notification,SellDocDeleteNotification >();

	SF::registerType<SellDocUpdateNotification>("SellDocUpdateNotification");
	SF::registerBaseAndDerived<Notification,SellDocUpdateNotification >();

	SF::registerType<SellDocLockNotification>("SellDocLockNotification");
	SF::registerBaseAndDerived<Notification,SellDocLockNotification >();

	SF::registerType<SellDocUnlockNotification>("SellDocUnlockNotification");
	SF::registerBaseAndDerived<Notification,SellDocUnlockNotification >();


	//outdocs notificaitons
	SF::registerType<OutDocInsertNotification>("OutDocInsertNotification");
	SF::registerBaseAndDerived<Notification,OutDocInsertNotification >();
	
	SF::registerType<OutDocUpdateNotification>("OutDocUpdateNotification");
	SF::registerBaseAndDerived<Notification,OutDocUpdateNotification >();

	SF::registerType<OutDocDeleteNotification>("OutDocDeleteNotification");
	SF::registerBaseAndDerived<Notification,OutDocDeleteNotification >();
		
	SF::registerType<OutDocLockNotification>("OutDocLockNotification");
	SF::registerBaseAndDerived<Notification,OutDocLockNotification >();

	SF::registerType<OutDocUnlockNotification>("OutDocUnlockNotification");
	SF::registerBaseAndDerived<Notification,OutDocUnlockNotification >();

		
	//buy invoice notifications
	SF::registerType<BuyInvoiceLockNotification>("BuyInvoiceLockNotification");
	SF::registerBaseAndDerived<Notification,BuyInvoiceLockNotification >();	

	SF::registerType<BuyInvoiceUnlockNotification>("BuyInvoiceUnlockNotification");
	SF::registerBaseAndDerived<Notification,BuyInvoiceUnlockNotification >();


	SF::registerType<BuyInvoiceInsertNotification>("BuyInvoiceInsertNotification");
	SF::registerBaseAndDerived<Notification,BuyInvoiceInsertNotification >();	

	SF::registerType<BuyInvoiceUpdateNotification>("BuyInvoiceUpdateNotification");
	SF::registerBaseAndDerived<Notification,BuyInvoiceUpdateNotification >();

	SF::registerType<BuyInvoiceDeleteNotification>("BuyInvoiceDeleteNotification");
	SF::registerBaseAndDerived<Notification,BuyInvoiceDeleteNotification >();	


	//sell invoice notifications
	SF::registerType<SellInvoiceLockNotification>("SellInvoiceLockNotification");
	SF::registerBaseAndDerived<Notification,SellInvoiceLockNotification >();	

	SF::registerType<SellInvoiceUnlockNotification>("SellInvoiceUnlockNotification");
	SF::registerBaseAndDerived<Notification,SellInvoiceUnlockNotification >();
	
	SF::registerType<SellInvoiceInsertNotification>("SellInvoiceInsertNotification");
	SF::registerBaseAndDerived<Notification,SellInvoiceInsertNotification >();	

	SF::registerType<SellInvoiceUpdateNotification>("SellInvoiceUpdateNotification");
	SF::registerBaseAndDerived<Notification,SellInvoiceUpdateNotification >();	

	SF::registerType<SellInvoiceDeleteNotification>("SellInvoiceDeleteNotification");
	SF::registerBaseAndDerived<Notification,SellInvoiceDeleteNotification >();	

	SF::registerType( (std::wstring *) NULL, "wstring");
	SF::registerType( (std::string *) NULL, "string");
	SF::registerType( (Decimal *) NULL, "decimal");
	SF::registerType( (DocDetails *) NULL, "DocDetails");
    SF::registerType( (CowDetails*) NULL, "CowDetails");
	SF::registerType( (uint32_t *) NULL, "uint32_t");
	SF::registerType( (ItemId *) NULL, "ItemId");
	SF::registerType( (count_t *) NULL, "count");
	SF::registerType( (CowEntryDetails *) NULL, "CowEntryDetails");
	SF::registerType( (int *) NULL, "int");
	SF::registerType( (DocTransactionItemTuple *) NULL, "DocTransactionItemTuple");
	SF::registerType( (InvoiceDetails *) NULL, "InvoiceDetails");
	SF::registerType( (CowInvoiceEntryDetails *) NULL, "CowInvoiceEntryDetails");
	SF::registerType( (InvoiceTransactionItemTuple *) NULL, "InvoiceTransactionItemTuple");
	SF::registerType( (InvoiceHent *) NULL, "InvoiceHent");
	SF::registerType( (bool *)NULL, "bool");

	SF::registerType( (std::vector<int> *) NULL, "vector<int>");
	SF::registerType( (std::vector<std::string> *) NULL, "vector<string>");
	SF::registerType( (std::vector<std::wstring> *) NULL, "vector<wstring>");
	SF::registerType( (std::vector<Decimal> *) NULL, "vector<decimal>");

	SF::registerType( (std::map<int,int> *) NULL, "map<int,int>");
	SF::registerType( (std::map<int,std::string> *) NULL, "map<int,string>");
	SF::registerType((std::map<int, std::wstring> *) NULL, "map<int,wstring>");
	SF::registerType( (std::map<int,Decimal> *) NULL, "map<int,decimal>");

	SF::registerType( (std::map<std::string,int> *) NULL, "map<string,int>");
	SF::registerType( (std::map<std::string,std::string> *) NULL, "map<string,string>");
	SF::registerType( (std::map<std::string,Decimal> *) NULL, "map<string,decimal>");
	SF::registerType( (Purchase*) NULL, "Purchase");
	SF::registerType( (PurchaseDownloadIssues*)NULL, "PurchaseDownloadIssues");
	SF::registerType( (SolutionParameters*)NULL, "SolutionParameters");
	SF::registerType( (SolutionType*)NULL, "SolutionType");
	SF::registerType( (std::vector<ItemId> *) NULL, "vector<ItemId>");
	SF::registerType((PurchaseDownloadIssueCategory*)NULL, "PurchaseDownloadIssueCategory");
	SF::registerType((DeductionItemTuple*)NULL, "DeductionItemTuple");
	SF::registerType((InvoiceDeduction*)NULL, "InvoiceDeduction");
	SF::registerType((CowNo*)NULL, "CowNo");
	SF::registerType((InvoiceType*)NULL, "InvoiceType");
	SF::registerType((DateTime*)NULL, "DateTime");	
		
	SF::registerAny( (std::wstring *)NULL);
    SF::registerAny( (DocDetails *) NULL);
    SF::registerAny( (CowDetails *) NULL);
	SF::registerAny( (uint32_t *) NULL);
	SF::registerAny( (ItemId *) NULL);
	SF::registerAny( (CowEntryDetails*) NULL);
	SF::registerAny( (count_t*) NULL);
	SF::registerAny( (int*) NULL);
	SF::registerAny( (bool*)NULL);
	SF::registerAny( (InvoiceType*)NULL);
	SF::registerAny( (Decimal*)NULL);
	SF::registerAny( (DocTransactionItemTuple*) NULL);
	SF::registerAny( (InvoiceDetails*) NULL);
	SF::registerAny( (InvoiceTransactionItemTuple*) NULL);
	SF::registerAny( (CowInvoiceEntryDetails*) NULL);
	SF::registerAny( (InvoiceHent*) NULL);
	SF::registerAny( (InvoiceDeduction*)NULL);
	SF::registerAny( (DateTime*)NULL);

	SF::registerAny( (std::wstring *) NULL);
	SF::registerAny( (std::vector<int> *) NULL);
	SF::registerAny( (std::vector<std::string> *) NULL);
	SF::registerAny( (std::vector<std::wstring> *) NULL);
	SF::registerAny( (std::vector<Decimal> *) NULL);

	SF::registerAny( (std::map<int,int> *) NULL);
	SF::registerAny( (std::map<int,std::string> *) NULL);
	SF::registerAny( (std::map<int, std::wstring> *) NULL);
	SF::registerAny( (std::map<int,Decimal> *) NULL);

	SF::registerAny( (std::map<std::string,int> *) NULL);
	SF::registerAny( (std::map<std::string,std::string> *) NULL);
	SF::registerAny( (std::map<std::string,Decimal> *) NULL);
	SF::registerAny( (Purchase *) NULL);
	SF::registerAny( (PurchaseDownloadIssues *)NULL);
	SF::registerAny( (SolutionParameters *)NULL);
	SF::registerAny( (SolutionType *)NULL);
	SF::registerAny( (std::vector<ItemId> *) NULL);
	SF::registerAny((PurchaseDownloadIssueCategory*)NULL);
	SF::registerAny((DeductionItemTuple*)NULL);
	SF::registerAny((CowNo*)NULL);
}


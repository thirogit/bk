#pragma once

#include <RCF/RCF.hpp>
#include "messages/LoginRequest.h"
#include "messages/LoginResponse.h"
#include "messages/OpenSeasonResponse.h"
#include "messages/FetchHentsResponse.h"
#include "messages/FetchCowsResponse.h"
#include "messages/FetchClassesResponse.h"
#include "messages/FetchStocksResponse.h"
#include "messages/FetchDocsResponses.h"
#include "messages/FetchInvoicesResponse.h"
#include "messages/HentRequest.h"
#include "messages/CowRequest.h"
#include "messages/TransactionRequest.h"
#include "messages/TransactionResponse.h"
#include "messages/FetchManifest.h"
#include "messages/Settings.h"
#include "messages/MarkInvoicePaidRequest.h"
#include "messages/FetchPrintProfilesResponse.h"
#include "messages/FetchPassportRecognitionProfilesResponse.h"
#include "messages/PrintProfileRequest.h"
#include "messages/PassportRecognitionProfileRequest.h"
#include "messages/FetchDeductionDefinitionsResponse.h"
#include "messages/DeductionDefinitionRequest.h"
#include "messages/PurchaseQuery.h"
#include "messages/FetchPurchasesResponse.h"
#include "messages/FetchPurchaseResponse.h"
#include "messages/FetchPurchaseHeadersResponse.h"
#include <SF/any.hpp>

RCF_BEGIN(IBKService, "IBKService")
    RCF_METHOD_R1(LoginResponseObj, Login, LoginRequestObj)

	RCF_METHOD_R1(OpenSeasonResponseObj, OpenSeason, uint32_t)
	RCF_METHOD_V1(void, CloseSeason, uint32_t)

	RCF_METHOD_R1(FetchManifestObj,BeginFetch,uint32_t)

	RCF_METHOD_R1(FetchHentsResponseObj,FetchHents,uint32_t)
	RCF_METHOD_R1(FetchCowsResponseObj,FetchCows,uint32_t)	
	RCF_METHOD_R1(FetchInDocsResponseObj,FetchInDocs,uint32_t)
	RCF_METHOD_R1(FetchBuyDocsResponseObj,FetchBuyDocs,uint32_t)
	RCF_METHOD_R1(FetchMoveDocsResponseObj,FetchMoveDocs,uint32_t)
	RCF_METHOD_R1(FetchSellDocsResponseObj,FetchSellDocs,uint32_t)
	RCF_METHOD_R1(FetchOutDocsResponseObj,FetchOutDocs,uint32_t)
	RCF_METHOD_R1(FetchInvoicesResponseObj,FetchBuyInvoices,uint32_t)
	RCF_METHOD_R1(FetchInvoicesResponseObj,FetchSellInvoices,uint32_t)
		
	RCF_METHOD_V1(void,EndFetch,uint32_t)

	RCF_METHOD_R1(SettingsObj,FetchSettings,uint32_t)
	RCF_METHOD_V2(void,SaveSettings,uint32_t,SettingsObj)
	
	RCF_METHOD_R2(uint32_t,InsertHent,uint32_t,HentRequestObj)
	RCF_METHOD_V2(void,UpdateHent,uint32_t,HentRequestObj)
	RCF_METHOD_V2(void,DeleteHent,uint32_t,uint32_t)

	RCF_METHOD_V2(void,LockHent,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockHent,uint32_t,uint32_t)

	RCF_METHOD_V2(void,LockCow,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockCow,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UpdateCow,uint32_t,CowRequestObj)

	RCF_METHOD_R2(uint32_t, OpenNewInDocTransaction, uint32_t,uint32_t) //sessionToken, herdId
	RCF_METHOD_R2(uint32_t, OpenEditInDocTransaction, uint32_t,uint32_t) //sessionToken, docId
	RCF_METHOD_V2(void, LockInDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, UnlockInDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteInDoc, uint32_t,uint32_t) //sessionToken, docId
		
	RCF_METHOD_R2(uint32_t, OpenNewBuyDocTransaction, uint32_t,uint32_t) //sessionToken, herdId
	RCF_METHOD_R2(uint32_t, OpenEditBuyDocTransaction, uint32_t,uint32_t) //sessionToken, docId
	RCF_METHOD_V2(void,LockBuyDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockBuyDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteBuyDoc, uint32_t,uint32_t) //sessionToken, docId
	
	RCF_METHOD_R3(uint32_t, OpenNewMoveDocTransaction, uint32_t,uint32_t,uint32_t) //sessionToken, srcHerdId, dstHerdId
	RCF_METHOD_R2(uint32_t, OpenEditMoveDocTransaction, uint32_t,uint32_t) //sessionToken, docId
	RCF_METHOD_V2(void,LockMoveDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockMoveDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteMoveDoc, uint32_t,uint32_t) //sessionToken, docId
	
	RCF_METHOD_R2(uint32_t, OpenNewSellDocTransaction, uint32_t,uint32_t) //sessionToken, herdId
	RCF_METHOD_R2(uint32_t, OpenEditSellDocTransaction, uint32_t,uint32_t) //sessionToken, docId
	RCF_METHOD_V2(void,LockSellDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockSellDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteSellDoc, uint32_t,uint32_t) //sessionToken, docId
		
	RCF_METHOD_R2(uint32_t, OpenNewOutDocTransaction, uint32_t,uint32_t) //sessionToken, herdId
	RCF_METHOD_R2(uint32_t, OpenEditOutDocTransaction, uint32_t,uint32_t) //sessionToken, docId
	RCF_METHOD_V2(void,LockOutDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void,UnlockOutDoc,uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteOutDoc, uint32_t,uint32_t) //sessionToken, docId	
	
	RCF_METHOD_R2(uint32_t, OpenNewBuyInvoiceTransaction, uint32_t,InvoiceScope)
	RCF_METHOD_R2(uint32_t, OpenEditBuyInvoiceTransaction, uint32_t,uint32_t) //sessionToken, invoiceId
//	RCF_METHOD_R2(uint32_t, OpenCorrectBuyInvoiceTransaction, uint32_t,uint32_t)
	RCF_METHOD_V2(void, DeleteBuyInvoice, uint32_t,uint32_t) //sessionToken, invoiceId
	RCF_METHOD_V2(void, LockBuyInvoice, uint32_t,uint32_t) //sessionToken, invoiceId
	RCF_METHOD_V2(void, UnlockBuyInvoice, uint32_t,uint32_t) //sessionToken, invoiceId

	RCF_METHOD_R2(uint32_t, OpenNewSellInvoiceTransaction, uint32_t,InvoiceScope)
	RCF_METHOD_R2(uint32_t, OpenEditSellInvoiceTransaction, uint32_t,uint32_t) //sessionToken, invoiceId

	RCF_METHOD_V2(void, DeleteSellInvoice, uint32_t,uint32_t) //sessionToken, invoiceId
	RCF_METHOD_V2(void, LockSellInvoice, uint32_t,uint32_t) //sessionToken, invoiceId
	RCF_METHOD_V2(void, UnlockSellInvoice, uint32_t,uint32_t) //sessionToken, invoiceId
	
	RCF_METHOD_V2(void, MarkPaidSellInvoice, uint32_t,MarkInvoicePaidRequestObj) 
	RCF_METHOD_V2(void, MarkPaidBuyInvoice, uint32_t,MarkInvoicePaidRequestObj) 

	RCF_METHOD_R2(TransactionResponseObj,ExecuteOperation,uint32_t,TransactionRequestObj) 
	RCF_METHOD_V2(void,CommitTransaction,uint32_t,uint32_t)
	RCF_METHOD_V2(void,AbortTransaction,uint32_t,uint32_t)

	RCF_METHOD_R0(FetchPrintProfilesResponseObj,FetchProfiles)
	RCF_METHOD_V1(void, UpdatePrintProfile,PrintProfileRequestObj)
	RCF_METHOD_R1(uint32_t, NewPrintProfile, PrintProfileRequestObj)
	RCF_METHOD_V1(void ,DeletePrintProfile,uint32_t)

	RCF_METHOD_R0(FetchPassportRecognitionProfilesResponseObj, FetchPassportRecognitionProfiles)
	RCF_METHOD_V1(void, UpdatePassportRecognitionProfile, PassportRecognitionProfileRequestObj)
	RCF_METHOD_R1(uint32_t, NewPassportRecognitionProfile, PassportRecognitionProfileRequestObj)
	RCF_METHOD_V1(void, DeletePassportRecognitionProfile, uint32_t)
	
	RCF_METHOD_R0(FetchDeductionDefinitionsResponseObj, FetchDeductionDefinitions)
	RCF_METHOD_V1(void, UpdateDeductionDefinition, DeductionDefinitionRequestObj)
	RCF_METHOD_R1(uint32_t, NewDeductionDefinition, DeductionDefinitionRequestObj)
	RCF_METHOD_V1(void, DeleteDeductionDefinition, uint32_t)
	
	RCF_METHOD_V1(void, OpenPurchaseRepository, uint32_t)
	RCF_METHOD_R2(int, QueryPurchases, uint32_t, PurchaseQueryObj)	
	RCF_METHOD_R2(FetchPurchaseResponseObj, FetchPurchase, uint32_t, std::wstring)
	RCF_METHOD_R1(FetchPurchaseHeadersResponseObj, FetchPurchaseHeaders, uint32_t)
	RCF_METHOD_R2(uint32_t, OpenDownloadPurchaseTransaction, uint32_t, std::wstring)
	RCF_METHOD_V1(void, ClosePurchaseRepository, uint32_t)

	/*RCF_METHOD_R0(DispatchHeaders, QueryDispatchInbox)
	RCF_METHOD_R1(Dispatch, FetchDispatch, uint32_t)
	RCF_METHOD_V1(void, ImportDispatch, uint32_t)*/
	
RCF_END(IBKService)

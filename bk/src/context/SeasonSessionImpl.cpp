#include "stdafx.h"
#include "SeasonSessionImpl.h"
#include "XContext.h"
#include "ContextException.h"
#include <boost/range/any_range.hpp>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <future>
#include <string\TextUtils.h>
#include <service\messages\TransactionRequest.h>
#include <service\messages\TransactionResponse.h>
#include "transaction\TransactionException.h"
#include <data\IInvoice.h>
#include <stdexcept>
#include "../runtime/TaskExecutor.h"

//-------------------------------------------------------------------------------------------------
SeasonSessionImpl::TransactionInterface::TransactionInterface(SeasonSessionImpl* pSession,uint32_t transactionId) : m_pSession(pSession),m_transactionId(transactionId)
{
}

boost::any SeasonSessionImpl::TransactionInterface::Execute(uint32_t operation,const OperationArguments& args)
{
	TransactionRequest* rq = new TransactionRequest();
	rq->WithTransactionId(m_transactionId);
	rq->WithOperation(operation);
	rq->WithParameters(args);
	try
	{
		TransactionResponseObj response =  m_pSession->m_pContext->InvokeClientR(&BKServiceClient::ExecuteOperation,m_pSession->m_sessionToken,TransactionRequestObj(rq));
		return response->GetResult();
	}
	catch(ContextException& e)
	{
		BOOST_THROW_EXCEPTION(TransactionException(e.message()));
	}
}

boost::any SeasonSessionImpl::TransactionInterface::Execute(uint32_t operation)
{
	return Execute(operation,OperationArguments());	
}
void SeasonSessionImpl::TransactionInterface::Commit()
{
	try
	{		
		m_pSession->m_pContext->InvokeClientR(&BKServiceClient::CommitTransaction,m_pSession->m_sessionToken,m_transactionId);
	}
	catch(ContextException& e)
	{
		BOOST_THROW_EXCEPTION(TransactionException(e.wwhat()));
	}
}

void SeasonSessionImpl::TransactionInterface::Rollback()
{
	try
	{
		m_pSession->m_pContext->InvokeClientR(&BKServiceClient::AbortTransaction,m_pSession->m_sessionToken,m_transactionId);
	}
	catch(RCF::Exception& e)
	{
		BOOST_THROW_EXCEPTION(TransactionException(TextUtils::FromUTF8(e.getWhat())));
	}
}

//-------------------------------------------------------------------------------------------------

SeasonSessionImpl::SeasonSessionImpl() :
	m_sessionToken(0),
	m_pContext(NULL),
	m_pTracker(NULL),
	m_season(NULL)
{	
}

ISeason* SeasonSessionImpl::GetSeason()
{
	return m_season;
}

XContext* SeasonSessionImpl::GetContext()
{
	return m_pContext;
}

void SeasonSessionImpl::AddSeasonObserver(SeasonObserver* pObserver)
{
	AddObserver(pObserver);
}

void SeasonSessionImpl::RemoveSeasonObserver(SeasonObserver* pObserver)
{
	RemoveObserver(pObserver);
}

void SeasonSessionImpl::Close()
{	
	m_pContext->CloseSession(this);		
}

void SeasonSessionImpl::BeginFetch()
{
	m_fetchManifest = m_pContext->InvokeClientR(&BKServiceClient::BeginFetch, m_sessionToken);
}

void SeasonSessionImpl::EndFetch()
{
	m_pContext->InvokeClient(&BKServiceClient::EndFetch,m_sessionToken);
	m_fetchManifest.reset();
}

void SeasonSessionImpl::FetchHents(SimpleProgress* progress)
{
	do
	{
		FetchHentsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchHents,m_sessionToken);
		if (response->GetHentsCount() == 0)
		{
			break;
		}
		PtrEnumerator<const HentWireObject> hentEnum = response->GetHents();

		Hent* pHent = NULL;
		const HentWireObject* pSrcHent = NULL;
		HentClientObject* pHentCO = NULL;
		while (hentEnum.hasNext())
		{
			pSrcHent = *hentEnum;
			pHent = new Hent(pSrcHent->GetId());
			pHent->CopyFrom(pSrcHent);
			pHentCO = new HentClientObject(pHent);
			pHentCO->SetLocked(pSrcHent->GetIsLocked());
			m_Hents.insert(pHentCO);
			hentEnum.advance();
		}
		progress->Step(response->GetHentsCount());
	} while (true);
}

InDocTransactionPtr SeasonSessionImpl::NewInDoc(uint32_t herdId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewInDocTransaction,m_sessionToken, herdId);
	InDocTransaction* pTransaction = new InDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InDocTransactionPtr(pTransaction);
}

InDocTransactionPtr SeasonSessionImpl::EditInDoc(uint32_t docId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditInDocTransaction,m_sessionToken, docId);
	InDocTransaction* pTransaction = new InDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InDocTransactionPtr(pTransaction);
}

MoveDocTransactionPtr SeasonSessionImpl::NewMoveDoc(uint32_t fromHerdId,uint32_t toHerdId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewMoveDocTransaction,m_sessionToken, fromHerdId, toHerdId);
	MoveDocTransaction* pTransaction = new MoveDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return MoveDocTransactionPtr(pTransaction);
}

MoveDocTransactionPtr SeasonSessionImpl::EditMoveDoc(uint32_t docId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditMoveDocTransaction,m_sessionToken, docId);
	MoveDocTransaction* pTransaction = new MoveDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return MoveDocTransactionPtr(pTransaction);
}

SellDocTransactionPtr SeasonSessionImpl::NewSellDoc(uint32_t herdId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewSellDocTransaction,m_sessionToken, herdId);
	SellDocTransaction* pTransaction = new SellDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return SellDocTransactionPtr(pTransaction);
}

SellDocTransactionPtr SeasonSessionImpl::EditSellDoc(uint32_t docId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditSellDocTransaction,m_sessionToken, docId);
	SellDocTransaction* pTransaction = new SellDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return SellDocTransactionPtr(pTransaction);
}

OutDocTransactionPtr SeasonSessionImpl::NewOutDoc(uint32_t herdId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewOutDocTransaction,m_sessionToken,herdId);
	OutDocTransaction* pTransaction = new OutDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return OutDocTransactionPtr(pTransaction);
}

OutDocTransactionPtr SeasonSessionImpl::EditOutDoc(uint32_t docId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditOutDocTransaction,m_sessionToken,docId);
	OutDocTransaction* pTransaction = new OutDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return OutDocTransactionPtr(pTransaction);
}

BuyDocTransactionPtr SeasonSessionImpl::NewBuyDoc(uint32_t herdId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewBuyDocTransaction,m_sessionToken,herdId);
	BuyDocTransaction* pTransaction = new BuyDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return BuyDocTransactionPtr(pTransaction);
}

BuyDocTransactionPtr SeasonSessionImpl::EditBuyDoc(uint32_t docId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditBuyDocTransaction,m_sessionToken,docId);
	BuyDocTransaction* pTransaction = new BuyDocTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return BuyDocTransactionPtr(pTransaction);
}



HerdRegistry* SeasonSessionImpl::GetHerdRegistry(uint32_t herdId)
{
	auto herdRegIt = m_Registers.find(herdId);
	if(m_Registers.end() != herdRegIt)
	{
		return herdRegIt->second;
	}

	return NULL;
}

void SeasonSessionImpl::FetchCows(SimpleProgress* progress)
{
	do
	{
		FetchCowsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchCows,m_sessionToken);

		if (response->GetCowsCount() == 0)
		{
			break;
		}

		PtrEnumerator<const CowWireObject> cowEnum = response->GetCows();
		Cow* pCow = NULL;
		const CowWireObject *pSrcCow = NULL;
		CowClientObject* pCowCO;
		while (cowEnum.hasNext())
		{
			pSrcCow = *cowEnum;
			pCow = new Cow(pSrcCow->GetId());
			pCow->CopyFrom(pSrcCow);
			pCowCO = new CowClientObject(pCow);
			pCowCO->SetLocked(pSrcCow->GetIsLocked());
			m_Cows.insert(pCowCO);
			m_pTracker->StartTrail(pCow);
			cowEnum.advance();
		}

		progress->Step(response->GetCowsCount());
	} while (true);
}

void SeasonSessionImpl::FetchInDocs(SimpleProgress* progress)
{
	do
	{
		FetchInDocsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchInDocs,m_sessionToken);
		if (response->GetDocsCount() == 0)
		{
			break;
		}

		PtrEnumerator<InDocWireObject> docEnum = response->GetDocs();
		InDoc* pDoc = NULL;
		InDocWireObject *pSrcDoc = NULL;
		InDocClientObject* pInDocCO = NULL;
		while (docEnum.hasNext())
		{
			pSrcDoc = *docEnum;
			pDoc = new InDoc(pSrcDoc->GetId());
			pDoc->CopyFrom(pSrcDoc);
			pInDocCO = new InDocClientObject(pDoc);
			pInDocCO->SetLocked(pSrcDoc->GetIsLocked());
			m_InDocs.insert(pInDocCO);
			docEnum.advance();
		}
		progress->Step(response->GetDocsCount());
	} while (true);
}

void SeasonSessionImpl::FetchBuyDocs(SimpleProgress* progress)
{
	do
	{
		FetchBuyDocsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchBuyDocs,m_sessionToken);
		if (response->GetDocsCount() == 0)
		{
			break;
		}

		PtrEnumerator<BuyDocWireObject> docEnum = response->GetDocs();
		BuyDoc* pDoc = NULL;
		BuyDocClientObject* pDocCO = NULL;
		BuyDocWireObject *pSrcDoc = NULL;
		while (docEnum.hasNext())
		{
			pSrcDoc = *docEnum;
			pDoc = new BuyDoc(pSrcDoc->GetId());
			pDoc->CopyFrom(pSrcDoc);
			pDocCO = new BuyDocClientObject(pDoc);
			pDocCO->SetLocked(pSrcDoc->GetIsLocked());
			m_BuyDocs.insert(pDocCO);
			docEnum.advance();
		}
		progress->Step(response->GetDocsCount());
	} while (true);
}

void SeasonSessionImpl::FetchMoveDocs(SimpleProgress* progress)
{
	do
	{
		FetchMoveDocsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchMoveDocs,m_sessionToken);
		if (response->GetDocsCount() == 0)
		{
			break;
		}

		PtrEnumerator<MoveDocWireObject> docEnum = response->GetDocs();
		MoveDoc* pDoc = NULL;
		MoveDocWireObject *pSrcDoc = NULL;
		MoveDocClientObject* pDocCO = NULL;
		while (docEnum.hasNext())
		{
			pSrcDoc = *docEnum;
			pDoc = new MoveDoc(pSrcDoc->GetId());
			pDoc->CopyFrom(pSrcDoc);
			pDocCO = new MoveDocClientObject(pDoc);
			pDocCO->SetLocked(pSrcDoc->GetIsLocked());
			m_MoveDocs.insert(pDocCO);
			docEnum.advance();
		}
		progress->Step(response->GetDocsCount());
	} while (true);
}

void SeasonSessionImpl::FetchSellDocs(SimpleProgress* progress)
{
	do
	{
		FetchSellDocsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchSellDocs,m_sessionToken);
		if (response->GetDocsCount() == 0)
		{
			break;
		}

		PtrEnumerator<SellDocWireObject> docEnum = response->GetDocs();
		SellDoc* pDoc = NULL;
		SellDocClientObject* pDocCO = NULL;
		SellDocWireObject *pSrcDoc = NULL;

		while (docEnum.hasNext())
		{
			pSrcDoc = *docEnum;
			pDoc = new SellDoc(pSrcDoc->GetId());
			pDoc->CopyFrom(pSrcDoc);
			pDocCO = new SellDocClientObject(pDoc);
			pDocCO->SetLocked(pSrcDoc->GetIsLocked());
			m_SellDocs.insert(pDocCO);
			docEnum.advance();
		}
		progress->Step(response->GetDocsCount());
	} while (true);
}

void SeasonSessionImpl::FetchOutDocs(SimpleProgress* progress)
{
	do
	{
		FetchOutDocsResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchOutDocs,m_sessionToken);
		if (response->GetDocsCount() == 0)
		{
			break;
		}

		PtrEnumerator<OutDocWireObject> docEnum = response->GetDocs();
		OutDoc* pDoc = NULL;
		OutDocClientObject* pDocCO = NULL;
		OutDocWireObject *pSrcDoc = NULL;
		while (docEnum.hasNext())
		{
			pSrcDoc = *docEnum;
			pDoc = new OutDoc(pSrcDoc->GetId());
			pDoc->CopyFrom(pSrcDoc);
			pDocCO = new OutDocClientObject(pDoc);
			pDocCO->SetLocked(pSrcDoc->GetIsLocked());
			m_OutDocs.insert(pDocCO);
			docEnum.advance();
		}
		progress->Step(response->GetDocsCount());
	} while (true);
}


void SeasonSessionImpl::FetchBuyInvoices(SimpleProgress* progress)
{
	do
	{
		FetchInvoicesResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchBuyInvoices,m_sessionToken);
		if (response->GetInvoicesCount() == 0)
		{
			break;
		}

		PtrEnumerator<const WireInvoice> invoiceEnum = response->GetInvoices();
		BuyInvoice* pInvoice = NULL;
		BuyInvoiceClientObject* pInvoiceCO = NULL;
		const WireInvoice *pSrcInvoice = NULL;
		while (invoiceEnum.hasNext())
		{
			pSrcInvoice = *invoiceEnum;
			pInvoice = new BuyInvoice(pSrcInvoice->GetId(), pSrcInvoice->GetInvoiceScope());
			pInvoice->CopyFrom(pSrcInvoice);
			pInvoiceCO = new BuyInvoiceClientObject(pInvoice);
			pInvoiceCO->SetLocked(pSrcInvoice->GetIsLocked());
			m_BuyInvoices.insert(pInvoiceCO);
			invoiceEnum.advance();
		}
		progress->Step(response->GetInvoicesCount());
	} while (true);
}

void SeasonSessionImpl::FetchSellInvoices(SimpleProgress* progress)
{
	do
	{
		FetchInvoicesResponseObj response = m_pContext->InvokeClientR(&BKServiceClient::FetchSellInvoices,m_sessionToken);
		if (response->GetInvoicesCount() == 0)
		{
			break;
		}

		PtrEnumerator<const WireInvoice> invoiceEnum = response->GetInvoices();
		SellInvoice* pInvoice = NULL;
		SellInvoiceClientObject* pInvoiceCO = NULL;
		const WireInvoice *pSrcInvoice = NULL;
		while (invoiceEnum.hasNext())
		{
			pSrcInvoice = *invoiceEnum;
			pInvoice = new SellInvoice(pSrcInvoice->GetId(), pSrcInvoice->GetInvoiceScope());
			pInvoice->CopyFrom(pSrcInvoice);
			pInvoiceCO = new SellInvoiceClientObject(pInvoice);
			pInvoiceCO->SetLocked(pSrcInvoice->GetIsLocked());
			m_SellInvoices.insert(pInvoiceCO);
			invoiceEnum.advance();
		}
		progress->Step(response->GetInvoicesCount());
	} while (true);
}

CowTrail* SeasonSessionImpl::FindCowTrail(uint32_t cowId)
{
	CowTrail* pTrail = m_pTracker->GetTrail(cowId);
	if(!pTrail)
		throw ContextException(TextUtils::FormatA(L"there is not trail started for cow %d",cowId));
	return pTrail;
}

void SeasonSessionImpl::FileAnInDoc(InDoc* pInDoc)
{
	PtrEnumerator<ICowEntry> docCowsEnum = pInDoc->EnumCows();
	auto registerIt = m_Registers.find(pInDoc->GetHerdId());

	if(registerIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"indoc %d refres to herd %d which is not in the registry", pInDoc->GetId(),pInDoc->GetHerdId()));

	HerdRegistry* pRegister = registerIt->second;

	pRegister->AddInDoc(pInDoc);
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);
	while(docCowsEnum.hasNext())
	{
		ICowEntry* pCowEntry = *docCowsEnum;
		uint32_t cowId = pCowEntry->GetCowId();
		auto cow = cowsById.find(cowId);
		if(cow == cowsById.end())
			throw ContextException(TextUtils::FormatA(L"indoc %d refres to cow %d which is not in the index", pInDoc->GetId(),pCowEntry->GetCowId()));
		pRegister->AddEnteringEvent((*cow)->Object(),pInDoc,pCowEntry);
						
		CowTrail* pTrail = FindCowTrail(cowId);
		pTrail->SetEntryFootprint(pInDoc,pCowEntry);	

		docCowsEnum.advance();
	}
}

void SeasonSessionImpl::CatalogInDocs(SimpleProgress* progress)
{	
	InDocClientObject* pInDocCO = NULL;
	InDoc* pInDoc = NULL;
	InDocsIndex::iterator docIt = m_InDocs.begin();
	while(docIt != m_InDocs.end())
	{
		pInDocCO = *docIt;
		pInDoc = pInDocCO->Object();
		FileAnInDoc(pInDoc);		
		docIt++;
		progress->Step();
	}
}

void SeasonSessionImpl::FileABuyDoc(BuyDoc* pDoc)
{
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);

	PtrEnumerator<ICowEntry> docCowsEnum = pDoc->EnumCows();
	auto registerIt = m_Registers.find(pDoc->GetHerdId());

	if(registerIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"buydoc %d refres to herd %d which is not in the registry", pDoc->GetId(),pDoc->GetHerdId()));

	HerdRegistry* pRegister = registerIt->second;

	pRegister->AddBuyDoc(pDoc);
	while(docCowsEnum.hasNext())
	{
		ICowEntry* pCowEntry = *docCowsEnum;
		uint32_t cowId = pCowEntry->GetCowId();
		auto cow = cowsById.find(cowId);
		if(cow == cowsById.end())
			throw ContextException(TextUtils::FormatA(L"buydoc %d refres to cow %d which is not in the index", pDoc->GetId(),cowId));
		pRegister->AddEnteringEvent((*cow)->Object(),pDoc,pCowEntry);
		docCowsEnum.advance();

		CowTrail* pTrail = FindCowTrail(cowId);
		pTrail->SetEntryFootprint(pDoc,pCowEntry);
	}
}

void SeasonSessionImpl::CatalogBuyDocs(SimpleProgress* progress)
{
	BuyDocClientObject* pBuyDocCO = NULL;
	BuyDoc* pBuyDoc = NULL;
	BuyDocsIndex::iterator docIt = m_BuyDocs.begin();
	while(docIt != m_BuyDocs.end())
	{
		pBuyDocCO = *docIt;
		pBuyDoc = pBuyDocCO->Object();
		FileABuyDoc(pBuyDoc);	
		docIt++;
		progress->Step();
	}

}

void SeasonSessionImpl::FileAMoveDoc(MoveDoc* pMoveDoc)
{
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);
	PtrEnumerator<ICowEntry> docCowsEnum = pMoveDoc->EnumCows();
	auto fromRegisterIt = m_Registers.find(pMoveDoc->GetSrcHerdId());
	auto toRegisterIt = m_Registers.find(pMoveDoc->GetDstHerdId());

	if(fromRegisterIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"movedoc %d refres to src herd %d which is not in the registry", pMoveDoc->GetId(),pMoveDoc->GetSrcHerdId()));

	if(toRegisterIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"movedoc %d refres to dst herd %d which is not in the registry", pMoveDoc->GetId(),pMoveDoc->GetDstHerdId()));


	HerdRegistry* pSrcRegister = fromRegisterIt->second;
	HerdRegistry* pDstRegister = toRegisterIt->second;

	pSrcRegister->AddMoveDoc(pMoveDoc);
	pDstRegister->AddMoveDoc(pMoveDoc);
	while(docCowsEnum.hasNext())
	{
		ICowEntry* pCowEntry = *docCowsEnum;
		uint32_t cowId = pCowEntry->GetCowId();
		auto cow = cowsById.find(cowId);
		if(cow == cowsById.end())
			throw ContextException(TextUtils::FormatA(L"movedoc %d refres to cow %d which is not in the index", pMoveDoc->GetId(),cowId));
		ICow* pCow = (*cow)->Object();

		pSrcRegister->AddLeavingEvent(pCow,pMoveDoc,pCowEntry);
		pDstRegister->AddEnteringEvent(pCow,pMoveDoc,pCowEntry);

		CowTrail* pTrail = FindCowTrail(cowId);
		pTrail->AddFootprint(pMoveDoc,pCowEntry);
			
		docCowsEnum.advance();
	}
}

void SeasonSessionImpl::CatalogMoveDocs(SimpleProgress* progress)
{	
	MoveDocClientObject* pMoveDocCO = NULL;
	MoveDoc* pMoveDoc = NULL;
	MoveDocsIndex::iterator docIt = m_MoveDocs.begin();
	while(docIt != m_MoveDocs.end())
	{
		pMoveDocCO = *docIt;
		pMoveDoc = pMoveDocCO->Object();
		FileAMoveDoc(pMoveDoc);	
		docIt++;
		progress->Step();
	}
}

void SeasonSessionImpl::FileASellDoc(SellDoc* pDoc)
{
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);

	PtrEnumerator<ICowEntry> docCowsEnum = pDoc->EnumCows();
	auto registerIt = m_Registers.find(pDoc->GetHerdId());

	if(registerIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"selldoc %d refres to herd %d which is not in the registry", pDoc->GetId(),pDoc->GetHerdId()));

	HerdRegistry* pRegister = registerIt->second;

	pRegister->AddSellDoc(pDoc);
	while(docCowsEnum.hasNext())
	{
		ICowEntry* pCowEntry = *docCowsEnum;
		uint32_t cowId = pCowEntry->GetCowId();
		auto cow = cowsById.find(cowId);
		if(cow == cowsById.end())
			throw ContextException(TextUtils::FormatA(L"selldoc %d refres to cow %d which is not in the index", pDoc->GetId(),cowId));
		pRegister->AddLeavingEvent((*cow)->Object(),pDoc,pCowEntry);

		CowTrail* pTrail = FindCowTrail(cowId);
		pTrail->SetExitFootprint(pDoc,pCowEntry);

		docCowsEnum.advance();
	}
}

void SeasonSessionImpl::CatalogSellDocs(SimpleProgress* progress)
{
	SellDocClientObject* pSellDocCO = NULL;
	SellDoc* pSellDoc = NULL;
	SellDocsIndex::iterator docIt = m_SellDocs.begin();
	while(docIt != m_SellDocs.end())
	{
		pSellDocCO = *docIt;
		pSellDoc = pSellDocCO->Object();
		FileASellDoc(pSellDoc);		
		docIt++;
		progress->Step();
	}
}

void SeasonSessionImpl::FileAnOutDoc(OutDoc* pDoc)
{
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);
	PtrEnumerator<ICowEntry> docCowsEnum = pDoc->EnumCows();
	auto registerIt = m_Registers.find(pDoc->GetHerdId());

	if(registerIt == m_Registers.end())
		throw ContextException(TextUtils::FormatA(L"outdoc %d refres to herd %d which is not in the registry", pDoc->GetId(),pDoc->GetHerdId()));

	HerdRegistry* pRegister = registerIt->second;

	pRegister->AddOutDoc(pDoc);
	while(docCowsEnum.hasNext())
	{
		ICowEntry* pCowEntry = *docCowsEnum;
		uint32_t cowId = pCowEntry->GetCowId();
		auto cow = cowsById.find(cowId);
		if(cow == cowsById.end())
			throw ContextException(TextUtils::FormatA(L"outdoc %d refres to cow %d which is not in the index", pDoc->GetId(),cowId));
		pRegister->AddLeavingEvent((*cow)->Object(),pDoc,pCowEntry);
		CowTrail* pTrail = FindCowTrail(cowId);
		pTrail->SetExitFootprint(pDoc,pCowEntry);
		docCowsEnum.advance();
	}
}

void SeasonSessionImpl::CatalogOutDocs(SimpleProgress* progress)
{
	
	
	OutDocClientObject* pOutDocCO = NULL;
	OutDoc* pOutDoc = NULL;
	OutDocsIndex::iterator docIt = m_OutDocs.begin();
	while(docIt != m_OutDocs.end())
	{
		pOutDocCO = *docIt;
		pOutDoc = pOutDocCO->Object();
		FileAnOutDoc(pOutDoc);		
		docIt++;
		progress->Step();
	}

}

void SeasonSessionImpl::DoFetch(ComplexProgress* progress)
{
	try
	{
		TaskExecutor executor(progress);
		executor << task(L"fetch_hents", m_fetchManifest->HentCount(), std::bind(&SeasonSessionImpl::FetchHents, this, std::placeholders::_1))
			<< task(L"fetch_cows", m_fetchManifest->CowCount(), std::bind(&SeasonSessionImpl::FetchCows, this, std::placeholders::_1))
			<< task(L"fetch_indocs", m_fetchManifest->InDocCount(), std::bind(&SeasonSessionImpl::FetchInDocs, this, std::placeholders::_1))
			<< task(L"fetch_buydocs", m_fetchManifest->BuyDocCount(), std::bind(&SeasonSessionImpl::FetchBuyDocs, this, std::placeholders::_1))
			<< task(L"fetch_movedocs", m_fetchManifest->MoveDocCount(), std::bind(&SeasonSessionImpl::FetchMoveDocs, this, std::placeholders::_1))
			<< task(L"fetch_selldocs", m_fetchManifest->SellDocCount(), std::bind(&SeasonSessionImpl::FetchSellDocs, this, std::placeholders::_1))
			<< task(L"fetch_outdocs", m_fetchManifest->OutDocCount(), std::bind(&SeasonSessionImpl::FetchOutDocs, this, std::placeholders::_1))
			<< task(L"fetch_buyinvoices", m_fetchManifest->BuyInvoicesCount(), std::bind(&SeasonSessionImpl::FetchBuyInvoices, this, std::placeholders::_1))
			<< task(L"fetch_sellinvoces", m_fetchManifest->SellInvoicesCount(), std::bind(&SeasonSessionImpl::FetchSellInvoices, this, std::placeholders::_1))
			<< task(L"creating_herd_registry", 1, [this](SimpleProgress* progress)
													{
														uint32_t herdId;
														PtrEnumerator<IHerd> herdEnum = m_pContext->EnumHerds();
														while (herdEnum.hasNext())
														{
															herdId = (*herdEnum)->GetId();
															m_Registers[herdId] = (new HerdRegistry(herdId));
															herdEnum.advance();
														}
														progress->Step();
													}
													)
			<< task(L"catalog_indocs", m_InDocs.size(), std::bind(&SeasonSessionImpl::CatalogInDocs, this, std::placeholders::_1))
			<< task(L"catalog_buydocs", m_BuyDocs.size(), std::bind(&SeasonSessionImpl::CatalogBuyDocs, this, std::placeholders::_1))
			<< task(L"catalog_movedocs", m_MoveDocs.size(), std::bind(&SeasonSessionImpl::CatalogMoveDocs, this, std::placeholders::_1))
			<< task(L"catalog_outdocs", m_OutDocs.size(), std::bind(&SeasonSessionImpl::CatalogOutDocs, this, std::placeholders::_1))
			<< task(L"catalog_selldocs", m_SellDocs.size(), std::bind(&SeasonSessionImpl::CatalogSellDocs, this, std::placeholders::_1));

		executor.Run();
				
		m_pNtfnSubsriber->Subsrcibe(m_observerEndPoint,m_pRelay);

	}
	catch(RCF::Exception& e)
	{
		throw ContextException(TextUtils::FromUTF8(e.getWhat()));
	}

}


	
 PtrEnumerator<HentClientObject> SeasonSessionImpl::EnumHents()
 {
	 return PtrEnumerator<HentClientObject>(m_Hents.begin(),m_Hents.end());
 }

 HentClientObject* SeasonSessionImpl::GetHent(uint32_t hentId)
 {
	 if(hentId != NULL_ID)
	 {		
		 const boost::multi_index::index<HentsIndex,hentsindex_tag_hentId>::type& hentsById = 
			boost::multi_index::get<hentsindex_tag_hentId>(m_Hents);
	
		 auto hentCOIt = hentsById.find(hentId);
		 if(hentCOIt != hentsById.end())
			 return *hentCOIt;
	 }

	 return NULL;
 }

HentClientObject* SeasonSessionImpl::GetHent(const HentNo& hentNo)
{
	 const boost::multi_index::index<HentsIndex,hentsindex_tag_hentNo>::type& hentsByNo = 
		boost::multi_index::get<hentsindex_tag_hentNo>(m_Hents);
	 	
	 auto hentCOIt = hentsByNo.find(hentNo);
	 if(hentCOIt != hentsByNo.end())
		 return *hentCOIt;

	 return NULL;
}

CowClientObject* SeasonSessionImpl::GetCow(uint32_t cowId)
{
	const boost::multi_index::index<CowsIndex,index_tag_Id>::type& cowsById = boost::multi_index::get<index_tag_Id>(m_Cows);
	
	 auto cowCOIt = cowsById.find(cowId);
	 if(cowCOIt != cowsById.end())
		 return *cowCOIt;

	 return NULL;
}

IClass* SeasonSessionImpl::GetClass(uint32_t classId)
{
	return m_pContext->GetClass(classId);
}

IClass* SeasonSessionImpl::GetClassByCd(const std::wstring& classCd)
{
	return m_pContext->GetClassByCd(classCd);
}

IStock* SeasonSessionImpl::GetStock(uint32_t stockId)
{	
	return m_pContext->GetStock(stockId);
}


ICowTrail* SeasonSessionImpl::GetCowTrail(uint32_t cowId)
{
	return m_pTracker->GetTrail(cowId);
}

std::vector<ICow*> SeasonSessionImpl::GetDocCows(IDoc* pDoc)
{
	std::vector<ICow*> cows;
	ICowEntry* pEntry;
	PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
	while(cowsEnum.hasNext())
	{
		pEntry = *cowsEnum;
		cows.push_back(GetCow(pEntry->GetCowId())->Object());
		cowsEnum.advance();
	}
	return std::move(cows);
}

IHerd* SeasonSessionImpl::GetHerd(uint32_t herdId)
{
	return m_pContext->GetHerd(herdId);
}

IHerd* SeasonSessionImpl::GetHerdByIndex(int herdIndex)
{
	return m_pContext->GetHerdByIndex(herdIndex);
}

uint32_t SeasonSessionImpl::InsertHent(const IHent* pHentToInsert)
{
	HentRequestObj hentRq(new HentRequest());
	hentRq->CreateHent(pHentToInsert->GetId())->CopyFrom(pHentToInsert);
	return m_pContext->InvokeClientR<HentRequestObj,uint32_t>(&BKServiceClient::InsertHent, m_sessionToken, hentRq);
}

void SeasonSessionImpl::UpdateHent(const IHent* pHentToUpdate)
{
	HentRequestObj hentRq(new HentRequest());
	hentRq->CreateHent(pHentToUpdate->GetId())->CopyFrom(pHentToUpdate);
	m_pContext->InvokeClient(&BKServiceClient::UpdateHent, m_sessionToken, hentRq);
}

void SeasonSessionImpl::DeleteHent(uint32_t hentId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteHent, m_sessionToken, hentId);
}

void SeasonSessionImpl::LockHent(uint32_t hentId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockHent, m_sessionToken, hentId);
}

void SeasonSessionImpl::UnlockHent(uint32_t hentId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockHent, m_sessionToken, hentId);
}

PtrEnumerator<IStock> SeasonSessionImpl::EnumStocks()
{
	return m_pContext->EnumStocks();
}

PtrEnumerator<IClass> SeasonSessionImpl::EnumClasses()
{
	return m_pContext->EnumClasses();
}

void SeasonSessionImpl::LockCow(uint32_t cowId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockCow,m_sessionToken, cowId);
}

void SeasonSessionImpl::UnlockCow(uint32_t cowId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockCow,m_sessionToken, cowId);
}

void SeasonSessionImpl::UpdateCow(uint32_t cowId,const CowDetails& details)
{
	CowRequestObj request(new CowRequest(cowId));
	request->WithDetails(details);
	m_pContext->InvokeClient(&BKServiceClient::UpdateCow,m_sessionToken, request);
}

PtrEnumerator<IAgent> SeasonSessionImpl::EnumAgents()
{
	return m_pContext->EnumAgents();
}

IAgent* SeasonSessionImpl::GetAgent(uint32_t agentId)
{
	return m_pContext->GetAgent(agentId);
}

BuyDocClientObject* SeasonSessionImpl::GetBuyDoc(uint32_t docId)
{
	return FindCOWithId(m_BuyDocs,docId);	
}
InDocClientObject* SeasonSessionImpl::GetInDoc(uint32_t docId)
{
	return FindCOWithId(m_InDocs,docId);	
}

SellDocClientObject* SeasonSessionImpl::GetSellDoc(uint32_t docId)
{
	return FindCOWithId(m_SellDocs,docId);
}

MoveDocClientObject* SeasonSessionImpl::GetMoveDoc(uint32_t docId)
{
	return FindCOWithId(m_MoveDocs,docId);
}

OutDocClientObject* SeasonSessionImpl::GetOutDoc(uint32_t docId)
{
	return FindCOWithId(m_OutDocs,docId);
}

BuyInvoiceClientObject*  SeasonSessionImpl::GetBuyInvoice(uint32_t invoiceId)
{
	return FindCOWithId(m_BuyInvoices,invoiceId);
}

SellInvoiceClientObject*  SeasonSessionImpl::GetSellInvoice(uint32_t invoiceId)
{
	return FindCOWithId(m_SellInvoices,invoiceId);
}

void SeasonSessionImpl::LockInDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockInDoc,m_sessionToken,docId);
}

void SeasonSessionImpl::UnlockInDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockInDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::LockSellDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockSellDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::UnlockSellDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockSellDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::DeleteSellDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteSellDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::LockMoveDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockMoveDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::UnlockMoveDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockMoveDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::DeleteMoveDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteMoveDoc,m_sessionToken,docId);
}

void SeasonSessionImpl::LockOutDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockOutDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::UnlockOutDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockOutDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::DeleteOutDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteOutDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::LockBuyDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockBuyDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::UnlockBuyDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockBuyDoc,m_sessionToken,docId);
}

void SeasonSessionImpl::DeleteBuyDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteBuyDoc,m_sessionToken, docId);
}

void SeasonSessionImpl::DeleteInDoc(uint32_t docId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteInDoc,m_sessionToken,docId);
}

void SeasonSessionImpl::LockBuyInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockBuyInvoice,m_sessionToken,invoiceId);
}

void SeasonSessionImpl::UnlockBuyInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockBuyInvoice,m_sessionToken, invoiceId);
}

void SeasonSessionImpl::DeleteBuyInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteBuyInvoice,m_sessionToken, invoiceId);
}

void SeasonSessionImpl::LockSellInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::LockSellInvoice,m_sessionToken, invoiceId);
}

void SeasonSessionImpl::UnlockSellInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::UnlockSellInvoice,m_sessionToken, invoiceId);
}

void SeasonSessionImpl::DeleteSellInvoice(uint32_t invoiceId)
{
	m_pContext->InvokeClient(&BKServiceClient::DeleteSellInvoice,m_sessionToken, invoiceId);
}

void SeasonSessionImpl::AddHerdObserver(uint32_t herdId,HerdObserver* pObserver)
{
	boost::iterator_range<std::multimap<uint32_t,HerdObserver*>::iterator> observersRange = m_HerdObservers.equal_range(herdId);

	auto observerIt = std::find_if(observersRange.begin(), observersRange.end(), 
									[pObserver](std::pair<uint32_t,HerdObserver*> pair) 
									{ 
										return pObserver == pair.second; 
									});

	if(observerIt == observersRange.end())
	{
		m_HerdObservers.insert(std::pair<uint32_t,HerdObserver*>(herdId,pObserver));
	}
}

void SeasonSessionImpl::RemoveHerdObserver(uint32_t herdId,HerdObserver* pObserver)
{
	auto observersRange = m_HerdObservers.equal_range(herdId);
	auto observerIt = std::find_if(observersRange.first, observersRange.second, 
									[pObserver](std::pair<uint32_t,HerdObserver*> pair) 
									{ 
										return pObserver == pair.second; 
									});
	if(observerIt != observersRange.second) 
		m_HerdObservers.erase(observerIt);	
}

InvoiceDocsTransactionPtr SeasonSessionImpl::NewBuyInvoiceDocs()
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewBuyInvoiceTransaction,m_sessionToken, InvoiceScope_Docs);
	InvoiceDocsTransaction* pTransaction = new InvoiceDocsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceDocsTransactionPtr(pTransaction);
}

InvoiceDocsTransactionPtr SeasonSessionImpl::EditBuyInvoiceDocs(uint32_t invoiceId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditBuyInvoiceTransaction,m_sessionToken, invoiceId);
	InvoiceDocsTransaction* pTransaction = new InvoiceDocsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceDocsTransactionPtr(pTransaction);
}

InvoiceCowsTransactionPtr SeasonSessionImpl::NewBuyInvoiceCows()
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewBuyInvoiceTransaction,m_sessionToken, InvoiceScope_Cows);
	InvoiceCowsTransaction* pTransaction = new InvoiceCowsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceCowsTransactionPtr(pTransaction);
}

InvoiceCowsTransactionPtr SeasonSessionImpl::EditBuyInvoiceCows(uint32_t invoiceId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditBuyInvoiceTransaction,m_sessionToken, invoiceId);
	InvoiceCowsTransaction* pTransaction = new InvoiceCowsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceCowsTransactionPtr(pTransaction);
}

InvoiceCowsTransactionPtr SeasonSessionImpl::NewSellInvoiceCows()
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewSellInvoiceTransaction,m_sessionToken, InvoiceScope_Cows);
	InvoiceCowsTransaction* pTransaction = new InvoiceCowsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceCowsTransactionPtr(pTransaction);
}

InvoiceCowsTransactionPtr SeasonSessionImpl::EditSellInvoiceCows(uint32_t invoiceId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditSellInvoiceTransaction,m_sessionToken, invoiceId);
	InvoiceCowsTransaction* pTransaction = new InvoiceCowsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceCowsTransactionPtr(pTransaction);
}

InvoiceDocsTransactionPtr SeasonSessionImpl::NewSellInvoiceDocs()
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenNewSellInvoiceTransaction,m_sessionToken, InvoiceScope_Docs);
	InvoiceDocsTransaction* pTransaction = new InvoiceDocsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceDocsTransactionPtr(pTransaction);
}

InvoiceDocsTransactionPtr SeasonSessionImpl::EditSellInvoiceDocs(uint32_t invoiceId)
{
	uint32_t transactionId = m_pContext->InvokeClientR(&BKServiceClient::OpenEditSellInvoiceTransaction,m_sessionToken, invoiceId);
	InvoiceDocsTransaction* pTransaction = new InvoiceDocsTransaction(new TransactionInterface(this,transactionId));
	pTransaction->Open();
	return InvoiceDocsTransactionPtr(pTransaction);
}

PtrEnumerator<BuyDocClientObject> SeasonSessionImpl::EnumBuyDocs()
{
	return PtrEnumerator<BuyDocClientObject>(m_BuyDocs.begin(),m_BuyDocs.end());
}

PtrEnumerator<SellDocClientObject> SeasonSessionImpl::EnumSellDocs()
{
	return PtrEnumerator<SellDocClientObject>(m_SellDocs.begin(), m_SellDocs.end());
}

PtrEnumerator<IHerd> SeasonSessionImpl::EnumHerds()
{
	return m_pContext->EnumHerds();
}

PtrEnumerator<BuyInvoiceClientObject> SeasonSessionImpl::EnumBuyInvoices()
{
	return PtrEnumerator<BuyInvoiceClientObject>(m_BuyInvoices.begin(),m_BuyInvoices.end());
}

PtrEnumerator<SellInvoiceClientObject> SeasonSessionImpl::EnumSellInvoices()
{
	return PtrEnumerator<SellInvoiceClientObject>(m_SellInvoices.begin(),m_SellInvoices.end());
}



IFarm* SeasonSessionImpl::GetFarm()
{
	return m_pContext->GetFarm();
}

UserSettings* SeasonSessionImpl::GetUserSettings()
{
	return m_pContext->GetUserSettings();
}

void SeasonSessionImpl::OpenPurchaseRepository()
{
	m_pContext->InvokeClient(&BKServiceClient::OpenPurchaseRepository, m_sessionToken);
}

int SeasonSessionImpl::QueryPurchases(const DateTime& startDt, const DateTime& endDt)
{
	PurchaseQueryObj query(new PurchaseQuery());
	query->WithStartDt(startDt);
	query->WithEndDt(endDt);
	return m_pContext->InvokeClientR<PurchaseQueryObj,int>(&BKServiceClient::QueryPurchases, m_sessionToken, query);
}

boost::optional<Purchase> SeasonSessionImpl::FetchPurchase(const std::wstring& purchaseId)
{
	boost::optional<Purchase> result;
	FetchPurchaseResponseObj rs = m_pContext->InvokeClientR<std::wstring,FetchPurchaseResponseObj>(&BKServiceClient::FetchPurchase, m_sessionToken, purchaseId);

	IPurchase* purchase = rs->Get();
	if (purchase != nullptr)
	{
		result = Purchase(purchase->GetId());
		result->CopyFrom(purchase);
	}
	return std::move(result);
}

PurchaseHeaders SeasonSessionImpl::FetchPurchaseHeaders()
{
	PurchaseHeaders result;
	FetchPurchaseHeadersResponseObj rs = m_pContext->InvokeClientR<FetchPurchaseHeadersResponseObj>(&BKServiceClient::FetchPurchaseHeaders,m_sessionToken);

	PtrEnumerator<IPurchaseHeader> enumPurchases = rs->Enum();
	while (enumPurchases.hasNext())
	{
		result.Add(*enumPurchases);
		enumPurchases.advance();
	};

	return std::move(result);
}

DownloadPurchaseTransactionPtr SeasonSessionImpl::OpenDownloadPurchaseTransaction(const std::wstring& purchaseId)
{
	uint32_t trainsactionId = m_pContext->InvokeClientR<std::wstring, uint32_t>(&BKServiceClient::OpenDownloadPurchaseTransaction, m_sessionToken, purchaseId);
	DownloadPurchaseTransactionPtr transaction(new DownloadPurchaseTransaction(new TransactionInterface(this, trainsactionId)));
	transaction->Open();
	return transaction;
	
}

void SeasonSessionImpl::ClosePurchaseRepository()
{
	m_pContext->InvokeClient(&BKServiceClient::ClosePurchaseRepository, m_sessionToken);
}

PtrEnumerator<IBankAccount> SeasonSessionImpl::EnumBankAccounts()
{
	return m_pContext->EnumBankAccounts();
}

IBankAccount* SeasonSessionImpl::GetBankAccount(uint32_t accountId)
{
	return m_pContext->GetBankAccount(accountId);
}
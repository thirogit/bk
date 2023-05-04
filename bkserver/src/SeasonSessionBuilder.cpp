#include "stdafx.h"
#include "SeasonSessionBuilder.h"
#include "dao/MuuDAO.h"
#include "SessionException.h"
#include "FarmSession.h"
#include <boost/thread.hpp>
#include "seasonimpl/ClientSeasonSessionInterfaceImpl.h"
#include <string/TextUtils.h>
#include <filesystem/paths.h>




SeasonSessionBuilder::LoggingSimpleProgress::LoggingSimpleProgress(const std::wstring& message, BkLogger* log) : m_log(log), m_maxSteps(0), m_currentPercent(0), m_message(message), m_currentStep(0)
{
}

void SeasonSessionBuilder::LoggingSimpleProgress::Log()
{
	m_log->i(TextUtils::Format(L"%s %d%%", m_message, m_currentPercent));
}

void SeasonSessionBuilder::LoggingSimpleProgress::SetSteps(int maxSteps)
{
	m_maxSteps = maxSteps;
	Log();
}

bool SeasonSessionBuilder::LoggingSimpleProgress::Step(int stepsToMake)
{
	m_currentStep += stepsToMake;

	int percent = (m_currentStep * 100) / m_maxSteps;

	if ((percent - m_currentPercent) >= 10)
	{
		m_currentPercent = percent;
		Log();
	}	

	return true;
}



SeasonSessionBuilder::LoggingComplexProgress::LoggingComplexProgress(const std::wstring& message, BkLogger* log) : m_log(log), m_message(message), m_currentTaskProgress(NULL)
{
}

SeasonSessionBuilder::LoggingComplexProgress::~LoggingComplexProgress()
{
	if (m_currentTaskProgress)
		delete m_currentTaskProgress;
}

void SeasonSessionBuilder::LoggingComplexProgress::SetTaskCount(int taskCount)
{
}

SimpleProgress* SeasonSessionBuilder::LoggingComplexProgress::BeginTask(const std::wstring& sTaskName)
{
	m_currentTaskProgress = new LoggingSimpleProgress(m_message + L": " + sTaskName, m_log);
	return m_currentTaskProgress;
}

void SeasonSessionBuilder::LoggingComplexProgress::EndTask()
{
	if (m_currentTaskProgress)
	{
		delete m_currentTaskProgress;
		m_currentTaskProgress = NULL;
	}
}





	

SeasonSessionBuilder::SeasonSessionBuilder(BkLogger* log) : m_pSeason(NULL), m_pFarmSession(NULL), m_pTokenGenerator(NULL), m_log(log)
{
}

void SeasonSessionBuilder::SetSeason(Season* pSeason)
{
	m_pSeason = pSeason;
}

void SeasonSessionBuilder::SetParentSession(FarmSession* pFarmSession)
{
	m_pFarmSession = pFarmSession;
}


void SeasonSessionBuilder::SetTokenGenerator(SessionTokenGenerator* generator)
{
	m_pTokenGenerator = generator;
}
	
SeasonSession* SeasonSessionBuilder::Build()
{
	MuuDAO dao;

	if(!m_pSeason)
		BOOST_THROW_EXCEPTION(SessionException(L"no season"));

	if(!m_pFarmSession)
		BOOST_THROW_EXCEPTION(SessionException(L"no parent farm session"));

	if (!m_pTokenGenerator)
		BOOST_THROW_EXCEPTION(SessionException(L"no token generator"));
	
	NewPtrFlushArray<Hent> hents;
	NewPtrFlushArray<Cow> cows;
	NewPtrFlushArray<InDoc> indocs;
	NewPtrFlushArray<BuyDoc> buydocs;
	NewPtrFlushArray<MoveDoc> movedocs;
	NewPtrFlushArray<SellDoc> selldocs;
	NewPtrFlushArray<OutDoc> outdocs;
	NewPtrFlushArray<BuyInvoice> buyinvoices;
	NewPtrFlushArray<SellInvoice> sellinvoices;
	uint32_t seasonId = m_pSeason->GetId();

	try
	{
		LoggingSimpleProgress hentsProgress(L"loading hents", m_log);
		dao.LoadHents(hents, seasonId, &hentsProgress);

		LoggingSimpleProgress cowsProgress(L"loading cows", m_log);
		dao.LoadCows(cows, seasonId, &cowsProgress);

		LoggingComplexProgress indocsProgress(L"loading indocs", m_log);
		dao.LoadInDocs(indocs, seasonId, &indocsProgress);

		LoggingComplexProgress buydocsProgress(L"loading buydocs", m_log);
		dao.LoadBuyDocs(buydocs, seasonId,&buydocsProgress);

		LoggingComplexProgress movedocsProgress(L"loading movedocs", m_log);
		dao.LoadMoveDocs(movedocs, seasonId, &movedocsProgress);

		LoggingComplexProgress selldocsProgress(L"loading selldocs", m_log);
		dao.LoadSellDocs(selldocs, seasonId, &selldocsProgress);

		LoggingComplexProgress outdocsProgress(L"loading outdocs", m_log);
		dao.LoadOutDocs(outdocs, seasonId, &outdocsProgress);

		LoggingComplexProgress buyinvoicesProgress(L"loading buyinvoices", m_log);
		dao.LoadBuyInvoices(buyinvoices, seasonId, &buyinvoicesProgress);

		LoggingComplexProgress sellinvoicesProgress(L"loading sellinvoices", m_log);
		dao.LoadSellInvoices(sellinvoices, seasonId, &sellinvoicesProgress);
	}
	catch (wexception e)
	{
		m_log->e(e.message());
		boost::rethrow_exception(boost::current_exception());
	}

	SeasonSession* pSeasonSession = new SeasonSession();
	pSeasonSession->m_pSeason = m_pSeason;
	pSeasonSession->m_pFarmSession = m_pFarmSession;

	m_log->i(L"creating specatator");
	pSeasonSession->m_pSpectator = m_pFarmSession->GetAudience()->ObtainSpectator(m_pSeason->GetId());
	pSeasonSession->m_pChangePublisher = m_pFarmSession->GetChangePublisherFactory()->CreatePublisher(pSeasonSession);
	pSeasonSession->m_worker = boost::thread(boost::bind(&boost::asio::io_service::run, &pSeasonSession->m_service));
	pSeasonSession->m_pClientInterface = new ClientSeasonSessionInterfaceImpl(pSeasonSession);
	pSeasonSession->m_tokenGenerator = m_pTokenGenerator;
	pSeasonSession->m_logger = new BkLogger(TextUtils::Format(L"%s_season_%s", m_pFarmSession->GetFarm()->GetFarmNo().ToString(), paths_RemoveInvalidCharacters(m_pSeason->GetSeasonName())));

	m_log->i(L"creating herd registers");
	PtrEnumerator<IHerd> herdEnum = m_pFarmSession->EnumHerds();
	IHerd* pHerd;
	while(herdEnum.hasNext())
	{
		pHerd = *herdEnum;
		pSeasonSession->m_Registers[pHerd->GetId()] = (new HerdRegistry(pHerd->GetId()));
		herdEnum.advance();
	}

	m_log->i(L"indexing hents");
	NewPtrFlushArray<Hent>::iterator hentIt = hents.begin();
	while(hentIt != hents.end())
	{
		pSeasonSession->m_Hents.insert(new HentServerObject(*hentIt));
		hentIt++;
	}

	m_log->i(L"indexing cows");
	Cow* pCow;
	NewPtrFlushArray<Cow>::iterator cowIt = cows.begin();
	while(cowIt != cows.end())
	{
		pCow = *cowIt;
		pSeasonSession->m_Cows.add(new CowServerObject(pCow));
		pSeasonSession->m_Tracker.StartTrail(pCow);
		pSeasonSession->m_CowNoReference.AddReference(pCow->GetCowNo(),pCow->GetId());
		cowIt++;
	}

	
	m_log->i(L"indexing indocs");
	CowServerObject* pCowSO;
	HerdRegistry* pRegister;
	NewPtrFlushArray<InDoc>::iterator indocIt = indocs.begin();
	InDoc* pInDoc;
	while(indocIt != indocs.end())
	{
		pInDoc = *indocIt;
		pSeasonSession->m_InDocs.add(new InDocServerObject(pInDoc));
		pRegister = pSeasonSession->m_Registers[pInDoc->GetHerdId()];
		pRegister->AddInDoc(pInDoc);

		PtrEnumerator<ICowEntry> entryEnum = pInDoc->EnumCows();
		ICowEntry* pEntry;
		uint32_t buyInvoiceId;
		DocKey key(DocType_In,pInDoc->GetId());
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			pCowSO = pSeasonSession->m_Cows.find(pEntry->GetCowId());
			pCow = pCowSO->Object();
			
			buyInvoiceId = pCow->GetBuyInvoiceId();
			if(buyInvoiceId != NULL_ID)
			{				
				if(!pSeasonSession->m_BuyInvoiceRelations.AreInRelationship(buyInvoiceId,key))
				{
					pSeasonSession->m_BuyInvoiceRelations.AddRelationship(buyInvoiceId,key);
				}
			}			

			CowTrail* pTrail = pSeasonSession->m_Tracker.GetTrail(pEntry->GetCowId());
			pTrail->SetEntryFootprint(pInDoc,pEntry);
			pRegister->AddEnteringEvent(pCowSO->Object(),pInDoc,pEntry);
			entryEnum.advance();
		}
		indocIt++;
	}
	

	m_log->i(L"indexing buydocs");
	NewPtrFlushArray<BuyDoc>::iterator buydocIt = buydocs.begin();
	BuyDoc* pBuyDoc;
	while(buydocIt != buydocs.end())
	{
		pBuyDoc = *buydocIt;
		pSeasonSession->m_BuyDocs.add(new BuyDocServerObject(pBuyDoc));
		pRegister = pSeasonSession->m_Registers[pBuyDoc->GetHerdId()];
		pRegister->AddBuyDoc(pBuyDoc);

		if(pBuyDoc->GetInvoiceId() != NULL_ID)
		{
			pSeasonSession->m_BuyInvoiceRelations.AddRelationship(pBuyDoc->GetInvoiceId(),DocKey(DocType_Buy,pBuyDoc->GetId()));
		}
		
		PtrEnumerator<ICowEntry> entryEnum = pBuyDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			pCowSO = pSeasonSession->m_Cows.find(pEntry->GetCowId());
			CowTrail* pTrail = pSeasonSession->m_Tracker.GetTrail(pEntry->GetCowId());
			pTrail->SetEntryFootprint(pBuyDoc,pEntry);
			pRegister->AddEnteringEvent(pCowSO->Object(), pBuyDoc, pEntry);
			entryEnum.advance();
		}
		buydocIt++;
	}
	
	m_log->i(L"indexing movedocs");
	NewPtrFlushArray<MoveDoc>::iterator movedocIt = movedocs.begin();
	MoveDoc* pMoveDoc;
	HerdRegistry* pFromRegister;
	HerdRegistry* pToRegister;
	while(movedocIt != movedocs.end())
	{
		pMoveDoc = *movedocIt;
		pSeasonSession->m_MoveDocs.add(new MoveDocServerObject(pMoveDoc));
		pFromRegister = pSeasonSession->m_Registers[pMoveDoc->GetSrcHerdId()];
		pToRegister = pSeasonSession->m_Registers[pMoveDoc->GetDstHerdId()];
		
		pFromRegister->AddMoveDoc(pMoveDoc);
		pToRegister->AddMoveDoc(pMoveDoc);


		PtrEnumerator<ICowEntry> entryEnum = pMoveDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			CowTrail* pTrail = pSeasonSession->m_Tracker.GetTrail(pEntry->GetCowId());
			pTrail->AddFootprint(pMoveDoc,pEntry);

			pCowSO = pSeasonSession->m_Cows.find(pEntry->GetCowId());
			pCow = pCowSO->Object();
			pFromRegister->AddLeavingEvent(pCow,pMoveDoc,pEntry);
			pToRegister->AddEnteringEvent(pCow,pMoveDoc,pEntry);
			entryEnum.advance();
		}
		movedocIt++;
	}
	

	m_log->i(L"indexing selldocs");
	NewPtrFlushArray<SellDoc>::iterator selldocIt = selldocs.begin();
	SellDoc* pSellDoc;
	while(selldocIt != selldocs.end())
	{
		pSellDoc = *selldocIt;
		pSeasonSession->m_SellDocs.add(new SellDocServerObject(pSellDoc));
		pRegister = pSeasonSession->m_Registers[pSellDoc->GetHerdId()];
		pRegister->AddSellDoc(pSellDoc);

		if(pSellDoc->GetInvoiceId() != NULL_ID)
		{
			pSeasonSession->m_SellInvoiceRelations.AddRelationship(pSellDoc->GetInvoiceId(),DocKey(DocType_Sell,pSellDoc->GetId()));
		}

		PtrEnumerator<ICowEntry> entryEnum = pSellDoc->EnumCows();
		ICowEntry* pEntry;
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			pCowSO = pSeasonSession->m_Cows.find(pEntry->GetCowId());
			
			CowTrail* pTrail = pSeasonSession->m_Tracker.GetTrail(pEntry->GetCowId());
			pTrail->SetExitFootprint(pSellDoc,pEntry);
			pRegister->AddLeavingEvent(pCowSO->Object(),pSellDoc,pEntry);
			entryEnum.advance();
		}

		selldocIt++;
	}
	

	m_log->i(L"indexing outdocs");
	NewPtrFlushArray<OutDoc>::iterator outdocIt = outdocs.begin();
	OutDoc* pOutDoc;
	while(outdocIt != outdocs.end())
	{
		pOutDoc = *outdocIt;
		pSeasonSession->m_OutDocs.add(new OutDocServerObject(*outdocIt));
		pRegister = pSeasonSession->m_Registers[pOutDoc->GetHerdId()];
		pRegister->AddOutDoc(pOutDoc);

		PtrEnumerator<ICowEntry> entryEnum = pOutDoc->EnumCows();
		ICowEntry* pEntry;
		uint32_t sellInvoiceId;
		DocKey key(DocType_Out,pOutDoc->GetId());
		while(entryEnum.hasNext())
		{
			pEntry = *entryEnum;
			pCowSO = pSeasonSession->m_Cows.find(pEntry->GetCowId());
			pCow = pCowSO->Object();
			sellInvoiceId = pCow->GetSellInvoiceId();
			if(sellInvoiceId != NULL_ID)
			{				
				if(!pSeasonSession->m_SellInvoiceRelations.AreInRelationship(sellInvoiceId,key))
				{
					pSeasonSession->m_SellInvoiceRelations.AddRelationship(sellInvoiceId,key);
				}
			}

			CowTrail* pTrail = pSeasonSession->m_Tracker.GetTrail(pEntry->GetCowId());
			pTrail->SetExitFootprint(pOutDoc,pEntry);
			pRegister->AddLeavingEvent(pCowSO->Object(),pOutDoc,pEntry);
			entryEnum.advance();
		}

		outdocIt++;
	}


	m_log->i(L"indexing buyinvoices");
	NewPtrFlushArray<BuyInvoice>::iterator buyInvoiceIt = buyinvoices.begin();
	while(buyInvoiceIt != buyinvoices.end())
	{
		pSeasonSession->m_BuyInvoices.add(new BuyInvoiceServerObject(*buyInvoiceIt));
		buyInvoiceIt++;
	}

	m_log->i(L"indexing sellinvoices");
	NewPtrFlushArray<SellInvoice>::iterator sellInvoiceIt = sellinvoices.begin();
	while(sellInvoiceIt != sellinvoices.end())
	{
		pSeasonSession->m_SellInvoices.add(new SellInvoiceServerObject(*sellInvoiceIt));
		sellInvoiceIt++;
	}
	

	m_log->i(L"season session created");
	return pSeasonSession;
}
	

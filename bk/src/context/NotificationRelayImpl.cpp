#include "stdafx.h"
#include "NotificationRelayImpl.h"
#include <data\utils\GetDocHerd.h>
#include <logic\changedetect\DocChangeDetector.h>
#include <logic\changedetect\InvoiceChangeDetector.h>
#include <boost\foreach.hpp>
#include "SeasonSessionImpl.h"

NotificationRelayImpl::NotificationRelayImpl(SeasonSessionImpl* pOwningSession) : m_pOwningSession(pOwningSession)
{
}

void NotificationRelayImpl::HentLock(uint32_t hentId)
{
	HentClientObject* pHentCO = m_pOwningSession->GetHent(hentId);
	pHentCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnHentLocked,hentId);									
}

void NotificationRelayImpl::HentUnlock(uint32_t hentId)
{
	
	HentClientObject* pHentCO = m_pOwningSession->GetHent(hentId);
	pHentCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnHentUnlocked,hentId);										
		
}

void NotificationRelayImpl::HentInserted(IHent* pHent)
{
	
	Hent* pNewHent = new Hent(pHent->GetId());
	pNewHent->CopyFrom(pHent);
	HentClientObject* pNewHentCO = new HentClientObject(pNewHent);
	m_pOwningSession->m_Hents.insert(pNewHentCO);															
	m_pOwningSession->VisitObservers(&SeasonObserver::OnHentInserted,pHent->GetId());															
		
}

void NotificationRelayImpl::HentUpdated(IHent* pHent)
{
	HentClientObject* pHentCO = m_pOwningSession->GetHent(pHent->GetId());
	pHentCO->Object()->CopyFrom(pHent);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnHentUpdated,pHent->GetId());						
}

void NotificationRelayImpl::HentDeleted(uint32_t hentId)
{
	m_pOwningSession->VisitObservers(&SeasonObserver::OnHentDeleted,hentId);							
}

void NotificationRelayImpl::BeginFetch()
{
	
}

void NotificationRelayImpl::EndFetch()
{
	
}

void NotificationRelayImpl::VisitTrail(uint32_t cowId,SeasonSessionImpl* pSession,std::function<void (HerdObserver*,CowPresenceKey&)>& visitor)
{
	ICowTrail* pTrail = pSession->GetCowTrail(cowId);
	IDoc* pEntryDoc = pTrail->GetEntry()->GetDoc(); 								
	uint32_t entryHerdId = GetDocHerd(pEntryDoc);
									
	pSession->VisitHerdObservers(entryHerdId,[pEntryDoc,cowId,visitor](HerdObserver* pObserver)
											{
												visitor(pObserver,CowPresenceKey(cowId,pEntryDoc->GetDocType(),pEntryDoc->GetId()));
											});
								
	PtrEnumerator<ICowMovement> movesEnum = pTrail->GetTrail();
	while(movesEnum.hasNext())
	{
		ICowMovement* pMove = *movesEnum;
		IMoveDoc* pMoveDoc = pMove->GetDoc();

		pSession->VisitHerdObservers(pMoveDoc->GetSrcHerdId(),[pMoveDoc,cowId,visitor](HerdObserver* pObserver)
											{
												visitor(pObserver,CowPresenceKey(cowId,pMoveDoc->GetDocType(),pMoveDoc->GetId()));
											});
								
		pSession->VisitHerdObservers(pMoveDoc->GetDstHerdId(),[pMoveDoc,cowId,visitor](HerdObserver* pObserver)
											{
												visitor(pObserver,CowPresenceKey(cowId,pMoveDoc->GetDocType(),pMoveDoc->GetId()));
											});
								
		movesEnum.advance();
	}

	const ICowFootprint* pExitFootprint = pTrail->GetExit();
	if(pExitFootprint)
	{
		IDoc* pExitDoc = pExitFootprint->GetDoc();
		pSession->VisitHerdObservers(GetDocHerd(pExitDoc),[pExitDoc,cowId,visitor](HerdObserver* pObserver)
														{
															visitor(pObserver,(CowPresenceKey(cowId,pExitDoc->GetDocType(),pExitDoc->GetId())));
														});
	}

}

void NotificationRelayImpl::CowLock(uint32_t cowId)
{
	
	CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
	pCowCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnCowLocked,cowId);
	std::function<void (HerdObserver*,CowPresenceKey&)> visitor = std::mem_fn(&HerdObserver::OnHerdCowLocked);
	VisitTrail(cowId,m_pOwningSession,visitor);
							
		
}

void NotificationRelayImpl::CowUnlock(uint32_t cowId)
{
	
	CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
	pCowCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnCowUnlocked,cowId);								

	std::function<void (HerdObserver*,CowPresenceKey&)> visitor = std::mem_fn(&HerdObserver::OnHerdCowUnlocked);
	VisitTrail(cowId,m_pOwningSession,visitor);
						
}

void NotificationRelayImpl::InDocLock(uint32_t docId)
{
	
	InDocClientObject* pDocCO = m_pOwningSession->GetInDoc(docId);
	pDocCO->SetLocked(true);
	InDoc* pInDoc = pDocCO->Object();
	m_pOwningSession->VisitObservers(&SeasonObserver::OnInDocLocked,docId);														

	m_pOwningSession->VisitHerdObservers(pInDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdInDocLocked(docId);
													});
							
}

void NotificationRelayImpl::InDocUnlock(uint32_t docId)
{
	
	InDocClientObject* pDocCO = m_pOwningSession->GetInDoc(docId);
	InDoc* pInDoc = pDocCO->Object();

	pDocCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnInDocUnlocked,docId);														

	m_pOwningSession->VisitHerdObservers(pInDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdInDocUnlocked(docId);
													});
							
}

void NotificationRelayImpl::CowInserted(ICow* pCow)
{
	
	Cow* pInsertedCow = new Cow(pCow->GetId());
	pInsertedCow->CopyFrom(pCow);
	CowClientObject* pCowCO = new CowClientObject(pInsertedCow);		
	m_pOwningSession->m_Cows.insert(pCowCO);
	m_pOwningSession->m_pTracker->StartTrail(pInsertedCow);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnCowInserted,pInsertedCow->GetId());														
							
}

void NotificationRelayImpl::CowUpdated(ICow* pCow)
{
	
	uint32_t cowId = pCow->GetId();
	CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
	Cow* pClientSideCow =  pCowCO->Object();

	pClientSideCow->CopyFrom(pCow);								
	m_pOwningSession->VisitObservers(&SeasonObserver::OnCowUpdated,cowId);		

	std::function<void (HerdObserver*,CowPresenceKey&)> visitor = std::mem_fn(&HerdObserver::OnHerdCowUpdated);
	VisitTrail(cowId,m_pOwningSession,visitor);
								
							
}

void NotificationRelayImpl::CowDeleted(uint32_t cowId)
{
	CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
	Cow* pClientSideCow =  pCowCO->Object();																
	m_pOwningSession->VisitObservers(&SeasonObserver::OnCowDeleted,cowId);								
	m_pOwningSession->m_Cows.erase(cowId);								
	delete pCowCO;											
}

void NotificationRelayImpl::InDocInserted(IInDoc* pDoc)
{
	InDoc* pInsertedInDoc = new InDoc(pDoc->GetId());
	pInsertedInDoc->CopyFrom(pDoc);
	m_pOwningSession->m_InDocs.insert(new InDocClientObject(pInsertedInDoc));
	m_pOwningSession->FileAnInDoc(pInsertedInDoc);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnInDocInserted,pInsertedInDoc->GetId());														

	m_pOwningSession->VisitHerdObservers(pInsertedInDoc->GetHerdId(),[pInsertedInDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdInDocInserted(pInsertedInDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedInDoc->EnumCows();
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	pObserver->OnHerdCowAdded(CowPresenceKey(pEntry->GetCowId(),pInsertedInDoc->GetDocType(),pInsertedInDoc->GetId()));
																	entryEnum.advance();
																}
															});
						
}


void NotificationRelayImpl::InDocUpdated(IInDoc* pDoc)
{
		InDocClientObject* pInDocCliSideObj = m_pOwningSession->GetInDoc(pDoc->GetId());
		InDoc* pClientSideInDoc = pInDocCliSideObj->Object();

		DocDetails details;
		details.CopyFrom(pDoc);
		pClientSideInDoc->SetDetails(details);

		DocChangeDetector detector(pClientSideInDoc,pDoc);
		DocChanges changes = detector.Detect();

		HerdRegistry* pRegister = m_pOwningSession->m_Registers.at(pClientSideInDoc->GetHerdId());

		BOOST_FOREACH(DocChange* change,changes)
		{
			ICowEntry* pNewEntry = change->New();
			ICowEntry* pOldEntry = change->Old();

			switch(change->Event())
			{
				case ChangeEvent_Added:
					{					
						uint32_t cowId = pNewEntry->GetCowId();
						CowEntry* entry = pClientSideInDoc->AddCow(cowId);
						entry->CopyFrom(pNewEntry);
						CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
						pRegister->AddEnteringEvent(pCowCO->Object(),pClientSideInDoc,entry);	
						CowTrail* pTrail = m_pOwningSession->FindCowTrail(cowId);
						pTrail->SetEntryFootprint(pClientSideInDoc,entry);		

						m_pOwningSession->VisitHerdObservers(pClientSideInDoc->GetHerdId(),[pClientSideInDoc,cowId](HerdObserver* pObserver)
																{
																	pObserver->OnHerdCowAdded(CowPresenceKey(cowId,pClientSideInDoc->GetDocType(),pClientSideInDoc->GetId()));																							
																});

						break;
					}											
				case ChangeEvent_Updated:
					{
						CowEntry* entry = pClientSideInDoc->GetEntry(pNewEntry->GetCowId());
						entry->CopyFrom(pNewEntry);
						break;
					}
				case ChangeEvent_Deleted:
					{
						uint32_t cowId = pOldEntry->GetCowId();
												
						m_pOwningSession->VisitHerdObservers(pClientSideInDoc->GetHerdId(),[pClientSideInDoc,cowId](HerdObserver* pObserver)
																{
																	pObserver->OnHerdCowDeleted(CowPresenceKey(cowId,pClientSideInDoc->GetDocType(),pClientSideInDoc->GetId()));																							
																});

						m_pOwningSession->m_pTracker->DeleteTrail(cowId);
						CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
						pRegister->RemoveEnteringEvent(pCowCO->Object(),pClientSideInDoc);
						pClientSideInDoc->RemoveEntry(cowId);

						break;
					}
			}
		}

		m_pOwningSession->VisitObservers(&SeasonObserver::OnInDocUpdated,pClientSideInDoc->GetId());														

		m_pOwningSession->VisitHerdObservers(pClientSideInDoc->GetHerdId(),[pClientSideInDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdInDocUpdated(pClientSideInDoc->GetId());																						
															});
							
}
void NotificationRelayImpl::InDocDeleted(uint32_t docId)
{
	InDocClientObject* pDocCO = m_pOwningSession->GetInDoc(docId);
	InDoc* pDoc = pDocCO->Object();
								
	uint32_t herdId = pDoc->GetHerdId();									
	HerdRegistry* pRegistry = m_pOwningSession->GetHerdRegistry(herdId);
	std::vector<ICow*> cows(m_pOwningSession->GetDocCows(pDoc));
								
	std::for_each(cows.begin(),cows.end(),[pRegistry,pDoc](ICow* cow)
	{		
		pRegistry->RemoveEnteringEvent(cow,pDoc);
	});

	std::for_each(cows.begin(),cows.end(),[this](ICow* cow)
	{
		m_pOwningSession->m_pTracker->DeleteTrail(cow->GetId());
	});

	pRegistry->RemoveInDoc(docId);
	m_pOwningSession->m_InDocs.erase(docId);
																		
	m_pOwningSession->VisitObservers(&SeasonObserver::OnInDocDeleted,docId);														

	m_pOwningSession->VisitHerdObservers(herdId,[pDoc,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdInDocDeleted(pDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;																											
														pObserver->OnHerdCowDeleted(CowPresenceKey(pEntry->GetCowId(),pDoc->GetDocType(),pDoc->GetId()));
														cowsEnum.advance();
													}																				
												});

	delete pDocCO;
	
}

void NotificationRelayImpl::BuyDocLock(uint32_t docId)
{
	
		BuyDocClientObject* pDocCO = m_pOwningSession->GetBuyDoc(docId);
		BuyDoc* pDoc = pDocCO->Object();
		pDocCO->SetLocked(true);
		m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyDocLocked,docId);														

		m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
														{
															pObserver->OnHerdBuyDocLocked(docId);
														});
							
}

void NotificationRelayImpl::BuyDocUnlock(uint32_t docId)
{
	
		BuyDocClientObject* pDocCO = m_pOwningSession->GetBuyDoc(docId);
		BuyDoc* pDoc = pDocCO->Object();

		pDocCO->SetLocked(false);
		m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyDocUnlocked,docId);														

		m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
														{
															pObserver->OnHerdBuyDocUnlocked(docId);
														});
							
}

void NotificationRelayImpl::BuyDocInserted(IBuyDoc* pDoc)
{
	BuyDoc* pInsertedDoc = new BuyDoc(pDoc->GetId());
	pInsertedDoc->CopyFrom(pDoc);
	m_pOwningSession->m_BuyDocs.insert(new BuyDocClientObject(pInsertedDoc));
	m_pOwningSession->FileABuyDoc(pInsertedDoc);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyDocInserted,pInsertedDoc->GetId());
													

	m_pOwningSession->VisitHerdObservers(pInsertedDoc->GetHerdId(),[pInsertedDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdBuyDocInserted(pInsertedDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedDoc->EnumCows();
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	pObserver->OnHerdCowAdded(CowPresenceKey(pEntry->GetCowId(),pInsertedDoc->GetDocType(),pInsertedDoc->GetId()));
																	entryEnum.advance();
																}
															});
						

}
void NotificationRelayImpl::BuyDocUpdated(IBuyDoc* pDoc)
{
	BuyDocClientObject* pBuyDocCliSideObj = m_pOwningSession->GetBuyDoc(pDoc->GetId());
	BuyDoc* pClientSideBuyDoc = pBuyDocCliSideObj->Object();

	DocDetails details;
	details.CopyFrom(pDoc);
	pClientSideBuyDoc->SetDetails(details);
	pClientSideBuyDoc->SetHentId(pDoc->GetHentId());
	pClientSideBuyDoc->SetInvoiceId(pDoc->GetInvoiceId());

	DocChangeDetector detector(pClientSideBuyDoc,pDoc);
	DocChanges changes = detector.Detect();

	HerdRegistry* pRegister = m_pOwningSession->m_Registers.at(pClientSideBuyDoc->GetHerdId());

	BOOST_FOREACH(DocChange* change,changes)
	{
		ICowEntry* pNewEntry = change->New();
		ICowEntry* pOldEntry = change->Old();

		switch(change->Event())
		{
			case ChangeEvent_Added:
				{					
					uint32_t cowId = pNewEntry->GetCowId();
					CowEntry* entry = pClientSideBuyDoc->AddCow(cowId);
					entry->CopyFrom(pNewEntry);
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
					pRegister->AddEnteringEvent(pCowCO->Object(),pClientSideBuyDoc,entry);	
					CowTrail* pTrail = m_pOwningSession->FindCowTrail(cowId);
					pTrail->SetEntryFootprint(pClientSideBuyDoc,entry);		

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowAdded(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					break;
				}											
			case ChangeEvent_Updated:
				{
					CowEntry* entry = pClientSideBuyDoc->GetEntry(pNewEntry->GetCowId());
					entry->CopyFrom(pNewEntry);
					break;
				}
			case ChangeEvent_Deleted:
				{
					uint32_t cowId = pOldEntry->GetCowId();
												
					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowDeleted(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					m_pOwningSession->m_pTracker->DeleteTrail(cowId);
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);
					pRegister->RemoveEnteringEvent(pCowCO->Object(),pClientSideBuyDoc);
					pClientSideBuyDoc->RemoveEntry(cowId);
					break;
				}
		}
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyDocUpdated,pClientSideBuyDoc->GetId());														

	m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc](HerdObserver* pObserver)
														{
															pObserver->OnHerdBuyDocUpdated(pClientSideBuyDoc->GetId());																						
														});

}
void NotificationRelayImpl::BuyDocDeleted(uint32_t docId)
{
	BuyDocClientObject* pDocCO = m_pOwningSession->GetBuyDoc(docId);
	BuyDoc* pDoc = pDocCO->Object();
								
	uint32_t herdId = pDoc->GetHerdId();									
	HerdRegistry* pRegistry = m_pOwningSession->GetHerdRegistry(herdId);
	std::vector<ICow*> cows(m_pOwningSession->GetDocCows(pDoc));
								
	std::for_each(cows.begin(),cows.end(),[pRegistry,pDoc](ICow* cow)
	{		
		pRegistry->RemoveEnteringEvent(cow,pDoc);
	});

	std::for_each(cows.begin(),cows.end(),[this](ICow* cow)
	{
		m_pOwningSession->m_pTracker->DeleteTrail(cow->GetId());
	});

	pRegistry->RemoveBuyDoc(docId);
	m_pOwningSession->m_BuyDocs.erase(docId);
																		
	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyDocDeleted,docId);														

	m_pOwningSession->VisitHerdObservers(herdId,[pDoc,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdBuyDocDeleted(pDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;																											
														pObserver->OnHerdCowDeleted(CowPresenceKey(pEntry->GetCowId(),pDoc->GetDocType(),pDoc->GetId()));
														cowsEnum.advance();
													}																				
												});

	delete pDocCO;
}

void NotificationRelayImpl::MoveDocLock(uint32_t docId)
{
	
	MoveDocClientObject* pDocCO = m_pOwningSession->GetMoveDoc(docId);
	MoveDoc* pDoc = pDocCO->Object();
	pDocCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnMoveDocLocked,docId);														

	m_pOwningSession->VisitHerdObservers(pDoc->GetSrcHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdMoveDocLocked(docId);
													});
	m_pOwningSession->VisitHerdObservers(pDoc->GetDstHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdMoveDocLocked(docId);
													});
							
}
void NotificationRelayImpl::MoveDocUnlock(uint32_t docId)
{
	
	MoveDocClientObject* pDocCO = m_pOwningSession->GetMoveDoc(docId);
	MoveDoc* pDoc = pDocCO->Object();

	pDocCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnMoveDocUnlocked,docId);														

	m_pOwningSession->VisitHerdObservers(pDoc->GetSrcHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdMoveDocUnlocked(docId);
													});

	m_pOwningSession->VisitHerdObservers(pDoc->GetDstHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdMoveDocUnlocked(docId);
													});
						
}
void NotificationRelayImpl::MoveDocInserted(IMoveDoc* pDoc)
{
	
	MoveDoc* pInsertedMoveDoc = new MoveDoc(pDoc->GetId());
	pInsertedMoveDoc->CopyFrom(pDoc);
	m_pOwningSession->m_MoveDocs.insert(new MoveDocClientObject(pInsertedMoveDoc));
	m_pOwningSession->FileAMoveDoc(pInsertedMoveDoc);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnMoveDocInserted,pInsertedMoveDoc->GetId());												

	m_pOwningSession->VisitHerdObservers(pInsertedMoveDoc->GetSrcHerdId(),[this,pInsertedMoveDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdMoveDocInserted(pInsertedMoveDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedMoveDoc->EnumCows();
																uint32_t cowId;
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	cowId = pEntry->GetCowId();
																	CowPath path = m_pOwningSession->m_pTracker->GetTrail(cowId)->GetPath();
																	IHerdFootprint* pThisStep = path.GetLastStep();
																	IHerdFootprint* pBeforeThisStep = pThisStep->PrevStep();
																	IDoc* pBeforeThisDoc = pBeforeThisStep->GetStepDoc();
																	pObserver->OnHerdCowMoved(
																		CowPresenceKey(cowId,pBeforeThisDoc->GetDocType(),pBeforeThisDoc->GetId()),
																		CowPresenceKey(cowId,pInsertedMoveDoc->GetDocType(),pInsertedMoveDoc->GetId()));
																	entryEnum.advance();
																}
															});

	m_pOwningSession->VisitHerdObservers(pInsertedMoveDoc->GetDstHerdId(),[pInsertedMoveDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdMoveDocInserted(pInsertedMoveDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedMoveDoc->EnumCows();
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	pObserver->OnHerdCowAdded(CowPresenceKey(pEntry->GetCowId(),pInsertedMoveDoc->GetDocType(),pInsertedMoveDoc->GetId()));
																	entryEnum.advance();
																}
															});
							

}

void NotificationRelayImpl::MoveDocUpdated(IMoveDoc* pDoc)
{
	MoveDocClientObject* pMoveDocCliSideObj = m_pOwningSession->GetMoveDoc(pDoc->GetId());
	MoveDoc* pClientSideBuyDoc = pMoveDocCliSideObj->Object();
		
	pClientSideBuyDoc->SetDetails(pDoc);

	DocChangeDetector detector(pClientSideBuyDoc,pDoc);
	DocChanges changes = detector.Detect();

	HerdRegistry* pSrcRegister = m_pOwningSession->m_Registers.at(pClientSideBuyDoc->GetSrcHerdId());
	HerdRegistry* pDstRegister = m_pOwningSession->m_Registers.at(pClientSideBuyDoc->GetDstHerdId());

	BOOST_FOREACH(DocChange* change,changes)
	{
		ICowEntry* pNewEntry = change->New();
		ICowEntry* pOldEntry = change->Old();

		switch(change->Event())
		{
			case ChangeEvent_Added:
				{					
					uint32_t cowId = pNewEntry->GetCowId();
					CowEntry* entry = pClientSideBuyDoc->AddCow(cowId);
					entry->CopyFrom(pNewEntry);
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					pSrcRegister->AddLeavingEvent(pCowCO->Object(),pClientSideBuyDoc,entry);	
					pDstRegister->AddEnteringEvent(pCowCO->Object(),pClientSideBuyDoc,entry);	

					CowTrail* pTrail = m_pOwningSession->FindCowTrail(cowId);

					CowPath path = pTrail->GetPath();
					IHerdFootprint* pLastStep = path.GetLastStep();
					pTrail->AddFootprint(pClientSideBuyDoc,entry);		

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetSrcHerdId(),[pClientSideBuyDoc,cowId,pLastStep](HerdObserver* pObserver)
															{
																IDoc* pLastStepDoc = pLastStep->GetStepDoc();
																pObserver->OnHerdCowMoved(CowPresenceKey(cowId,pLastStepDoc->GetDocType(),pLastStepDoc->GetId()),
																						  CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetDstHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowAdded(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					break;
				}											
			case ChangeEvent_Updated:
				{
					CowEntry* entry = pClientSideBuyDoc->GetEntry(pNewEntry->GetCowId());
					uint32_t cowId = entry->GetCowId();
					entry->CopyFrom(pNewEntry);


					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetSrcHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowUpdated(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetDstHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowUpdated(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});

					break;
				}
			case ChangeEvent_Deleted:
				{
					uint32_t cowId = pOldEntry->GetCowId();				
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cowId);
					pTrail->EraseLastMovement();

					CowPath path = pTrail->GetPath();
					IDoc* pLastDoc = path.GetLastStep()->GetStepDoc();
					
					pSrcRegister->RemoveLeavingEvent(pCowCO->Object(),pClientSideBuyDoc);
					pDstRegister->RemoveEnteringEvent(pCowCO->Object(),pClientSideBuyDoc);

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetSrcHerdId(),[pClientSideBuyDoc,cowId,pLastDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()),
																						    CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));																							
															});

					
					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetDstHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowDeleted(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});


					pClientSideBuyDoc->RemoveEntry(cowId);
					break;
				}
		}
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnMoveDocUpdated,pClientSideBuyDoc->GetId());														

	m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetSrcHerdId(),[pClientSideBuyDoc](HerdObserver* pObserver)
														{
															pObserver->OnHerdMoveDocUpdated(pClientSideBuyDoc->GetId());																						
														});

	m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetDstHerdId(),[pClientSideBuyDoc](HerdObserver* pObserver)
														{
															pObserver->OnHerdMoveDocUpdated(pClientSideBuyDoc->GetId());																						
														});



}

void NotificationRelayImpl::MoveDocDeleted(uint32_t docId)
{
	
	MoveDocClientObject* pDocCO = m_pOwningSession->GetMoveDoc(docId);
	MoveDoc* pDoc = pDocCO->Object();
								
	uint32_t srcHerdId = pDoc->GetSrcHerdId();
	uint32_t dstHerdId = pDoc->GetDstHerdId();
								
	HerdRegistry* pSrcRegistry = m_pOwningSession->GetHerdRegistry(srcHerdId);
	HerdRegistry* pDstRegistry = m_pOwningSession->GetHerdRegistry(dstHerdId);

	std::vector<ICow*> cows(m_pOwningSession->GetDocCows(pDoc));
								
	std::for_each(cows.begin(),cows.end(),[pSrcRegistry,pDstRegistry,pDoc](ICow* cow)
	{
		pSrcRegistry->RemoveLeavingEvent(cow,pDoc);
		pDstRegistry->RemoveEnteringEvent(cow,pDoc);
	});

	std::for_each(cows.begin(),cows.end(),[this](ICow* cow)
	{
		CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cow->GetId());
		pTrail->EraseLastMovement();
	});

	m_pOwningSession->m_MoveDocs.erase(docId);

	pSrcRegistry->RemoveMoveDoc(docId);
	pDstRegistry->RemoveMoveDoc(docId);
																
	m_pOwningSession->VisitObservers(&SeasonObserver::OnMoveDocDeleted,docId);														

	m_pOwningSession->VisitHerdObservers(dstHerdId,[pDoc,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdMoveDocDeleted(pDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;																											
														pObserver->OnHerdCowDeleted(CowPresenceKey(pEntry->GetCowId(),pDoc->GetDocType(),pDoc->GetId()));
														cowsEnum.advance();
													}																				
												});

	m_pOwningSession->VisitHerdObservers(srcHerdId,[pDoc,&cows,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdMoveDocDeleted(pDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;
																				
														uint32_t cowId = pEntry->GetCowId();
														ICowTrail* pTrail = m_pOwningSession->GetCowTrail(cowId);
														CowPath path = pTrail->GetPath();
														IHerdFootprint* pLastStep = path.GetLastStep();
														IDoc* pLastDoc = pLastStep->GetStepDoc();
														pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pDoc->GetDocType(),pDoc->GetId()),
																					CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));

														cowsEnum.advance();
													}																				
												});
	delete pDocCO;
							
}

void NotificationRelayImpl::SellDocLock(uint32_t docId)
{
	
	SellDocClientObject* pDocCO = m_pOwningSession->GetSellDoc(docId);
	SellDoc* pDoc = pDocCO->Object();
	pDocCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellDocLocked,docId);													

	m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdSellDocLocked(docId);
													});
							
}
void NotificationRelayImpl::SellDocUnlock(uint32_t docId)
{
	SellDocClientObject* pDocCO = m_pOwningSession->GetSellDoc(docId);
	SellDoc* pDoc = pDocCO->Object();
	pDocCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellDocUnlocked,docId);														

	m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdSellDocUnlocked(docId);
													});
							
}
void NotificationRelayImpl::SellDocInserted(ISellDoc* pDoc)
{
	
	SellDoc* pInsertedSellDoc = new SellDoc(pDoc->GetId());
	pInsertedSellDoc->CopyFrom(pDoc);
	m_pOwningSession->m_SellDocs.insert(new SellDocClientObject(pInsertedSellDoc));
	m_pOwningSession->FileASellDoc(pInsertedSellDoc);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellDocInserted,pInsertedSellDoc->GetId());														

	m_pOwningSession->VisitHerdObservers(pInsertedSellDoc->GetHerdId(),[pInsertedSellDoc,this](HerdObserver* pObserver)
															{
																pObserver->OnHerdSellDocInserted(pInsertedSellDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedSellDoc->EnumCows();
																uint32_t cowId;
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	cowId = pEntry->GetCowId();
																	CowPath path = m_pOwningSession->m_pTracker->GetTrail(cowId)->GetPath();
																	IHerdFootprint* pThisStep = path.GetLastStep();
																	IHerdFootprint* pBeforeThisStep = pThisStep->PrevStep();
																	IDoc* pBeforeThisDoc = pBeforeThisStep->GetStepDoc();																								
																	pObserver->OnHerdCowRemoved(CowPresenceKey(cowId,pBeforeThisDoc->GetDocType(),pBeforeThisDoc->GetId()));
																	entryEnum.advance();
																}
															});
							

}
void NotificationRelayImpl::SellDocUpdated(ISellDoc* pDoc)
{
	SellDocClientObject* pSellDocCliSideObj = m_pOwningSession->GetSellDoc(pDoc->GetId());
	SellDoc* pClientSideBuyDoc = pSellDocCliSideObj->Object();
		
	pClientSideBuyDoc->SetDetails(pDoc);
	pClientSideBuyDoc->SetHentId(pDoc->GetHentId());
	pClientSideBuyDoc->SetInvoiceId(pDoc->GetInvoiceId());


	DocChangeDetector detector(pClientSideBuyDoc,pDoc);
	DocChanges changes = detector.Detect();

	HerdRegistry* pRegister = m_pOwningSession->m_Registers.at(pClientSideBuyDoc->GetHerdId());
	
	BOOST_FOREACH(DocChange* change,changes)
	{
		ICowEntry* pNewEntry = change->New();
		ICowEntry* pOldEntry = change->Old();

		switch(change->Event())
		{
			case ChangeEvent_Added:
				{					
					uint32_t cowId = pNewEntry->GetCowId();
					CowEntry* entry = pClientSideBuyDoc->AddCow(cowId);
					entry->CopyFrom(pNewEntry);
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					pRegister->AddLeavingEvent(pCowCO->Object(),pClientSideBuyDoc,entry);	
					
					CowTrail* pTrail = m_pOwningSession->FindCowTrail(cowId);

					CowPath path = pTrail->GetPath();
					IHerdFootprint* pLastStep = path.GetLastStep();
					pTrail->SetExitFootprint(pClientSideBuyDoc,entry);		

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId,pLastStep](HerdObserver* pObserver)
															{
																IDoc* pLastStepDoc = pLastStep->GetStepDoc();
																pObserver->OnHerdCowRemoved(CowPresenceKey(cowId,pLastStepDoc->GetDocType(),pLastStepDoc->GetId()));																							
															});					

					break;
				}											
			case ChangeEvent_Updated:
				{
					CowEntry* entry = pClientSideBuyDoc->GetEntry(pNewEntry->GetCowId());
					uint32_t cowId = entry->GetCowId();
					entry->CopyFrom(pNewEntry);


					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowUpdated(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});
									

					break;
				}
			case ChangeEvent_Deleted:
				{
					uint32_t cowId = pOldEntry->GetCowId();				
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cowId);
					pTrail->EraseExitFootprint();

					CowPath path = pTrail->GetPath();
					IDoc* pLastDoc = path.GetLastStep()->GetStepDoc();
					
					pRegister->RemoveLeavingEvent(pCowCO->Object(),pClientSideBuyDoc);
					
					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId,pLastDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()),
																						    CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));																							
															});

					pClientSideBuyDoc->RemoveEntry(cowId);
					break;
				}
		}
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellDocUpdated,pClientSideBuyDoc->GetId());															

	m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc](HerdObserver* pObserver)
														{
															pObserver->OnHerdSellDocUpdated(pClientSideBuyDoc->GetId());																						
														});

}


void NotificationRelayImpl::SellDocDeleted(uint32_t docId)
{
	
	SellDocClientObject* pDocCO = m_pOwningSession->GetSellDoc(docId);
	SellDoc* pSellDoc = pDocCO->Object();
	uint32_t herdId = pSellDoc->GetHerdId();
								
	HerdRegistry* pRegistry = m_pOwningSession->GetHerdRegistry(herdId);

	std::vector<ICow*> cows(m_pOwningSession->GetDocCows(pSellDoc));
								
	std::for_each(cows.begin(),cows.end(),[pRegistry,pSellDoc](ICow* cow)
	{
		pRegistry->RemoveLeavingEvent(cow,pSellDoc);
	});

	std::for_each(cows.begin(),cows.end(),[this](ICow* cow)
	{
		CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cow->GetId());
		pTrail->EraseExitFootprint();
	});

	m_pOwningSession->m_SellDocs.erase(docId);
	pRegistry->RemoveSellDoc(docId);
																
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellDocDeleted,docId);														

	m_pOwningSession->VisitHerdObservers(herdId,[pSellDoc,&cows,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdSellDocDeleted(pSellDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pSellDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;
																				
														uint32_t cowId = pEntry->GetCowId();
														ICowTrail* pTrail = m_pOwningSession->GetCowTrail(cowId);
														CowPath path = pTrail->GetPath();
														IHerdFootprint* pLastStep = path.GetLastStep();
														IDoc* pLastDoc = pLastStep->GetStepDoc();
														pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pSellDoc->GetDocType(),pSellDoc->GetId()),
																					CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));

														cowsEnum.advance();
													}																				
												});
	delete pDocCO;
							
}

void NotificationRelayImpl::OutDocLock(uint32_t docId)
{
	OutDocClientObject* pDocCO = m_pOwningSession->GetOutDoc(docId);
	OutDoc* pDoc = pDocCO->Object();
	pDocCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnOutDocLocked,docId);													

	m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdOutDocLocked(docId);
													});

}
void NotificationRelayImpl::OutDocUnlock(uint32_t docId)
{
	OutDocClientObject* pDocCO = m_pOwningSession->GetOutDoc(docId);
	OutDoc* pDoc = pDocCO->Object();
	pDocCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnOutDocUnlocked,docId);														

	m_pOwningSession->VisitHerdObservers(pDoc->GetHerdId(),[docId](HerdObserver* pObserver)
													{
														pObserver->OnHerdOutDocUnlocked(docId);
													});
							

}
void NotificationRelayImpl::OutDocInserted(IOutDoc* pDoc)
{
	OutDoc* pInsertedDoc = new OutDoc(pDoc->GetId());
	pInsertedDoc->CopyFrom(pDoc);
	m_pOwningSession->m_OutDocs.insert(new OutDocClientObject(pInsertedDoc));
	m_pOwningSession->FileAnOutDoc(pInsertedDoc);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnOutDocInserted,pInsertedDoc->GetId());														

	m_pOwningSession->VisitHerdObservers(pInsertedDoc->GetHerdId(),[pInsertedDoc,this](HerdObserver* pObserver)
															{
																pObserver->OnHerdOutDocInserted(pInsertedDoc->GetId());
																ICowEntry* pEntry;
																PtrEnumerator<ICowEntry> entryEnum = pInsertedDoc->EnumCows();
																uint32_t cowId;
																while(entryEnum.hasNext())
																{
																	pEntry = *entryEnum;
																	cowId = pEntry->GetCowId();
																	CowPath path = m_pOwningSession->m_pTracker->GetTrail(cowId)->GetPath();
																	IHerdFootprint* pThisStep = path.GetLastStep();
																	IHerdFootprint* pBeforeThisStep = pThisStep->PrevStep();
																	IDoc* pBeforeThisDoc = pBeforeThisStep->GetStepDoc();																								
																	pObserver->OnHerdCowRemoved(CowPresenceKey(cowId,pBeforeThisDoc->GetDocType(),pBeforeThisDoc->GetId()));
																	entryEnum.advance();
																}
															});
							

}
void NotificationRelayImpl::OutDocUpdated(IOutDoc* pDoc)
{
	OutDocClientObject* pSellDocCliSideObj = m_pOwningSession->GetOutDoc(pDoc->GetId());
	OutDoc* pClientSideBuyDoc = pSellDocCliSideObj->Object();
		
	pClientSideBuyDoc->SetDetails(pDoc);

	DocChangeDetector detector(pClientSideBuyDoc,pDoc);
	DocChanges changes = detector.Detect();

	HerdRegistry* pRegister = m_pOwningSession->m_Registers.at(pClientSideBuyDoc->GetHerdId());
	
	BOOST_FOREACH(DocChange* change,changes)
	{
		ICowEntry* pNewEntry = change->New();
		ICowEntry* pOldEntry = change->Old();

		switch(change->Event())
		{
			case ChangeEvent_Added:
				{					
					uint32_t cowId = pNewEntry->GetCowId();
					CowEntry* entry = pClientSideBuyDoc->AddCow(cowId);
					entry->CopyFrom(pNewEntry);
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					pRegister->AddLeavingEvent(pCowCO->Object(),pClientSideBuyDoc,entry);	
					
					CowTrail* pTrail = m_pOwningSession->FindCowTrail(cowId);

					CowPath path = pTrail->GetPath();
					IHerdFootprint* pLastStep = path.GetLastStep();
					pTrail->SetExitFootprint(pClientSideBuyDoc,entry);		

					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId,pLastStep](HerdObserver* pObserver)
															{
																IDoc* pLastStepDoc = pLastStep->GetStepDoc();
																pObserver->OnHerdCowRemoved(CowPresenceKey(cowId,pLastStepDoc->GetDocType(),pLastStepDoc->GetId()));																							
															});					

					break;
				}											
			case ChangeEvent_Updated:
				{
					CowEntry* entry = pClientSideBuyDoc->GetEntry(pNewEntry->GetCowId());
					uint32_t cowId = entry->GetCowId();
					entry->CopyFrom(pNewEntry);


					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowUpdated(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()));																							
															});
									

					break;
				}
			case ChangeEvent_Deleted:
				{
					uint32_t cowId = pOldEntry->GetCowId();				
					CowClientObject* pCowCO = m_pOwningSession->GetCow(cowId);

					CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cowId);
					pTrail->EraseExitFootprint();

					CowPath path = pTrail->GetPath();
					IDoc* pLastDoc = path.GetLastStep()->GetStepDoc();
					
					pRegister->RemoveLeavingEvent(pCowCO->Object(),pClientSideBuyDoc);
					
					m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc,cowId,pLastDoc](HerdObserver* pObserver)
															{
																pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pClientSideBuyDoc->GetDocType(),pClientSideBuyDoc->GetId()),
																						    CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));																							
															});

					pClientSideBuyDoc->RemoveEntry(cowId);
					break;
				}
		}
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnOutDocUpdated,pClientSideBuyDoc->GetId());															

	m_pOwningSession->VisitHerdObservers(pClientSideBuyDoc->GetHerdId(),[pClientSideBuyDoc](HerdObserver* pObserver)
														{
															pObserver->OnHerdOutDocUpdated(pClientSideBuyDoc->GetId());																						
														});


}
void NotificationRelayImpl::OutDocDeleted(uint32_t docId)
{
	OutDocClientObject* pDocCO = m_pOwningSession->GetOutDoc(docId);
	OutDoc* pDoc = pDocCO->Object();
	uint32_t herdId = pDoc->GetHerdId();
								
	HerdRegistry* pRegistry = m_pOwningSession->GetHerdRegistry(herdId);

	std::vector<ICow*> cows(m_pOwningSession->GetDocCows(pDoc));
								
	std::for_each(cows.begin(),cows.end(),[pRegistry,pDoc](ICow* cow)
	{
		pRegistry->RemoveLeavingEvent(cow,pDoc);
	});

	std::for_each(cows.begin(),cows.end(),[this](ICow* cow)
	{
		CowTrail* pTrail = m_pOwningSession->m_pTracker->GetTrail(cow->GetId());
		pTrail->EraseExitFootprint();
	});

	m_pOwningSession->m_OutDocs.erase(docId);
	pRegistry->RemoveOutDoc(docId);
																
	m_pOwningSession->VisitObservers(&SeasonObserver::OnOutDocDeleted,docId);														

	m_pOwningSession->VisitHerdObservers(herdId,[pDoc,&cows,this](HerdObserver* pObserver)
												{
													pObserver->OnHerdOutDocDeleted(pDoc->GetId());

													ICowEntry* pEntry;
													PtrEnumerator<ICowEntry> cowsEnum = pDoc->EnumCows();
													while(cowsEnum.hasNext())
													{
														pEntry = *cowsEnum;
																				
														uint32_t cowId = pEntry->GetCowId();
														ICowTrail* pTrail = m_pOwningSession->GetCowTrail(cowId);
														CowPath path = pTrail->GetPath();
														IHerdFootprint* pLastStep = path.GetLastStep();
														IDoc* pLastDoc = pLastStep->GetStepDoc();
														pObserver->OnHerdCowRevoked(CowPresenceKey(cowId,pDoc->GetDocType(),pDoc->GetId()),
																					CowPresenceKey(cowId,pLastDoc->GetDocType(),pLastDoc->GetId()));

														cowsEnum.advance();
													}																				
												});
	delete pDocCO;
	
}


void NotificationRelayImpl::BuyInvoiceLock(uint32_t invoiceId)
{
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pOwningSession->GetBuyInvoice(invoiceId);
	pBuyInvoiceCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyInvoiceLocked,invoiceId);									
}

void NotificationRelayImpl::BuyInvoiceInserted(IBuyInvoice* pInvoice)
{
	BuyInvoice* pNewInvoice = new BuyInvoice(pInvoice->GetId(),pInvoice->GetInvoiceScope());
	pNewInvoice->CopyFrom(pInvoice);

	BuyInvoiceClientObject* pBuyInvoiceCO = new BuyInvoiceClientObject(pNewInvoice);
	m_pOwningSession->m_BuyInvoices.insert(pBuyInvoiceCO);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyInvoiceInserted,pInvoice->GetId());									
}

void NotificationRelayImpl::BuyInvoiceUpdated(IBuyInvoice* pUpdatedInvoice)
{
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pOwningSession->GetBuyInvoice(pUpdatedInvoice->GetId());
	BuyInvoice* pClientSideInvoice = pBuyInvoiceCO->Object();

	pClientSideInvoice->SetFrom(pUpdatedInvoice);

	InvoiceChangeDetector detector(pClientSideInvoice,pUpdatedInvoice);
	InvoiceChanges changes = detector.Detect();
	BOOST_FOREACH(InvoiceChange* pChange,changes)
	{
		ICowInvoiceEntry* pNewEntry = pChange->New();
		ICowInvoiceEntry* pOldEntry = pChange->Old();

		switch(pChange->Event())
		{
		case ChangeEvent_Added:
			pClientSideInvoice->AddEntry(pNewEntry->GetCowId())->CopyFrom(pNewEntry);
			break;
		case ChangeEvent_Deleted:
			pClientSideInvoice->RemoveEntry(pOldEntry->GetCowId());
			break;
		case ChangeEvent_Updated:
			pClientSideInvoice->GetEntry(pNewEntry->GetCowId())->CopyFrom(pNewEntry);
			break;
		}
	}


	pClientSideInvoice->RemoveAllDeductions();
	PtrEnumerator<IInvoiceDeduction> deductionEnum = pUpdatedInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pClientSideInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyInvoiceUpdated,pUpdatedInvoice->GetId());
}

void NotificationRelayImpl::BuyInvoiceDeleted(uint32_t invoiceId)
{
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pOwningSession->GetBuyInvoice(invoiceId);
	m_pOwningSession->m_BuyInvoices.erase(invoiceId);
	delete pBuyInvoiceCO;
	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyInvoiceDeleted,invoiceId);									
}

void NotificationRelayImpl::BuyInvoiceUnlock(uint32_t invoiceId)
{
	BuyInvoiceClientObject* pBuyInvoiceCO = m_pOwningSession->GetBuyInvoice(invoiceId);
	pBuyInvoiceCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnBuyInvoiceUnlocked,invoiceId);									
}

void NotificationRelayImpl::SellInvoiceLock(uint32_t invoiceId)
{
	SellInvoiceClientObject* pSellInvoiceCO = m_pOwningSession->GetSellInvoice(invoiceId);
	pSellInvoiceCO->SetLocked(true);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellInvoiceLocked,invoiceId);	
}

void NotificationRelayImpl::SellInvoiceInserted(ISellInvoice* pInvoice)
{
	SellInvoice* pNewInvoice = new SellInvoice(pInvoice->GetId(),pInvoice->GetInvoiceScope());
	pNewInvoice->CopyFrom(pInvoice);

	SellInvoiceClientObject* pSellInvoiceCO = new SellInvoiceClientObject(pNewInvoice);
	m_pOwningSession->m_SellInvoices.insert(pSellInvoiceCO);

	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellInvoiceInserted,pInvoice->GetId());									
}

void NotificationRelayImpl::SellInvoiceUpdated(ISellInvoice* pUpdatedInvoice)
{
	SellInvoiceClientObject* pSellInvoiceCO = m_pOwningSession->GetSellInvoice(pUpdatedInvoice->GetId());
	SellInvoice* pClientSideInvoice = pSellInvoiceCO->Object();

	pClientSideInvoice->SetFrom(pUpdatedInvoice);

	InvoiceChangeDetector detector(pClientSideInvoice,pUpdatedInvoice);
	InvoiceChanges changes = detector.Detect();
	BOOST_FOREACH(InvoiceChange* pChange,changes)
	{
		ICowInvoiceEntry* pNewEntry = pChange->New();
		ICowInvoiceEntry* pOldEntry = pChange->Old();

		switch(pChange->Event())
		{
		case ChangeEvent_Added:
			pClientSideInvoice->AddEntry(pNewEntry->GetCowId())->CopyFrom(pNewEntry);
			break;
		case ChangeEvent_Deleted:
			pClientSideInvoice->RemoveEntry(pOldEntry->GetCowId());
			break;
		case ChangeEvent_Updated:
			pClientSideInvoice->GetEntry(pNewEntry->GetCowId())->CopyFrom(pNewEntry);
			break;
		}
	}
	
	pClientSideInvoice->RemoveAllDeductions();
	PtrEnumerator<IInvoiceDeduction> deductionEnum = pUpdatedInvoice->EnumDeductions();
	while (deductionEnum.hasNext())
	{
		pClientSideInvoice->AddDeduction(*deductionEnum);
		deductionEnum.advance();
	}

	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellInvoiceUpdated,pUpdatedInvoice->GetId());
}

void NotificationRelayImpl::SellInvoiceDeleted(uint32_t invoiceId)
{
	SellInvoiceClientObject* pSellInvoiceCO = m_pOwningSession->GetSellInvoice(invoiceId);
	m_pOwningSession->m_SellInvoices.erase(invoiceId);
	delete pSellInvoiceCO;
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellInvoiceDeleted,invoiceId);									
}

void NotificationRelayImpl::SellInvoiceUnlock(uint32_t invoiceId)
{
	SellInvoiceClientObject* pSellInvoiceCO = m_pOwningSession->GetSellInvoice(invoiceId);
	pSellInvoiceCO->SetLocked(false);
	m_pOwningSession->VisitObservers(&SeasonObserver::OnSellInvoiceUnlocked,invoiceId);									
}


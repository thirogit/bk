#include "stdafx.h"
#include "FarmHentViewCreator.h"

FarmHentViewCreator::FarmHentViewCreator(SeasonSession* facade) : m_pFacade(facade)
{
}

void FarmHentViewCreator::RefreshView(HentView* pView,uint32_t herdId)
{

	IFarm* farm = m_pFacade->GetFarm();
	IHerd* herd = m_pFacade->GetHerd(herdId);

	pView->SetHentType(HentType_Company);	

	HentNo hentNo = HentNoFactory::Create(farm->GetFarmNo(),HerdNoFactory::Create(herd->GetHerdIndex()));

	pView->SetHentNo(hentNo);
	pView->SetAlias(L"");

	pView->SetExtras(L"");

	pView->SetName(farm->GetName());
	pView->SetAlias(farm->GetAlias());
	pView->SetFiscalNo(farm->GetNIP());
	pView->SetPhoneNo(farm->GetPhoneNo());
	pView->SetPOBox(farm->GetPOBox());
	pView->SetStreet(farm->GetStreet());
	pView->SetWetNo(farm->GetWetIdNo());
	pView->SetZip(farm->GetZipCode());
	pView->SetREGON(farm->GetREGON());
	pView->SetCity(farm->GetCity());


	pView->SetPlateNo(L"");			
	pView->SetPESEL(L"");
	pView->SetIdNo(L"");
	pView->SetIssueDate(DateTime());
	pView->SetIssuePost(L"");

	pView->SetSyncIt(false);

	pView->SetWetLicenceNo(farm->GetWetLicNo());
	pView->SetCellPhoneNo(L"");
	pView->SetEmailAddress(L"");

	pView->SetLatitude(Latitude());
	pView->SetLongitude(Longitude());
	pView->SetLocked(false);

}

HentView* FarmHentViewCreator::CreateView(uint32_t herdId)
{
	HentView* pView = new HentView(herdId);			
	RefreshView(pView,herdId);
	return pView;			
}

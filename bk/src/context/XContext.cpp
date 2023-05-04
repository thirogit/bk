#include "stdafx.h"
#include "XContext.h"

#include "ContextException.h"
#include <boost/iterator/transform_iterator.hpp>
#include <string\TextUtils.h>
#include <arrays/DeepDelete.h>
#include <boost/range/any_range.hpp>
#include <boost/foreach.hpp>
#include <boost/range/adaptors.hpp>

XContext::XContext() : m_pClient(NULL), m_User(NULL)
{
}

PtrEnumerator<ISeason> XContext::EnumSeasons() const
{
	return PtrEnumerator<ISeason>(m_Seasons);
}

PtrEnumerator<IHerd> XContext::EnumHerds() const
{	
	return PtrEnumerator<IHerd>(m_Herds);
}


size_t XContext::GetHerdCount() const
{
	return m_Herds.size();
}

ISeason*  XContext::GetSeason(uint32_t seasonId)
{
	const boost::multi_index::index<SeasonIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_Seasons);

	auto seasonIt = indexById.find(seasonId);
	if (seasonIt != indexById.end())
		return *seasonIt;

	return NULL;
}

SeasonSession* XContext::OpenSeason(uint32_t seasonId)
{
	ISeason* season = GetSeason(seasonId);
	if (season == NULL)
		BOOST_THROW_EXCEPTION(ContextException(TextUtils::FormatA(L"season with id=%d does not exist", seasonId)));

	std::map<uint32_t,SeasonSessionImpl*>::iterator sessionIt = m_Sessions.find(seasonId);
	if(sessionIt != m_Sessions.end())
		BOOST_THROW_EXCEPTION(ContextException(TextUtils::FormatA(L"season with id=%d already open", seasonId)));
	try
	{
		OpenSeasonResponseObj response = m_pClient->OpenSeason(seasonId);
		if(response->GetResult() == Open_Successful)
		{
			SeasonSessionImpl* session = new SeasonSessionImpl();
		
			session->m_sessionToken = response->GetSessionToken();
			session->m_season = season;
			session->m_pContext = this;			
			session->m_pTracker = new CowTracker();
			session->m_observerEndPoint = RCF::TcpEndpoint(m_ServerEndpoint.getIp(),response->GetObserverPort());
			session->m_pNtfnSubsriber = new NotificationSubsriber();
			session->m_pRelay = new NotificationRelayImpl(session);			
			m_Sessions[seasonId] = session;
			return session;		
		}
	}
	catch(RCF::Exception& e)
	{
		BOOST_THROW_EXCEPTION(ContextException(TextUtils::FromUTF8(e.getWhat())));
	}

	return NULL;
}

IHerd* XContext::GetHerd(uint32_t herdId)
{

	const boost::multi_index::index<HerdsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_Herds);

	auto herdIt = indexById.find(herdId);
	if (herdIt != indexById.end())
		return *herdIt;

	return NULL;
}

IHerd* XContext::GetHerdByIndex(int herdIndex)
{
	BOOST_FOREACH(Herd* herd, m_Herds)
	{
		if (herd->GetHerdIndex() == herdIndex)
		{
			return herd;
		}
	}
	return NULL;
}

void XContext::CloseSession(SeasonSessionImpl* pSession)
{
	try
	{
		//if there are locked records, CloseSeason will cause unlock and subsequest unlock broadcast, 
		//since we are not interested in any notification (we're closing!) first lets unsubscribe from receiving notifications ...
		pSession->m_pNtfnSubsriber->Unsubscribe();
		//...and then close
		m_pClient->CloseSeason(pSession->m_sessionToken);
	}
	catch (...)
	{
		//ignore while closing
	}

	
	delete pSession->m_pNtfnSubsriber;
	delete pSession->m_pRelay;
	pSession->RemoveAllObservers();
	delete pSession->m_pTracker;

	DeepDelete(pSession->m_OutDocs);
	DeepDelete(pSession->m_Hents);	
	DeepDelete(pSession->m_Cows);
	DeepDelete(pSession->m_InDocs);
	DeepDelete(pSession->m_BuyDocs);
	DeepDelete(pSession->m_MoveDocs);
	DeepDelete(pSession->m_SellDocs);
	DeepDelete(pSession->m_Registers);
	DeepDelete(pSession->m_BuyInvoices);
	DeepDelete(pSession->m_SellInvoices);
	

	m_Sessions.erase(pSession->GetSeason()->GetId());
	delete pSession;
	
}

IFarm* XContext::GetFarm() const
{
	return m_pFarm;
}

PrintProfiles XContext::GetPrintProfiles(const std::wstring& sDocumentCode)
{
	PrintProfiles profiles;

	BOOST_FOREACH(PrintProfile* profile, m_printProfiles)
	{
		if (profile->GetDocumentCode() == sDocumentCode)
			profiles.AddProfile(profile);
	}

	return std::move(profiles);
}

IPrintProfile* XContext::GetPrintProfile(uint32_t profileId)
{
	const boost::multi_index::index<PrintProfileIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_printProfiles);

	auto profileIt = indexById.find(profileId);
	if (profileIt != indexById.end())
		return *profileIt;

	return NULL;
}

uint32_t XContext::SaveNewPrintProfile(const IPrintProfile* pProfile)
{
	PrintProfileRequest* request = new PrintProfileRequest();
	request->CreateProfile(NULL_ID)->CopyFrom(pProfile);

	uint32_t newProfileId = InvokeClientR<PrintProfileRequestObj,uint32_t>(&BKServiceClient::NewPrintProfile, PrintProfileRequestObj(request));

	PrintProfile* newPrintProfile = new PrintProfile(newProfileId);
	newPrintProfile->CopyFrom(pProfile);
	m_printProfiles.insert(newPrintProfile);

	return newProfileId;
}

void XContext::UpdatePrintProfile(const IPrintProfile* pProfile)
{
	PrintProfileRequest* request = new PrintProfileRequest();
	request->CreateProfile(pProfile->GetProfileId())->CopyFrom(pProfile);

	InvokeClient(&BKServiceClient::UpdatePrintProfile,PrintProfileRequestObj(request));

	boost::multi_index::index<PrintProfileIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_printProfiles);
	auto profileIt = indexById.find(pProfile->GetProfileId());

	PrintProfile* profileToUpdate = *profileIt;
	profileToUpdate->SetProfileName(pProfile->GetProfileName());
	profileToUpdate->CopyOptions(pProfile);
	profileToUpdate->CopyOrderSpecifiers(pProfile);
}

void XContext::DeletePrintProfile(uint32_t profileId)
{
	InvokeClient(&BKServiceClient::DeletePrintProfile, profileId);
	
	boost::multi_index::index<PrintProfileIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_printProfiles);
	auto profileIt = indexById.find(profileId);
	delete *profileIt;
	indexById.erase(profileIt);
}

PassportRecognitionProfiles XContext::GetPassportRecognitionProfiles()
{
	PassportRecognitionProfiles profiles;

	BOOST_FOREACH(PassportRecognitionProfile* profile, m_passRecgntnProfiles)
	{
		profiles.AddProfile(profile);
	}

	return std::move(profiles);
}

IPassportRecognitionProfile* XContext::GetPassportRecognitionProfile(uint32_t profileId)
{
	boost::multi_index::index<PassportRecognitionProfilesIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_passRecgntnProfiles);
	auto profileIt = indexById.find(profileId);

	if (indexById.end() != profileIt)
		return *profileIt;

	return NULL;	
}

uint32_t XContext::SaveNewPassportRecognitionProfile(const IPassportRecognitionProfile* pProfile)
{
	PassportRecognitionProfileRequest* request = new PassportRecognitionProfileRequest();
	request->CreateProfile(NULL_ID)->CopyFrom(pProfile);

	uint32_t newProfileId = m_pClient->NewPassportRecognitionProfile(PassportRecognitionProfileRequestObj(request));

	PassportRecognitionProfile* newProfile = new PassportRecognitionProfile(newProfileId);
	newProfile->CopyFrom(pProfile);
	m_passRecgntnProfiles.insert(newProfile);

	return newProfileId;
}

void XContext::UpdatePassportRecognitionProfile(const IPassportRecognitionProfile* pProfile)
{
	PassportRecognitionProfileRequest* request = new PassportRecognitionProfileRequest();
	request->CreateProfile(pProfile->GetId())->CopyFrom(pProfile);
	m_pClient->UpdatePassportRecognitionProfile(PassportRecognitionProfileRequestObj(request));

	boost::multi_index::index<PassportRecognitionProfilesIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_passRecgntnProfiles);
	auto profileIt = indexById.find(pProfile->GetId());

	PassportRecognitionProfile* profileToUpdate = *profileIt;	
	profileToUpdate->CopyFrom(pProfile);
}

void XContext::DeletePassportRecognitionProfile(uint32_t profileId)
{
	InvokeClient(&BKServiceClient::DeletePassportRecognitionProfile, profileId);
	
	boost::multi_index::index<PassportRecognitionProfilesIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_passRecgntnProfiles);
	auto profileIt = indexById.find(profileId);
	delete *profileIt;
	indexById.erase(profileIt);
}

IAgent* XContext::GetAgent(uint32_t agentId)
{
	const boost::multi_index::index<AgentsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_Agents);

	auto it = indexById.find(agentId);
	if (it != indexById.end())
		return *it;

	return NULL;
}


PtrEnumerator<IAgent> XContext::EnumAgents()
{
	return PtrEnumerator<IAgent>(m_Agents);
}

PtrEnumerator<IBankAccount> XContext::EnumBankAccounts()
{
	return PtrEnumerator<IBankAccount>(m_Accounts);
}

IBankAccount* XContext::GetBankAccount(uint32_t accountId)
{
	const boost::multi_index::index<BankAccountsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_Accounts);

	auto it = indexById.find(accountId);
	if (it != indexById.end())
		return *it;

	return NULL;
}

DeductionDefinitions XContext::GetDeductionDefinitions()
{
	DeductionDefinitions deductions;

	BOOST_FOREACH(DeductionDefinition* definition,m_deductionDefinitions)
	{
		deductions.Add(definition);
	}	

	return std::move(deductions);
}

IDeductionDefinition* XContext::GetDeductionDefinition(uint32_t definitionId)
{
	boost::multi_index::index<DeductionDefinitionsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_deductionDefinitions);
	auto definitionIt = indexById.find(definitionId);

	if (indexById.end() != definitionIt)
		return *definitionIt;

	return NULL;
}

uint32_t XContext::SaveNewDeductionDefinition(const IDeductionDefinition* definition)
{
	DeductionDefinitionRequest* request = new DeductionDefinitionRequest();
	request->CreateDefinition(definition->GetId())->CopyFrom(definition);

	uint32_t newDefinitionId = InvokeClientR<DeductionDefinitionRequestObj,uint32_t>(&BKServiceClient::NewDeductionDefinition, DeductionDefinitionRequestObj(request));

	DeductionDefinition* newDefinition = new DeductionDefinition(newDefinitionId);
	newDefinition->CopyFrom(definition);
	m_deductionDefinitions.insert(newDefinition);
	return newDefinitionId;
}

void XContext::UpdateDeductionDefinition(const IDeductionDefinition* definition)
{
	DeductionDefinitionRequest* request = new DeductionDefinitionRequest();
	request->CreateDefinition(definition->GetId())->CopyFrom(definition);

	InvokeClient(&BKServiceClient::UpdateDeductionDefinition, DeductionDefinitionRequestObj(request));

	boost::multi_index::index<DeductionDefinitionsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_deductionDefinitions);
	auto definitionIt = indexById.find(definition->GetId());

	DeductionDefinition* definitionToUpdate = *definitionIt;
	definitionToUpdate->CopyFrom(definition);
}

void XContext::DeleteDeductionDefinition(uint32_t definitionId)
{
	InvokeClient(&BKServiceClient::DeleteDeductionDefinition, definitionId);

	boost::multi_index::index<DeductionDefinitionsIndex, index_tag_id>::type& indexById = boost::multi_index::get<index_tag_id>(m_deductionDefinitions);
	auto definitionIt = indexById.find(definitionId);
	delete *definitionIt;
	indexById.erase(definitionIt);
}

IClass* XContext::GetClass(uint32_t classId)
{
	return m_Classes.find(classId);
}

IClass* XContext::GetClassByCd(const std::wstring& classCd)
{
	BOOST_FOREACH(Class* cowClass, m_Classes)
	{
		if (cowClass->GetClassCode() == classCd)
		{
			return cowClass;
		}
	}

	return NULL;
}

IStock* XContext::GetStock(uint32_t stockId)
{
	return m_Stocks.find(stockId);
}

PtrEnumerator<IStock> XContext::EnumStocks()
{	
	return PtrEnumerator<IStock>(m_Stocks);
}

PtrEnumerator<IClass> XContext::EnumClasses()
{
	return PtrEnumerator<IClass>(m_Classes);
}

void XContext::SaveSettings()
{
	SettingsObj settings(new Settings());

	BOOST_FOREACH(Setting* setting, m_settings | boost::adaptors::map_values)
	{
		settings->AddSetting(setting);
	}

	InvokeClient(&BKServiceClient::SaveSettings, 0, settings);
}

boost::any XContext::GetSetting(const std::wstring& settingName)
{
	auto it = m_settings.find(settingName);
	if (it != m_settings.end())
		return it->second->GetValue();

	return boost::any();
}

void XContext::SetSetting(const std::wstring& settingName, const boost::any& settingValue)
{
	auto it = m_settings.find(settingName);
	if (it != m_settings.end())
	{
		it->second->SetValue(settingValue);
	}
	else
	{
		m_settings[settingName] = new Setting(settingName, settingValue);
	}
}

UserSettings* XContext::GetUserSettings()
{
	return m_pSettingsFacade;
}


ISimpleUser* XContext::GetUser() const
{
	return m_User;
}
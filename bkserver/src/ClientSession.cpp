#include "stdafx.h"
#include "ClientSession.h"
#include "SessionException.h"
#include <string/TextUtils.h>
#include <string/IntString.h>
#include <string/ToString.h>
#include <boost/foreach.hpp>
#include <filesystem/paths.h>

ClientSession::ClientSession(FarmSession* pOwnerSession,uint32_t userId) : 
	m_pOwnerSession(pOwnerSession),m_userId(userId)
{
	IUser* user = m_pOwnerSession->FindUser(m_userId);
	IFarm* farm = m_pOwnerSession->GetFarm();
	m_logger = new BkLogger(farm->GetFarmNo().ToString() + L"_user_" + paths_RemoveInvalidCharacters(user->GetLogin()));
}

ClientSession::~ClientSession()
{
	delete m_logger;
}

void ClientSession::CloseSession(uint32_t seasonToken)
{
	ClientSeasonSessionsByToken& indexByToken = m_ClientSeasonSessions.get<0>();
	auto sessionIt = indexByToken.find(seasonToken);
	if (sessionIt == indexByToken.end())
	{
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"there is no season session with token=%d under this connection", seasonToken)));
	}

	ClientSeasonSessionFacade session = *sessionIt;
	session.Close();
	m_ClientSeasonSessions.erase(sessionIt);
}

void ClientSession::CloseAllSessions()
{
	BOOST_FOREACH(ClientSeasonSessionFacade session,m_ClientSeasonSessions)
	{
		try
		{
			session.Close();
		}
		catch(...)
		{
			//ignore any exception while closing
		}
	}
	m_ClientSeasonSessions.clear();
}

ClientSeasonSessionFacade ClientSession::OpenSeason(uint32_t seasonId)
{
	auto sessionIt = m_ClientSeasonSessions.find(seasonId);
	if(sessionIt != m_ClientSeasonSessions.end())
	{
		BOOST_THROW_EXCEPTION(SessionException(TextUtils::FormatA(L"season with id=%d has been already openned in this session",seasonId)));
	}
	else
	{
		SeasonSession* pSeasonSession = m_pOwnerSession->AquireSeasonSession(seasonId);	
		if(pSeasonSession)
		{
			ClientSeasonSessionFacade pNewSession = pSeasonSession->Open(this);		
			m_ClientSeasonSessions.insert(pNewSession);
			return pNewSession;
		}
		else
		{
			return ClientSeasonSessionFacade(NULL);
		}		
	}	
}


ClientSeasonSessionFacade ClientSession::FindClientSeasonSession(uint32_t sessionToken)
{
	const boost::multi_index::index<ClientSeasonSessionsIndex,index_tag_token>::type& indexByToken = boost::multi_index::get<index_tag_token>(m_ClientSeasonSessions);
	auto sessionIt = indexByToken.find(sessionToken);
	if(indexByToken.end() != sessionIt)
		return *sessionIt;

	BOOST_THROW_EXCEPTION(SessionException(L"invalid session token"));
	
}

uint32_t ClientSession::GetUserId() const
{
	return m_userId;
}


PrintProfiles ClientSession::FetchPrintProfiles()
{
	PrintProfiles profiles;
	
	UserDoAny([&profiles](UserSessionFacade* facade)
						{
							PtrEnumerator<IPrintProfile> enumProfile = facade->EnumPrintProfiles();
							while (enumProfile.hasNext())
							{
								profiles.AddProfile(*enumProfile);
								enumProfile.advance();
							}

						});

	return std::move(profiles);
}

void ClientSession::UpdatePrintProfile(IPrintProfile* pProfile)
{
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::UpdatePrintProfile), std::placeholders::_1, pProfile));
}

uint32_t ClientSession::NewPrintProfile(IPrintProfile* pProfile)
{
	return UserEvalAny<uint32_t>(std::bind(std::mem_fn(&UserSessionFacade::NewPrintProfile), std::placeholders::_1, pProfile));
}

void ClientSession::DeletePrintProfile(uint32_t profileId)
{
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::DeletePrintProfile), std::placeholders::_1, profileId));
}

PassportRecognitionProfiles ClientSession::FetchPassportRecognitionProfiles()
{
	PassportRecognitionProfiles profiles;

	UserDoAny([&profiles](UserSessionFacade* facade)
	{
		PtrEnumerator<IPassportRecognitionProfile> enumProfile = facade->EnumPassportRecognitionProfiles();
		while (enumProfile.hasNext())
		{
			profiles.AddProfile(*enumProfile);
			enumProfile.advance();
		}

	});
	return std::move(profiles);
}

void ClientSession::UpdatePassportRecognitionProfile(IPassportRecognitionProfile* pProfile)
{
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::UpdatePassportRecognitionProfile), std::placeholders::_1, pProfile));
}

uint32_t ClientSession::NewPassportRecognitionProfile(IPassportRecognitionProfile* pProfile)
{
	return UserEvalAny<uint32_t>(std::bind(std::mem_fn(&UserSessionFacade::NewPassportRecognitionProfile), std::placeholders::_1, pProfile));
}

void ClientSession::DeletePassportRecognitionProfile(uint32_t profileId)
{
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::DeletePassportRecognitionProfile), std::placeholders::_1, profileId));
}


DeductionDefinitions ClientSession::FetchDeductionDefinitions()
{
	DeductionDefinitions definitions;

	UserDoAny([&definitions](UserSessionFacade* facade)
	{
		PtrEnumerator<IDeductionDefinition> enumDeductions = facade->EnumDeductionDefinitions();
		while (enumDeductions.hasNext())
		{
			definitions.Add(*enumDeductions);
			enumDeductions.advance();
		}

	});
	return std::move(definitions);

}

void ClientSession::UpdateDeductionDefinition(IDeductionDefinition* pDefinition)
{
	d(L"updating deduction with id=" + IntString(pDefinition->GetId()) + L": " + ToString(pDefinition));
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::UpdateDeductionDefinition), std::placeholders::_1, pDefinition));
}

uint32_t ClientSession::NewDeductionDefinition(IDeductionDefinition* pDefinition)
{
	d(L"new deduction submitted: " + ToString(pDefinition));
	return UserEvalAny<uint32_t>(std::bind(std::mem_fn(&UserSessionFacade::NewDeductionDefinition), std::placeholders::_1, pDefinition));
}

void ClientSession::DeleteDeductionDefinition(uint32_t definitionId)
{
	d(L"deleting deduction with id=" + IntString(definitionId));
	UserDoAny(std::bind(std::mem_fn(&UserSessionFacade::DeleteDeductionDefinition), std::placeholders::_1, definitionId));
}


void ClientSession::d(const std::wstring& message)
{
	m_logger->d(message);
}

void ClientSession::i(const std::wstring& message)
{
	m_logger->i(message);
}

void ClientSession::e(const std::wstring& message)
{
	m_logger->e(message);
}

UserSettings ClientSession::GetSettings()
{
	UserSettings settings;

	UserDoAny([&settings](UserSessionFacade* facade)
	{
		ISetting* setting;
		PtrEnumerator<ISetting> settingsEnum = facade->EnumSettings();
		while (settingsEnum.hasNext())
		{
			setting = *settingsEnum;
			settings.SetSetting(setting->GetName(), setting->GetValue());
			settingsEnum.advance();
		}
	});

	return settings;
}

void ClientSession::SaveSettings(const UserSettings& settings)
{
	UserDoAny([&settings](UserSessionFacade* facade)
	{
		facade->SaveSettings(settings);
	});
}


repo::RepoClientFactory* ClientSession::GetRepoClientFactory()
{
	return m_pOwnerSession->GetRepoClientFactory();
}
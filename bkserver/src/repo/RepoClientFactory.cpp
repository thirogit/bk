#include "stdafx.h"
#include "RepoClientFactory.h"
#include "string/TextUtils.h"
#include "RepoException.h"

namespace repo 
{

	RepoConnectionSettings& RepoConnectionSettings::Auth0(const Auth0Account& account)
	{
		m_account = account;
		return *this;
	}

	const Auth0Account& RepoConnectionSettings::Auth0() const
	{
		return m_account;
	}

	RepoConnectionSettings& RepoConnectionSettings::Url(const std::string& url)
	{
		m_Url = url;
		return *this;
	}

	const std::string& RepoConnectionSettings::Url() const
	{
		return m_Url;
	}
	
	//#################################################################################################
	
	
	RepoClientFactory::~RepoClientFactory()
	{

	}

	RepoClient* RepoClientFactory::CreateClient(const FarmNo& farmNo)
	{
		Url::Url repoUrl(m_settings.Url());
		repo::RepoClient* client = new repo::RepoClient(repoUrl, m_settings.Auth0(), farmNo);
		
		return client;
	}

	RepoClientFactory::RepoClientFactory(const RepoConnectionSettings& settings) : m_settings(settings)
	{

	}

}	

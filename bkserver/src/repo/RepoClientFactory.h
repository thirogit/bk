#ifndef __REPOCLIENTFACTORY_H__
#define __REPOCLIENTFACTORY_H__

#include "RepoClient.h"
#include "../cloud/Auth0Account.h"

namespace repo 
{
	class RepoConnectionSettings
	{
	public:
		RepoConnectionSettings& Url(const std::string& url);
		const std::string& Url() const;

		RepoConnectionSettings& Auth0(const Auth0Account& account);
		const Auth0Account& Auth0() const;
	private:
		std::string m_Url;
		Auth0Account m_account;		
	};


	class RepoClientFactory
	{
	public:
		RepoClientFactory(const RepoConnectionSettings& settings);
		~RepoClientFactory();
		RepoClient* CreateClient(const FarmNo& farmNo);
	private:
		 RepoConnectionSettings m_settings;
	};

}

#endif
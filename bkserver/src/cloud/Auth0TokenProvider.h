#pragma once

#include "Auth0Account.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "HttpClient.h"

struct Auth0Token
{
	std::string token;
	std::string token_type;
	int expires_in;
};

class Auth0TokenProvider
{	
public:
	Auth0TokenProvider(const Auth0Account& account);
	Auth0Token ObtainToken();
private:
	void RefreshOAuth2TokenIfNeeded();
	Auth0Token RequestOAuth2Token();

private:
	boost::posix_time::ptime m_tokenIssueTime;
	Auth0Account m_account;
	boost::optional<Auth0Token> m_OAuth2Token;
	HttpClient m_httpClient;
};


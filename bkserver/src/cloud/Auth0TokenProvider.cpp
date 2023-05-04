#include "stdafx.h"
#include "Auth0TokenProvider.h"
#include "jwt/jwt_all.h"
#include <boost/format.hpp>

Auth0TokenProvider::Auth0TokenProvider(const Auth0Account& account) : m_account(account)
{
	
}

Auth0Token Auth0TokenProvider::ObtainToken()
{
	RefreshOAuth2TokenIfNeeded();
	return m_OAuth2Token.get();
}

void Auth0TokenProvider::RefreshOAuth2TokenIfNeeded()
{
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	if (!m_OAuth2Token.is_initialized())
	{
		m_OAuth2Token = RequestOAuth2Token();
		m_tokenIssueTime = now;
		return;
	}

	if ((now - m_tokenIssueTime) > boost::posix_time::seconds(m_OAuth2Token->expires_in))
	{
		m_OAuth2Token = RequestOAuth2Token();
		m_tokenIssueTime = now;
		return;
	}
}

Auth0Token Auth0TokenProvider::RequestOAuth2Token()
{

	nlohmann::json tokenRq;

	tokenRq["grant_type"] = m_account.grant_type;
	tokenRq["client_id"] = m_account.client_id;
	tokenRq["client_secret"] = m_account.client_secret;
	tokenRq["audience"] = m_account.audience;
	

	std::map<std::string, std::string> headers = {
		{ "Content-Type", "application/json" }		
	};

	CallResponsePtr response = m_httpClient.MakePostCall(Url::Url(m_account.token_uri), headers, tokenRq.dump());

	if (response->result_int() == 200)
	{
		nlohmann::json jsonResponse = nlohmann::json::parse(response->body().data());
		std::string oauth2Token = jsonResponse["access_token"];
		std::string tokenType = jsonResponse["token_type"];
		int expiresInSeconds = jsonResponse["expires_in"];

		Auth0Token token;
		token.expires_in = expiresInSeconds;
		token.token = oauth2Token;
		token.token_type = tokenType;
		return token;
	}

	BOOST_THROW_EXCEPTION(std::runtime_error((boost::format("faild to obtain oauth token, reason: %d %s") % response->result_int() % response->reason()).str().c_str()));
}



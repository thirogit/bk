#pragma once

#include "GoogleServiceAccount.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "HttpClient.h"

namespace gcp
{
	struct GoogleAuth2Token
	{
		std::string token;
		std::string token_type;
		int expires_in;
	};

	class GoogleAuth2TokenProvider
	{
	public:
		GoogleAuth2TokenProvider(const GoogleServiceAccount& account);
		GoogleAuth2Token ObtainToken();
	private:
		void RefreshOAuth2TokenIfNeeded();
		GoogleAuth2Token RequestOAuth2Token();
		std::string CreateJWT();
	private:
		boost::posix_time::ptime m_tokenIssueTime;
		GoogleServiceAccount m_account;
		boost::optional<GoogleAuth2Token> m_OAuth2Token;
		HttpClient m_httpClient;
	};

}

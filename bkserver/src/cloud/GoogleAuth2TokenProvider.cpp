#include "stdafx.h"
#include "GoogleAuth2TokenProvider.h"
#include "jwt/jwt_all.h"
#include <boost/format.hpp>
#include "jwt/rsavalidator.h"
#include "private/clock.h"
#include <jwt/rsasigner.h>

namespace gcp
{
	GoogleAuth2TokenProvider::GoogleAuth2TokenProvider(const GoogleServiceAccount& account) : m_account(account)
	{

	}

	GoogleAuth2Token GoogleAuth2TokenProvider::ObtainToken()
	{
		RefreshOAuth2TokenIfNeeded();
		return m_OAuth2Token.get();
	}

	void GoogleAuth2TokenProvider::RefreshOAuth2TokenIfNeeded()
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

	GoogleAuth2Token GoogleAuth2TokenProvider::RequestOAuth2Token()
	{
		std::string content = "grant_type=" + Url::escape("urn:ietf:params:oauth:grant-type:jwt-bearer", Url::Url::UNRESERVED, false);
		content += "&";
		content += "assertion=" + Url::escape(CreateJWT(), Url::Url::UNRESERVED, false);

		std::map<std::string, std::string> headers = {
			{ "Content-Type", "application/x-www-form-urlencoded" }
		};

		CallResponsePtr response = m_httpClient.MakePostCall(Url::Url("https://www.googleapis.com/oauth2/v4/token"), headers, content);

		if (response->result_int() == 200)
		{
			nlohmann::json jsonResponse = nlohmann::json::parse(response->body().data());
			GoogleAuth2Token token;
			token.token = jsonResponse["access_token"];
			token.token_type = jsonResponse["token_type"];
			token.expires_in = jsonResponse["expires_in"];

			return token;
		}

		BOOST_THROW_EXCEPTION(std::runtime_error((boost::format("faild to obtain oauth token, reason: %d %s") % response->result_int() % response->reason()).str().c_str()));
	}


	std::string GoogleAuth2TokenProvider::CreateJWT()
	{
		UtcClock clock;
		uint64_t now = clock.Now();

		nlohmann::json claim;

		claim["iss"] = m_account.client_email();
		claim["scope"] = "https://www.googleapis.com/auth/firebase.messaging https://www.googleapis.com/auth/cloud-platform https://www.googleapis.com/auth/pubsub https://www.googleapis.com/auth/userinfo.email https://www.googleapis.com/auth/firebase.database";
		claim["aud"] = "https://www.googleapis.com/oauth2/v4/token";
		claim["iat"] = now;
		claim["exp"] = now + 59 * 60 + 59;

		RS256Signer signer(m_account.private_key().c_str());
		return JWT::Encode(signer, claim);
	}


}
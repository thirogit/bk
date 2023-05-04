#include "stdafx.h"
#include "FirebaseTopicPublisher.h"
#include "jwt/jwt_all.h"
#include "jwt/rsavalidator.h"
#include "private/clock.h"
#include <openssl/pem.h>
#include <private/ssl_compat.h>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <jwt/rsasigner.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>




FirebaseTopicPublisher::FirebaseTopicPublisher(const gcp::GoogleServiceAccount& account) : m_account(account)
{
	
}

void FirebaseTopicPublisher::RefreshOAuth2TokenIfNeeded()
{
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	if (m_tokenIssueTime == boost::posix_time::not_a_date_time || (now-m_tokenIssueTime) > boost::posix_time::minutes(59))
	{
		m_OAuth2Token = RequestOAuth2Token();
		m_tokenIssueTime = now;
	}
}

std::string FirebaseTopicPublisher::RequestOAuth2Token()
{
	std::string content = "grant_type=" + Url::escape("urn:ietf:params:oauth:grant-type:jwt-bearer",Url::Url::UNRESERVED,false);
	content += "&";
	content += "assertion=" + Url::escape(CreateJWT(), Url::Url::UNRESERVED,false);

	std::map<std::string, std::string> headers = {
		{ "Content-Type", "application/x-www-form-urlencoded" }		
	};

	CallResponsePtr response = m_httpClient.MakePostCall(Url::Url("https://www.googleapis.com/oauth2/v4/token"), headers, content);

	if (response->result_int() == 200)
	{
		nlohmann::json jsonResponse = nlohmann::json::parse(response->body().data());
		std::string oauth2Token = jsonResponse["access_token"];

		return oauth2Token;
	}

	BOOST_THROW_EXCEPTION(std::runtime_error((boost::format("faild to obtain oauth token, reason: %d %s") % response->result_int() % response->reason()).str().c_str()));
}

std::string FirebaseTopicPublisher::CreateJWT()
{
	UtcClock clock;
	uint64_t now = clock.Now();

	nlohmann::json claim;

	claim["iss"] = m_account.client_email();
	claim["scope"] = "https://www.googleapis.com/auth/firebase.messaging https://www.googleapis.com/auth/cloud-platform https://www.googleapis.com/auth/pubsub";
	claim["aud"] = "https://www.googleapis.com/oauth2/v4/token";
	claim["iat"] = now;
	claim["exp"] = now + 59 * 60 + 59;

	RS256Signer signer(m_account.private_key().c_str());	
	return JWT::Encode(signer, claim);
}

void FirebaseTopicPublisher::PublishToTopic(const std::string& message, const std::string& topic)
{
#ifdef _DEBUG
	std::cout << "publishing to '" << topic << "': " << std::endl << message << std::endl;
#endif
	RefreshOAuth2TokenIfNeeded();

	std::string topicUrl = "https://pubsub.googleapis.com/v1/projects/" + m_account.project_id() + "/topics/" + topic+ ":publish";
	
	nlohmann::json jsonMsgData;
	jsonMsgData["data"] = EncodeBase64(message);
	nlohmann::json jsonBody;
	jsonBody["messages"] = nlohmann::json::array({ jsonMsgData });
	
	std::map<std::string, std::string> headers = { 
		{ "Content-Type", "application/json" },
		{ "Authorization", "Bearer " + m_OAuth2Token} 
	};

	CallResponsePtr response = m_httpClient.MakePostCall(Url::Url(topicUrl), headers, jsonBody.dump());

	if (response->result_int() != 200)
	{
		BOOST_THROW_EXCEPTION(std::runtime_error((boost::format("faild to publish message, reason: %d %s") % response->result_int() % response->reason()).str().c_str()));
	}
}

std::string FirebaseTopicPublisher::EncodeBase64(const std::string& s)
{
	using namespace boost::archive::iterators;
	using encoderIt = base64_from_binary<    
		transform_width<   
		std::string::const_iterator,
		6,
		8
		>
	>;

	std::string encoded;
	encoded.assign(encoderIt(s.begin()), encoderIt(s.end()));
	return encoded;
}

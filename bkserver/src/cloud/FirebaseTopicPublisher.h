#pragma once

#include "GoogleServiceAccount.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "HttpClient.h"


class FirebaseTopicPublisher
{
public:
	FirebaseTopicPublisher(const gcp::GoogleServiceAccount& account);
	void PublishToTopic(const std::string& message, const std::string& topic);
private:
	void RefreshOAuth2TokenIfNeeded();
	std::string RequestOAuth2Token();
	std::string CreateJWT();
	
	std::string EncodeBase64(const std::string& s);

private:
	gcp::GoogleServiceAccount m_account;
	std::string m_OAuth2Token;
	boost::posix_time::ptime m_tokenIssueTime;
	HttpClient m_httpClient;
};


#include "stdafx.h"
#include "FirebaseClient.h"
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
#include <boost/beast/core.hpp>
//#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include "root_certificates.hpp"
#include <string/TextUtils.h>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "FirebaseException.h"

namespace gcp
{

	using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
	namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
	namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

	FirebaseClient::FirebaseClient(const GoogleServiceAccount& account) : m_account(account), m_tokenProvider(account)
	{

	}

	std::map<std::string, std::string> FirebaseClient::CreateHeaders()
	{
		GoogleAuth2Token token = m_tokenProvider.ObtainToken();
		std::map<std::string, std::string> headers = {
			{ "Content-Type", "application/json" },
		{ "Authorization", token.token_type + " " + token.token }
		};

		return std::move(headers);

	}

	json::value FirebaseClient::Get(const Ref& ref)
	{
		Url::Url url = GetUrl(ref);		
		CallResponsePtr response = m_httpClient.MakeGetCall(url, CreateHeaders());
		std::string body = response->body();			
		return json::value::parse(TextUtils::FromUTF8(body));			
	}

	Url::Url FirebaseClient::GetUrl(const Ref& ref)
	{
		Url::Url url("https://" + m_account.project_id() + ".firebaseio.com");
		url /= u(ref.ToString()) + ".json";
		return url;
	}

	void FirebaseClient::Patch(const Ref& ref, const json::value& value)
	{
		Url::Url url = GetUrl(ref);

		CallResponsePtr response = m_httpClient.MakeHttpCall(http::verb::patch,url, CreateHeaders(),TextUtils::ToUTF8(value.serialize()));
		
		
	}

	std::wstring FirebaseClient::Post(const Ref& ref, const json::value& value)
	{
		const wchar_t* error_attribute_name = L"error";
		const wchar_t* id_attribute_name = L"name";

		Url::Url url = GetUrl(ref);

		CallResponsePtr response = m_httpClient.MakeHttpCall(http::verb::post, url, CreateHeaders(), TextUtils::ToUTF8(value.serialize()));
		std::string body = response->body();
		json::value result = json::value::parse(TextUtils::FromUTF8(body));
		
		if (result.has_field(error_attribute_name))
		{
			BOOST_THROW_EXCEPTION(FirebaseException(result[error_attribute_name].as_string()));
		}

		return result[id_attribute_name].as_string();
	}

	void FirebaseClient::Put(const Ref& ref, const json::value& value)
	{
		Url::Url url = GetUrl(ref);

		CallResponsePtr response = m_httpClient.MakeHttpCall(http::verb::put, url, CreateHeaders(), TextUtils::ToUTF8(value.serialize()));
		
	}

	


	
	
}

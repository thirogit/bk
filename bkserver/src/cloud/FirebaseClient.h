#pragma once

#include "GoogleServiceAccount.h"
#include "GoogleAuth2TokenProvider.h"
#include <casajson/json.h>
#include <url.h>
#include <boost/beast/http.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <map>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "ChunkedEncodingReader.hpp"
#include "Ref.h"
#include "HttpClient.h"

namespace gcp
{
	class FirebaseClient
	{
	public:
		FirebaseClient(const GoogleServiceAccount& account);
		json::value Get(const Ref& ref);
		void Patch(const Ref& ref, const json::value& value);
		std::wstring Post(const Ref& ref, const json::value& value);
		void Put(const Ref& ref, const json::value& value);
		
	private:
		Url::Url GetUrl(const Ref& path);
		std::map<std::string, std::string> CreateHeaders();
	private:
		GoogleAuth2TokenProvider m_tokenProvider;
		GoogleServiceAccount m_account;
		HttpClient m_httpClient;
	private:
		

	};

};

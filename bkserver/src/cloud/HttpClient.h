#pragma once

#include <url.h>
#include <boost/beast/http.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <map>
#include "ChunkedEncodingReader.hpp"

class HttpClient
{	
public:	
	CallResponsePtr  MakePostCall(const Url::Url& url, const std::map<std::string, std::string>& headers, const std::string& body);
	CallResponsePtr  MakeGetCall(const Url::Url& url, const std::map<std::string, std::string>& headers);
	CallResponsePtr  MakeHttpCall(boost::beast::http::verb method,const Url::Url& url, const std::map<std::string, std::string>& headers, const std::string& body);
private:	
	boost::asio::io_context ioc;
	boost::asio::ssl::context ctx{ boost::asio::ssl::context::sslv23_client };

};


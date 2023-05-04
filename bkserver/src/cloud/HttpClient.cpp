#include "stdafx.h"
#include "HttpClient.h"
#include <boost/lexical_cast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>

CallResponsePtr HttpClient::MakePostCall(const Url::Url& url, const std::map<std::string, std::string>& headers, const std::string& body)
{
	return MakeHttpCall(http::verb::post,url, headers, body);
}

CallResponsePtr HttpClient::MakeGetCall(const Url::Url& url, const std::map<std::string, std::string>& headers)
{
	return MakeHttpCall(http::verb::get,url, headers, std::string());
}


CallResponsePtr HttpClient::MakeHttpCall(boost::beast::http::verb method,const Url::Url& url,const std::map<std::string, std::string>& headers,const std::string& body)
{
	const int version = 11;

	//load_root_certificates(ctx);
	
	std::string host = url.host();
	int port = url.port();
	port = port != 0 ? port : ("https" == url.scheme() ? 443 : 80);

	auto const target = url.fullpath();
	
	// These objects perform our I/O
	tcp::resolver resolver{ ioc };
		
	// Look up the domain name
	auto const results = resolver.resolve(tcp::v4(),host, boost::lexical_cast<std::string>(port));

	ssl::stream<tcp::socket> stream{ ioc, ctx };


	// Set SNI Hostname (many hosts need this to handshake successfully)
	if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
	{
		boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
		throw boost::system::system_error{ ec };
	}

	// Make the connection on the IP address we get from a lookup
	boost::asio::connect(stream.next_layer(), results.begin(), results.end());
	// Perform the SSL handshake
	stream.handshake(ssl::stream_base::client);

	
	// Set up an HTTP GET request message
	http::request<http::string_body> req{ method, target, version };
	req.set(http::field::host, host);
	req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	req.set(http::field::content_length, body.length());
				
	auto headerIt = headers.begin();
	while (headerIt != headers.end())
	{
		req.set(headerIt->first, headerIt->second);
		headerIt++;
	}

	req.body() = body;
	
#ifdef _DEBUG
	std::cout << req << std::endl;
#endif

	http::write(stream, req);
	
	boost::beast::flat_buffer buffer;
	
	boost::system::error_code ec;
	CallResponsePtr res = boost::beast::http::read_chunked_body<false>(stream, buffer, ec);
		
	//std::cout << "status = " << res->result_int() << std::endl << "data: " << std::endl << res->body() << std::endl;
	boost::system::error_code shutdown_ec;
	stream.shutdown(shutdown_ec);
	//if (shutdown_ec == boost::asio::error::eof)
	//{
	//	// Rationale:
	//	// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
	//	shutdown_ec.assign(0, ec.category());
	//}
	//if (shutdown_ec)
	//	throw boost::system::system_error{ shutdown_ec };

	return res;
	
}

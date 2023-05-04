#pragma once

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/http.hpp>

typedef boost::beast::http::response<boost::beast::http::string_body> CallResponse;
typedef boost::shared_ptr<CallResponse> CallResponsePtr;


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


namespace boost {
	namespace beast {
		namespace http {

			template<
				bool isRequest,
				class SyncReadStream,
				class DynamicBuffer>
				CallResponsePtr
				read_chunked_body(					
					SyncReadStream& stream,
					DynamicBuffer& buffer,
					error_code& ec)
			{
				// Declare the parser with an empty body since
				// we plan on capturing the chunks ourselves.
				
				
				
				parser<isRequest, CallResponse::body_type> p;
						

				// First read the complete header
				read_header(stream, buffer, p, ec);
				if (ec)
					throw boost::system::system_error{ ec };
						
				if (!p.chunked())
				{
					while (!p.is_done())
					{						
						read(stream, buffer, p, ec);
						if (!ec)
							continue;
						else 
							throw boost::system::system_error{ ec };
												
					}

					auto const& msg = p.get();
					CallResponsePtr response(new CallResponse(msg.base(), msg.body()));
					return response;
				}
				else
				{
					CallResponse::body_type::value_type response_buffer;

					CallResponse::header_type header;
					CallResponse::body_type::reader body_reader(header, response_buffer);

					// This container will hold the extensions for each chunk
					chunk_extensions ce;


					// Declare our chunk header callback  This is invoked
					// after each chunk header and also after the last chunk.
					auto header_cb =
						[&](std::uint64_t size,         // Size of the chunk, or zero for the last chunk
							string_view extensions,     // The raw chunk-extensions string. Already validated.
							error_code& ev)             // We can set this to indicate an error
					{
						// Parse the chunk extensions so we can access them easily
						ce.parse(extensions, ev);
						if (ev)
							return;

						// See if the chunk is too big
						if (size > (std::numeric_limits<std::size_t>::max)())
						{
							ev = error::body_limit;
							return;
						}

						// Make sure we have enough storage, and
						// reset the container for the upcoming chunk
	//					chunk.reserve(static_cast<std::size_t>(size));
	//					chunk.clear();
					};

					// Set the callback. The function requires a non-const reference so we
					// use a local variable, since temporaries can only bind to const refs.
					p.on_chunk_header(header_cb);

					// Declare the chunk body callback. This is called one or
					// more times for each piece of a chunk body.
					auto body_cb =
						[&](std::uint64_t remain,   // The number of bytes left in this chunk
							string_view body,       // A buffer holding chunk body data
							error_code& ec)         // We can set this to indicate an error
					{
						// If this is the last piece of the chunk body,
						// set the error so that the call to `read` returns
						// and we can process the chunk.
						if (remain == body.size())
							ec = error::end_of_chunk;

						body_reader.put(boost::asio::buffer(body.data(), body.size()), ec);
						// Append this piece to our container
						//chunk.append(body.data(), body.size());


						// The return value informs the parser of how much of the body we
						// consumed. We will indicate that we consumed everything passed in.
						return body.size();
					};
					p.on_chunk_body(body_cb);

					while (!p.is_done())
					{
						// Read as much as we can. When we reach the end of the chunk, the chunk
						// body callback will make the read return with the end_of_chunk error.
						read(stream, buffer, p, ec);
						if (!ec)
							continue;
						else if (ec != error::end_of_chunk)
							throw boost::system::system_error{ ec };
						else
							ec.assign(0, ec.category());

						// We got a whole chunk, print the extensions:
						/*for (auto const& extension : ce)
						{
							os << "Extension: " << extension.first;
							if (!extension.second.empty())
								os << " = " << extension.second << std::endl;
							else
								os << std::endl;
						}*/

						// Now print the chunk body
						//os << "Chunk Body: " << chunk << std::endl;
					}
					auto const& msg = p.get();
					CallResponsePtr response(new CallResponse(msg.base(), response_buffer));
					return response;
				}

				
			}
		}
	}
}


#define BOOST_ASIO_STANDALONE
#define BOOST_ERROR_CODE_HEADER_ONLY

#include "WebClient.h"

#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/io_service.hpp"
#include "boost/asio/connect.hpp"
#include "boost/asio/streambuf.hpp"
#include "boost/asio/write.hpp"
#include "boost/asio/read_until.hpp"
#include "boost/asio/read.hpp"
#include "boost/system/error_code.hpp"

using boost::asio::ip::tcp;

namespace WebClient
{
	bool HTTP::GET(const std::string &Server, const std::string &Path, std::string *Response) 
	{
		*Response = "";

		boost::asio::io_service io_service;

		// Get a list of endpoints corresponding to the server name.
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(Server, "http");

		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		tcp::socket socket(io_service);
		boost::asio::connect(socket, endpoint_iterator);

		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << Path << " HTTP/1.1\r\n";
		request_stream << "Host: " << Server << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		boost::asio::write(socket, request);

		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.
		std::istream response_stream(&response);
		std::string http_version;

		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;

		if (status_code != 200) return false;

		// Read the response headers, which are terminated by a blank line.
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.
		std::string header;
		while (std::getline(response_stream, header) && header != "\r");

		std::ostringstream response_string;
		boost::system::error_code error;
		while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error)) response_string << &response;
		*Response = response_string.str();

		return true;
	}
}
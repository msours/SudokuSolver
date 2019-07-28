#pragma once

#include <iostream>

namespace WebClient
{
	class HTTP 
	{
	public:
		static bool GET(const std::string &Server, const std::string &Path, std::string *Response);
	};
}

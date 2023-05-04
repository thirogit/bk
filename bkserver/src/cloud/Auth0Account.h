#pragma once

#include <string>
#include <iostream>

class Auth0Account
{
public:
	std::string  token_uri;
	std::string  client_id;
	std::string  client_secret;
	std::string  audience;
	std::string  grant_type;
};

class Auth0AccountFactory
{
public:
	Auth0Account Create(std::istream& is);
};


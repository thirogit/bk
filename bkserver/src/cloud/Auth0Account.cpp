#include "stdafx.h"
#include "Auth0Account.h"
#include "jwt/jwt_all.h"

Auth0Account Auth0AccountFactory::Create(std::istream& is)
{
	nlohmann::json serviceAccountJson = nlohmann::json::parse(is);
	Auth0Account account;

	account.token_uri = serviceAccountJson.value("token_uri", "");
	account.client_id = serviceAccountJson.value("client_id", "");	
	account.client_secret = serviceAccountJson.value("client_secret", "");
	account.audience = serviceAccountJson.value("audience", "");
	account.grant_type = serviceAccountJson.value("grant_type", "");
	
	return account;
}

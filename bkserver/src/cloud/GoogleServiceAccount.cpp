#include "stdafx.h"
#include "GoogleServiceAccount.h"
#include "jwt/jwt_all.h"

namespace gcp
{

	std::string GoogleServiceAccount::project_id() const
	{
		return m_Value->project_id;
	}

	std::string GoogleServiceAccount::private_key_id() const
	{
		return m_Value->private_key_id;
	}

	std::string GoogleServiceAccount::private_key() const
	{
		return m_Value->private_key;
	}

	std::string GoogleServiceAccount::client_email() const
	{
		return m_Value->client_email;
	}

	std::string GoogleServiceAccount::client_id() const
	{
		return m_Value->client_id;
	}

	std::string  GoogleServiceAccount::auth_uri() const
	{
		return m_Value->auth_uri;
	}

	std::string  GoogleServiceAccount::token_uri() const
	{
		return m_Value->token_uri;
	}

	std::string  GoogleServiceAccount::auth_provider_x509_cert_url() const
	{
		return m_Value->auth_provider_x509_cert_url;
	}

	std::string  GoogleServiceAccount::client_x509_cert_url() const
	{
		return m_Value->client_x509_cert_url;
	}

	void GoogleServiceAccount::MakeNull()
	{		
		m_Value.reset();
	}

	bool GoogleServiceAccount::IsNull() const
	{
		return !m_Value.is_initialized();
	}
	
//-------------------------------------------------------------------------------------------------

	GoogleServiceAccount GoogleServiceAccountFactory::Create(std::istream& is)
	{
		nlohmann::json serviceAccountJson = nlohmann::json::parse(is);
		GoogleServiceAccount::_GoogleServiceAccount _account;

		_account.project_id = serviceAccountJson.value("project_id", "");
		_account.private_key_id = serviceAccountJson.value("private_key_id", "");
		_account.private_key = serviceAccountJson.value("private_key", "");
		_account.client_email = serviceAccountJson.value("client_email", "");
		_account.client_id = serviceAccountJson.value("client_id", "");
		_account.auth_uri = serviceAccountJson.value("auth_uri", "");
		_account.token_uri = serviceAccountJson.value("token_uri", "");
		_account.auth_provider_x509_cert_url = serviceAccountJson.value("auth_provider_x509_cert_url", "");
		_account.client_x509_cert_url = serviceAccountJson.value("client_x509_cert_url", "");
		
		GoogleServiceAccount account;
		account.m_Value = _account;

		return account;
	}
}

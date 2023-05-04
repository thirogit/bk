#pragma once

#include <string>
#include <iostream>
#include <boost/optional.hpp>
#include <types/Nullable.h>

namespace gcp
{
	class GoogleServiceAccountFactory;	

	class GoogleServiceAccount : public Nullable
	{
		struct _GoogleServiceAccount
		{
			std::string project_id;
			std::string private_key_id;
			std::string private_key;
			std::string client_email;
			std::string client_id;
			std::string  auth_uri;
			std::string  token_uri;
			std::string  auth_provider_x509_cert_url;
			std::string  client_x509_cert_url;
		};


	public:

		virtual void MakeNull();
		virtual bool IsNull() const;

		std::string project_id() const;
		std::string private_key_id() const;
		std::string private_key() const;
		std::string client_email() const;
		std::string client_id() const;
		std::string  auth_uri() const;
		std::string  token_uri() const;
		std::string  auth_provider_x509_cert_url() const;
		std::string  client_x509_cert_url() const;
		
	private:

		boost::optional<_GoogleServiceAccount> m_Value;

		friend class GoogleServiceAccountFactory;
	};

	class GoogleServiceAccountFactory
	{
	public:
		GoogleServiceAccount Create(std::istream& is);
	};
};
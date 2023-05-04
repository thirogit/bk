#include "stdafx.h"
#include "DbConnectionOptions.h"

DbConnectionOptions::DbConnectionOptions() : m_port(0)
{
}

const std::string& DbConnectionOptions::Host() const
{
	return m_host;
}

DbConnectionOptions& DbConnectionOptions::Host(const std::string& sHost)
{
	m_host = sHost;
	return *this;
}

uint16_t DbConnectionOptions::Port() const
{
	return m_port;
}

DbConnectionOptions& DbConnectionOptions::Port(uint16_t port)
{
	m_port = port;
	return *this;
}

const std::string& DbConnectionOptions::User() const
{
	return m_user;
}

DbConnectionOptions& DbConnectionOptions::User(const std::string& user)
{
	m_user = user;
	return *this;
}

const std::string& DbConnectionOptions::Password() const
{
	return m_password;
}

DbConnectionOptions& DbConnectionOptions::Password(const std::string& password)
{
	m_password = password;
	return *this;
}


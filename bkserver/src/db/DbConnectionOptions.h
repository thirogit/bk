#ifndef __DBCONNECTIONOPTIONS_H__
#define __DBCONNECTIONOPTIONS_H__

#include <stdint.h>
#include <string>

class DbConnectionOptions
{
public:
	DbConnectionOptions();

	const std::string& Host() const;
	DbConnectionOptions& Host(const std::string& sHost);

	uint16_t Port() const;
	DbConnectionOptions& Port(uint16_t port);

	const std::string& User() const;
	DbConnectionOptions& User(const std::string&);

	const std::string& Password() const;
	DbConnectionOptions& Password(const std::string&);

private:
	std::string m_host;
	uint16_t m_port;
	std::string m_user;
	std::string m_password;
};

#endif

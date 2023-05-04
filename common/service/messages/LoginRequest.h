#pragma once

#include <string>
#include <data/types/FarmNo.h>
#include <SF/Archive.hpp>

class LoginRequest
{
private:
	FarmNo      m_FarmNo;
	std::wstring m_Login;
	std::wstring m_Password;
	std::wstring m_ClientVersion;
public:

	LoginRequest();

	LoginRequest& WithLogin(const std::wstring& sLogin);
	LoginRequest& WithPassword(const std::wstring& sPassowrd);
	LoginRequest& WithClientVersion(const std::wstring& sClientVersion);
	LoginRequest& WithFarmNo(const FarmNo& farmNo);

	const std::wstring& GetLogin() const;
	const std::wstring& GetPassword() const;
	const FarmNo& GetFarmNo() const;
	const std::wstring& GetClientVersion() const;

	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<LoginRequest> LoginRequestObj;

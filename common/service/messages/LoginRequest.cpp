#include "stdafx.h"
#include "LoginRequest.h"
#include <data/types/serialization/TypesSerialization.h>
#include <SF/string.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LoginRequest::LoginRequest()
{
}

LoginRequest& LoginRequest::WithLogin(const std::wstring& sLogin)
{
	m_Login = sLogin;
	return *this;
}

LoginRequest& LoginRequest::WithPassword(const std::wstring& sPassowrd)
{
	m_Password = sPassowrd;
	return *this;
}

LoginRequest& LoginRequest::WithFarmNo(const FarmNo& farmNo)
{
	m_FarmNo = farmNo;
	return *this;
}

LoginRequest& LoginRequest::WithClientVersion(const std::wstring& sClientVersion)
{
	m_ClientVersion = sClientVersion;
	return *this;
}

const std::wstring& LoginRequest::GetLogin() const
{
	return m_Login;
}

const std::wstring& LoginRequest::GetPassword() const
{
	return m_Password;
}

const FarmNo& LoginRequest::GetFarmNo() const
{
	return m_FarmNo;
}

const std::wstring& LoginRequest::GetClientVersion() const
{
	return m_ClientVersion;
}

void LoginRequest::serialize(SF::Archive &archive)
{
	archive & m_FarmNo & m_Login & m_Password & m_ClientVersion;
}

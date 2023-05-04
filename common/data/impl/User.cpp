#include "stdafx.h"
#include "User.h"

User::User() : m_Id(NULL_ID),m_pFarm(NULL)
{
}

User::User(uint32_t Id,IFarm* pFarm)
{
	m_Id = Id;
	m_pFarm = pFarm;
}
	
void User::CopyFrom(const User &copy)
{
	m_UserName = copy.m_UserName;
	m_FirstName = copy.m_FirstName;
	m_LastName = copy.m_LastName;
	m_Password = copy.m_Password;
	m_LastLogin = copy.m_LastLogin;	
}

User::User(User &copy)
{
	CopyFrom(copy);
}


User& User::operator=(const User& right)
{
	CopyFrom(right);
	return *this;
}
	
const std::wstring& User::GetLogin() const
{
	return m_UserName;
}

uint32_t User::GetId() const
{
	return m_Id;
}

const std::wstring& User::GetFirstName() const
{
	return m_FirstName;
}

const std::wstring& User::GetLastName() const
{
	return m_LastName;
}

const std::wstring& User::GetPassword() const
{
	return m_Password;
}

const DateTime& User::GetLastLogin() const
{
	return m_LastLogin;
}

const IFarm* User::GetFarm() const 
{
	return m_pFarm;
}

void User::SetLogin(const std::wstring& sUserName)
{
	m_UserName = sUserName;
}

void User::SetFirstName(const std::wstring& sFirstName)
{
	m_FirstName = sFirstName;
}

void User::SetLastName(const std::wstring& sLastName)
{
	m_LastName = sLastName;
}

void User::SetPassword(const std::wstring& sPassword)
{
	m_Password = sPassword;
}

void User::SetLastLogin(const DateTime& lastLogin)
{
	m_LastLogin = lastLogin;
}
		


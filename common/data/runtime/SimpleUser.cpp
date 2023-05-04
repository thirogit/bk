#include "stdafx.h"
#include "SimpleUser.h"
#include <SF/string.hpp>

SimpleUser::SimpleUser(uint32_t id) : m_id(id)
{
}

SimpleUser::SimpleUser(const SimpleUser& src)
{
	m_id = src.m_id;
	m_login = src.m_login;
	m_firstname = src.m_firstname;
	m_lastname = src.m_lastname;
}

SimpleUser::SimpleUser(const ISimpleUser* src)
{
	m_id = src->GetId();
	CopyFrom(src);
}

void SimpleUser::CopyFrom(const ISimpleUser* pUser)
{
	m_login = pUser->GetLogin();
	m_firstname = pUser->GetFirstName();
	m_lastname = pUser->GetLastName();
}

const std::wstring& SimpleUser::GetLogin() const
{
	return m_login;
}

const std::wstring& SimpleUser::GetFirstName() const
{
	return m_firstname;
}

const std::wstring& SimpleUser::GetLastName() const
{
	return m_lastname;
}

uint32_t SimpleUser::GetId() const
{
	return m_id;
}

void SimpleUser::SetLogin(const std::wstring& login)
{
	m_login = login;
}

void SimpleUser::SetFirstName(const std::wstring& firstname)
{
	m_firstname = firstname;
}

void SimpleUser::SetLastName(const std::wstring& lastname)
{
	m_lastname = lastname;
}


void SimpleUser::serialize(SF::Archive &archive)
{
	archive & m_id;
	archive & m_login;
	archive & m_firstname;
	archive & m_lastname;
}
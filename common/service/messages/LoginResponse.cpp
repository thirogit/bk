#include "stdafx.h"
#include "LoginResponse.h"


LoginResponse::LoginResponse() : m_Result(LoginResult_InvalidLogin),m_pOrganisation(new FarmOrganisation()), m_user(new SimpleUser(NULL_ID))
{
}

LoginResponse::~LoginResponse()
{
	delete m_pOrganisation;
	delete m_user;
}
	
LoginResponse& LoginResponse::WithResult(LoginResult result)
{
	m_Result = result;
	return *this;
}

LoginResult LoginResponse::GetResult()
{
	return m_Result;
}

const FarmOrganisation* LoginResponse::GetFarmOrganisation() const
{
	return m_pOrganisation;
}

LoginResponse& LoginResponse::WithFarmOrganisation(const FarmOrganisation& organisation)
{
	m_pOrganisation->CopyFrom(&organisation);
	return *this;
}

LoginResponse& LoginResponse::WithUser(const SimpleUser& user)
{
	m_user->CopyFrom(&user);
	return *this;
}

ISimpleUser* LoginResponse::GetUser() const
{
	return m_user;
}

void LoginResponse::serialize(SF::Archive &archive)
{
	archive & m_Result;
	if(archive.isRead())
	{
		if(m_Result == LoginResult_Success)
		{			
			m_pOrganisation->serialize(archive);
			m_user->serialize(archive);
		}
	}
	else
	{
		if(m_Result == LoginResult_Success)
		{
			m_pOrganisation->serialize(archive);
			m_user->serialize(archive);
		}
	}
}



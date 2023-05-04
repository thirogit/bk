#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>
#include "FarmOrganisation.h"
#include <data/runtime/SimpleUser.h>

enum LoginResult
{
	LoginResult_InvalidLogin,	
	LoginResult_Success	
};

class LoginResponse
{
private:
	LoginResult m_Result;
	FarmOrganisation* m_pOrganisation;
	SimpleUser* m_user;
public:
	LoginResponse();
	~LoginResponse();
	
	LoginResponse& WithResult(LoginResult result);
	LoginResponse& WithUser(const SimpleUser& user);
	LoginResponse& WithFarmOrganisation(const FarmOrganisation& organisation);

	LoginResult GetResult();
	const FarmOrganisation* GetFarmOrganisation() const;
	ISimpleUser* GetUser() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<LoginResponse> LoginResponseObj;

#pragma once

#include "UserSession.h"

class UserSessionBuilder
{	
public:
	UserSessionBuilder(uint32_t userId);
	UserSession* Build();
private:	
	uint32_t m_userId;	
};

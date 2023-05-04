#include "stdafx.h"
#include "UserSessionBuilder.h"
#include "dao/UserDAO.h"
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>

UserSessionBuilder::UserSessionBuilder(uint32_t userId) : m_userId(userId)
{
}

UserSession* UserSessionBuilder::Build()
{
	UserDAO dao;	
	NewPtrFlushArray<PrintProfile> userPrintProfiles;
	NewPtrFlushArray<PassportRecognitionProfile> userPassRecogntnProfiles;
	NewPtrFlushArray<DeductionDefinition> deductionDefinitions;
	

	dao.LoadPrintProfiles(userPrintProfiles, m_userId);
	dao.LoadPassRecgntnProfiles(userPassRecogntnProfiles, m_userId);
	dao.LoadDeductionDefinitions(deductionDefinitions, m_userId);
	

	UserSession* session = new UserSession(m_userId);
	session->m_settingsManager.Initialize();
	session->m_settingsManager.Load();

	auto printProfileIt = userPrintProfiles.begin();
	while (printProfileIt != userPrintProfiles.end())
	{
		session->m_printProfiles.insert(*printProfileIt);
		printProfileIt++;
	}

	auto passRecogntnProfileIt = userPassRecogntnProfiles.begin();
	while (passRecogntnProfileIt != userPassRecogntnProfiles.end())
	{
		session->m_passRecgntnProfiles.insert(*passRecogntnProfileIt);
		passRecogntnProfileIt++;
	}
	
	BOOST_FOREACH(DeductionDefinition* definition, deductionDefinitions)
	{
		session->m_deductionDefinitions.insert(definition);
	}

	return session;
}



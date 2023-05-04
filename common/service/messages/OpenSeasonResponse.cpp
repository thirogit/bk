#include "stdafx.h"
#include "OpenSeasonResponse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OpenSeasonResponse::OpenSeasonResponse() : m_Result(Invalid_SeasonId),m_SeasonSessionToken(0),m_ObserverPort(0)
{
}

OpenSeasonResponse::~OpenSeasonResponse()
{
	
}


OpenSeasonResponse& OpenSeasonResponse::WithObserverPort(uint16_t observerPort)
{
	m_ObserverPort = observerPort;
	return *this;
}

OpenSeasonResponse& OpenSeasonResponse::WithResult(OpenSeasonResult result)
{
	m_Result = result;
	return *this;
}

OpenSeasonResult OpenSeasonResponse::GetResult() const
{
	return m_Result;
}

OpenSeasonResponse& OpenSeasonResponse::WithSeasonSessionToken(uint32_t sessionToken)
{
	m_SeasonSessionToken = sessionToken;
	return *this;
}

void OpenSeasonResponse::serialize(SF::Archive &archive)
{
	archive & m_Result;	
	archive & m_SeasonSessionToken;
	archive & m_ObserverPort;	
}

uint32_t OpenSeasonResponse::GetSessionToken() const
{
	return m_SeasonSessionToken;
}

uint16_t OpenSeasonResponse::GetObserverPort() const
{
	return m_ObserverPort;
}



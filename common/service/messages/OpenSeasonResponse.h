#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>

enum OpenSeasonResult
{
	Invalid_SeasonId,	
	Open_Successful
};

class OpenSeasonResponse
{
private:
	OpenSeasonResult m_Result;
	uint32_t m_SeasonSessionToken;
	uint16_t m_ObserverPort;
public:
	OpenSeasonResponse();
	~OpenSeasonResponse();
	
	OpenSeasonResponse& WithObserverPort(uint16_t observerPort);
	OpenSeasonResponse& WithSeasonSessionToken(uint32_t sessionToken);
	OpenSeasonResponse& WithResult(OpenSeasonResult result);

	OpenSeasonResult GetResult() const;
	uint32_t GetSessionToken() const;
	uint16_t GetObserverPort() const;
	
	void serialize(SF::Archive &archive);
};

typedef boost::shared_ptr<OpenSeasonResponse> OpenSeasonResponseObj;

#pragma once

class SeasonSession;

class SeasonSessionFinalizer
{
public:
	SeasonSessionFinalizer(SeasonSession* session);
	void Finalize();	
private:
	SeasonSession* m_session;
	
};
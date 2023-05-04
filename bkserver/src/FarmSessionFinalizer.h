#pragma once

class FarmSession;

class FarmSessionFinalizer
{
public:
	FarmSessionFinalizer(FarmSession* session);
	void Finalize();	
private:
	FarmSession* m_session;
	
};
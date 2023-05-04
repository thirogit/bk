#pragma once

class SeasonSession;

class ScopedUpdateState
{
public:
	ScopedUpdateState(SeasonSession* pSession);
	~ScopedUpdateState();
private:
	SeasonSession* m_pSession;
};


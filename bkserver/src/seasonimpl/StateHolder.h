#pragma once

#include "SeasonSessionState.h"

class StateHolder
{
public:
	StateHolder(SeasonSessionState stateValue);

	SeasonSessionState value() const;
	int count() const;
	void increase();
	void decrease();
		
private:
	SeasonSessionState m_State;
	int m_Counter;
};


bool operator==(const StateHolder&  holder,SeasonSessionState stateValue);
bool operator==(SeasonSessionState stateValue,const StateHolder&  holder);


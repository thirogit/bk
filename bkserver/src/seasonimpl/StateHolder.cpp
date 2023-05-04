#include "stdafx.h"
#include "StateHolder.h"

StateHolder::StateHolder(SeasonSessionState stateValue) : m_State(stateValue),m_Counter(0)
{
}

SeasonSessionState StateHolder::value() const
{
	return m_State;
}

int StateHolder::count() const
{
	return m_Counter;
}

void StateHolder::increase()
{
	m_Counter++;
}

void StateHolder::decrease()
{
	m_Counter--;
}


bool operator==(const StateHolder&  holder,SeasonSessionState stateValue)
{
	return holder.value() == stateValue;
}

bool operator==(SeasonSessionState stateValue,const StateHolder&  holder)
{
	return holder == stateValue;
}

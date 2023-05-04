#include "stdafx.h"
#include "ScopedUpdateState.h"
#include "SeasonSession.h"

ScopedUpdateState::ScopedUpdateState(SeasonSession* pSession) : m_pSession(pSession)
{
	m_pSession->EnterUpdateState();
}

ScopedUpdateState::~ScopedUpdateState()
{
	m_pSession->ExitUpdateState();
}

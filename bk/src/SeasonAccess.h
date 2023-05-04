#pragma once

#include "context/SeasonSession.h"

namespace SeasonAccess
{
	SeasonSession* GetAssociatedSeason(CWnd* pWnd);
	void RemoveAssociation(CWnd* pWnd);
	void CreateAssociation(CWnd* pWnd,SeasonSession* pSession);
}

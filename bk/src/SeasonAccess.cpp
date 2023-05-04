#include "stdafx.h"
#include "SeasonAccess.h"
#include <map>

namespace SeasonAccess
{
	std::map<HWND,SeasonSession*> g_Associations;

	SeasonSession* GetAssociatedSeason(CWnd* pWnd)
	{		
		auto assocIt = g_Associations.find(pWnd->GetSafeHwnd());
		if(assocIt != g_Associations.end())
			return assocIt->second;
		else
		{
			CWnd* pParent = pWnd->GetParent();
			if(pParent != NULL)
				return GetAssociatedSeason(pParent);
		}
		
		return NULL;
	}
	
	void RemoveAssociation(CWnd* pWnd)
	{
		g_Associations.erase(pWnd->GetSafeHwnd());
	}
	
	void CreateAssociation(CWnd* pWnd,SeasonSession* pSeason)
	{
		g_Associations[pWnd->GetSafeHwnd()] = pSeason;
	}
}
#include "stdafx.h"
#include "AgentsComboContent.h"

void AgentsComboContent::CreateContent(SeasonSession* facade)
{
	IAgent* pAgent;
	PtrEnumerator<IAgent> agentEnum = facade->EnumAgents();
	while(agentEnum.hasNext())
	{
		pAgent = *agentEnum;
		AddItem(pAgent->GetId(),pAgent->GetAgentCode());
		agentEnum.advance();
	}
}

void AgentsComboContent::DestroyContent()
{
	ComboContent::Destroy();
}

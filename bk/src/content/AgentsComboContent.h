#pragma once

#include "../ctrls/BaseCombo.h"
#include "../context/SeasonSession.h"

class AgentsComboContent : public ComboContent
{
public:
	void CreateContent(SeasonSession* facade);
	void DestroyContent();
};



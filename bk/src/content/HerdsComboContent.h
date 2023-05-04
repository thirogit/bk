#ifndef __HERDSCOMBOCONTENT_H__
#define __HERDSCOMBOCONTENT_H__

#include "../ctrls/BaseCombo.h"
#include <data\IHerd.h>
#include "../context/SeasonSession.h"

class HerdsComboContent : public ComboContent
{
public:
	void CreateContent(SeasonSession* facade);
	void DestroyContent();
};

#endif
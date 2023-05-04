#ifndef __STOCKSCOMBOCONTENT_H__
#define __STOCKSCOMBOCONTENT_H__

#include "../ctrls/BaseCombo.h"
#include <data\IStock.h>
#include "../context/SeasonSession.h"
#include "../context/XContext.h"

class StocksComboContent : public ComboContent
{
public:
	void CreateContent(SeasonSession* facade);
	void CreateContent(XContext* context);
	void DestroyContent();
private:
	void CreateContent(PtrEnumerator<IStock>& stocksEnum);
};


#endif
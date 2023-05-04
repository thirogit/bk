#pragma once

#include "RecognitionItemView.h"
#include "../../../context/SeasonSession.h"
#include "../../session/IRecognitionItem.h"

class RecognitionItemViewCreator 
{
public:
	RecognitionItemViewCreator(SeasonSession* facade);
	RecognitionItemView* CreateView(IRecognitionItem* pItem);
	void RefreshView(RecognitionItemView* pView,IRecognitionItem* pItem);
private:
	SeasonSession* m_pFacade;
};

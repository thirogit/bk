#include "stdafx.h"
#include "StocksComboContent.h"
#include "../context/SeasonSession.h"

void StocksComboContent::CreateContent(SeasonSession* facade)
{	
	PtrEnumerator<IStock> stocksEnum = facade->EnumStocks();
	CreateContent(stocksEnum);
}

void StocksComboContent::DestroyContent()
{
	ComboContent::Destroy();
}

void StocksComboContent::CreateContent(XContext* context)
{
	PtrEnumerator<IStock> stocksEnum = context->EnumStocks();
	CreateContent(stocksEnum);
}

void StocksComboContent::CreateContent(PtrEnumerator<IStock>& stocksEnum)
{
	IStock* pStock = NULL;
	while (stocksEnum.hasNext())
	{
		pStock = *stocksEnum;
		AddItem(pStock->GetId(), pStock->GetStockCode());
		stocksEnum.advance();
	}
}

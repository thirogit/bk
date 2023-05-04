#pragma once

struct SellDocOperations
{
	enum
	{
		Operation_GetDocId = 1,
		Operation_GetDocDetails,
		Operation_SetDocDetails,
		Operation_SetHent,
		Operation_GetHent,
		Operation_AddCow,
		Operation_DeleteCow,
		Operation_UpdateEntry,
		Operation_GetItemsCount,
		Operation_GetItemAt,
		Operation_GetHerd,		
		Operation_GetItem
	};
};
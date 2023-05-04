#pragma once

struct MoveDocOperations
{
	enum
	{
		Operation_GetDocId = 1,
		Operation_GetDocDetails,
		Operation_SetDocDetails,
		Operation_AddCow,
		Operation_DeleteCow,
		Operation_UpdateEntry,
		Operation_GetItemsCount,
		Operation_GetItemAt,
		Operation_GetItem,
		Operation_GetSrcHerdId,
		Operation_GetDstHerdId		
	};
};
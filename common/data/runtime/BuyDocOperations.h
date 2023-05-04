#pragma once

struct BuyDocOperations
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
		Operation_UpdateCow,
		Operation_UpdateEntry,
		Operation_GetItemsCount,
		Operation_GetItem,
		Operation_GetItemAt,
		Operation_GetHerd,
		Operation_StartPassportImageUpload,
		Operation_UploadPassportImageChunk,
		Operation_EndPassportImageUpload,
		Operation_ErasePassportImage		
	};
};
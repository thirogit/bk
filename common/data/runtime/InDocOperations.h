#pragma once

struct InDocOperations
{
	enum
	{
		Operation_GetDocId = 1,
		Operation_GetDocDetails,
		Operation_SetDocDetails,
		Operation_AddCow,
		Operation_DeleteCow,
		Operation_UpdateCow,
		Operation_UpdateEntry,
		Operation_GetItemsCount,
		Operation_GetItemAt,		
		Operation_GetHerdId,
		Operation_StartPassportImageUpload,
		Operation_UploadPassportImageChunk,
		Operation_EndPassportImageUpload,
		Operation_ErasePassportImage		
	};
};
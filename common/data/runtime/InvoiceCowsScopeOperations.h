#pragma once

struct InvoiceCowsScopeOperations
{
	enum
	{
		Operation_GetInvoiceId = 1,
		Operation_GetInvoiceNo,
		
		Operation_GetInvoiceDetails,
		Operation_SetInvoiceDetails,
		
		Operation_GetHent,
		Operation_SetHent,
		Operation_UpdateHent,

		Operation_GetDoc,
		Operation_SetDoc,

		Operation_AddCow,
		Operation_UpdateCow,
		Operation_UpdateCowInvoiceEntry,

		Operation_DeleteItem,
		Operation_GetItemsCount,
		Operation_GetItemAt,		
		Operation_GetItem,

		Operation_GetDeductionsCount,
		Operation_GetDeductionAt,
		Operation_GetDeduction,
		Operation_AddDeduction,
		Operation_RemoveDeduction,		
		Operation_UpdateDeduction,
		Operation_GetInvoiceType,
		Operation_SetInvoiceType,
	};
};
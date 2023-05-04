#pragma once

struct InvoiceDocScopeOperations
{
	enum
	{
		Operation_GetInvoiceId = 1,
		Operation_GetInvoiceNo,
		
		Operation_GetInvoiceDetails,
		Operation_SetInvoiceDetails,
		
		Operation_GetHent,
		Operation_SetHent,		
				
		Operation_GetDocCount,
		Operation_GetDocAt,		
		Operation_AddDoc,
		Operation_DeleteDoc,
		
		Operation_UpdateCow,
		Operation_UpdateCowInvoiceEntry,
		
		Operation_GetItemsCount,
		Operation_GetItemAt,
		Operation_GetItem,
		
		Operation_GetDocItemAt,
		Operation_GetDocItemCount,

		Operation_GetScope,
		Operation_GetDirection,		
		
		Operation_GetDeductionsCount,
		Operation_GetDeductionAt,
		Operation_GetDeduction,
		Operation_UpdateDeduction,
		Operation_AddDeduction,
		Operation_RemoveDeduction,

		Operation_GetInvoiceType,
		Operation_SetInvoiceType,
		
	};
};
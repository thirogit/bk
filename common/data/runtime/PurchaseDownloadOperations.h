#pragma once

struct PurchaseDownloadOperations
{
	enum
	{	
		Operation_GetPurchaseId = 1,
		Operation_GetIssues,
		Operation_SolveSingleIssue,
		Operation_SolveCategoryIssue,
		Operation_GetPurchase,
		Operation_SetDocDate,
	};
};
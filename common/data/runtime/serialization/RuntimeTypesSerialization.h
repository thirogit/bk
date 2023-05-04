#pragma once

#include <SF/Archive.hpp>
#include "../ItemId.h"
#include "../InvoiceItemKey.h"
#include "../PurchaseDownloadIssues.h"
#include "../SolutionParameters.h"
#include "../PurchaseDownloadIssueCategory.h"

namespace SF
{
	void serialize(SF::Archive &archive,ItemId& id);
	void serialize(SF::Archive &archive,InvoiceItemKey& key);
	void serialize(SF::Archive &archive, PurchaseDownloadIssues& issues);
	void serialize(SF::Archive &archive, SolutionParameters& parameters);
	void serialize(SF::Archive &archive, PurchaseDownloadIssueCategory& category);
}

#include "stdafx.h"
#include "RuntimeTypesSerialization.h"
#include "../../impl/serialization/DataSerialization.h"
#include <SF/SerializeStaticArray.hpp>
#include "PurchaseDownloadIssuesSerializer.h"
#include "../../../CommonTypes.h"
#include "SolutionParametersSerializer.h"
#include "PurchaseDownloadIssueCategorySerializer.h"

namespace SF
{
	void serialize(SF::Archive &archive,ItemId& id)
	{
		for(int i = 0 ;i < ItemId::static_size();i++)
			archive & id.data;
	}

	void serialize(SF::Archive &archive,InvoiceItemKey& key)
	{
		if(archive.isWrite())
		{
			archive & key.GetDocKey();
			archive & key.GetItemId();
		}
		else
		{
			DocKey docKey;
			ItemId itemId;

			archive & docKey;
			archive & itemId;

			key = InvoiceItemKey(docKey,itemId);
		}
	}

	void serialize(SF::Archive &archive, PurchaseDownloadIssues& issues)
	{
		PurchaseDownloadIssuesSerializer(&issues).serialize(archive);
	}


	void serialize(SF::Archive &archive, SolutionParameters& parameters)
	{
		SolutionParametersSerializer(&parameters).serialize(archive);
	}

	void serialize(SF::Archive &archive, PurchaseDownloadIssueCategory& category)
	{
		PurchaseDownloadIssueCategorySerializer(&category).serialize(archive);
	}
	
}

//void serialize(SF::Archive &archive,DocItem& item)
//{
//	if(archive.isRead())
//	{
//		ItemId id;
//		CowDetails cowDetails;

//		CowEntryDetails entryDetails;
//
//		serialize(archive,id);
//		archive & cowDetails;
//		archive & entryDetails;
//		item.SetItemId(id);
//		item.SetCow(cowDetails);
//		item.SetEntry(entryDetails);
//
//	}
//	else
//	{
//		ItemId id = item.GetItemId();
//		serialize(archive,id);
//		archive & item.GetCow();
//		archive & item.GetEntry();		
//	}
//
//
//}
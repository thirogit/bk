#include "stdafx.h"
#include "RepoTypesSerialization.h"
#include "SerializablePurchase.h"

void serialize(SF::Archive &archive,Purchase& purchase)
{
	SerializablePurchase(&purchase).serialize(archive);
}


void serialize(SF::Archive &archive, PurchaseObjPath& purchaseObjPath)
{
	if (archive.isWrite())
	{
		archive & purchaseObjPath.IsNull() & purchaseObjPath.GetId() & purchaseObjPath.GetType() & purchaseObjPath.HasChild();
		if (purchaseObjPath.HasChild())
		{
			PurchaseObjPath child = purchaseObjPath.GetChild();
			serialize(archive, child);
		}
	}
	else
	{
		bool null;
		PurchaseObjType type;
		std::wstring id;
		bool hasChild;

		archive & null & id & type & hasChild;

		if (null)
		{
			purchaseObjPath.MakeNull();
		}
		else
		{
			PurchaseObjPath parent(type,id);
			if (hasChild)
			{
				PurchaseObjPath child;
				serialize(archive, child);
				purchaseObjPath = parent / child;
			}
			else
			{
				purchaseObjPath = parent;
			}
		}
	}
}

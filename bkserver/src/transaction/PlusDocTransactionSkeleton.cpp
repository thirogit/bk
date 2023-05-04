#include "stdafx.h"
#include "PlusDocTransactionSkeleton.h"
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <boost/foreach.hpp>
#include <errortoken/ErrorToken.h>
#include "TransactionException.h"

PlusDocTransactionSkeleton::PlusDocTransactionSkeleton(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId) : DocTransactionSkeleton(pInterface, logger, transactionId)
{
}

PlusDocTransactionSkeleton::~PlusDocTransactionSkeleton()
{
}

ItemId PlusDocTransactionSkeleton::AddCow(const CowDetails& details)
{
	BOOST_FOREACH(DocTransactionCowItem* item,m_Items)
	{		
		if(item->GetCow().GetCowNo() == details.GetCowNo())
		{
			BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"adding_causes_dupl")(L"cowno",details.GetCowNo().ToString()).str()));
		}
	}

	return DocTransactionSkeleton::AddCow(NULL_ID,details);
}

void PlusDocTransactionSkeleton::DeleteCow(ItemId id)
{
	DocTransactionCowItem* item = FindItem(id);
	uint32_t cowId = item->GetCowId();
	if(cowId != NULL_ID)
	{
		Do([this,cowId,id](SeasonSessionFacade* facade)
		{
			ICowTrail* pTrail = facade->GetCowTrail(cowId);
			ICow* pCow = facade->FindCow(cowId);
			if(!pTrail->IsStationary())
			{
				BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"doc_cow_was_moved")(L"cowNo",pCow->GetCowNo().ToString()).str()));
			}
			DocTransactionSkeleton::DeleteCow(id);
		});
	}
	else
	{
		DocTransactionSkeleton::DeleteCow(id);
	}
}

void PlusDocTransactionSkeleton::UpdateCow(ItemId id, const CowDetails& details)
{
	std::vector<DocTransactionCowItem*> otherItems;
	boost::range::copy( m_Items | boost::adaptors::filtered([id](DocTransactionCowItem* item) -> bool
																								{
																									return item->GetId() != id;
																								}),
						std::back_inserter(otherItems) );


	auto duplicates = boost::range::count_if(otherItems,[details](DocTransactionCowItem* item) -> bool
								{
									return item->GetCow().GetCowNo() == details.GetCowNo();
								});

	if(duplicates > 0)
	{
		BOOST_THROW_EXCEPTION(TransactionException(ErrorToken(L"update_causes_dupl")(L"cowno",details.GetCowNo().ToString()).str()));
	}

	DocTransactionCowItem* item = FindItem(id);
	item->SetCow(details);
}

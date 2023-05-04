#include "stdafx.h"
#include "TransferBuyDocTransaction.h"
#include <data/runtime/InDocOperations.h>
#include <boost/range/adaptors.hpp>
#include <boost/foreach.hpp>
#include <boost/range/algorithm.hpp>
#include "TransactionException.h"
#include <data/runtime/DocTransactionItemTuple.h>
//-------------------------------------------------------------------------------------------------

TransferBuyDocTransaction::TransferBuyDocTransaction(ISeasonInterface* pInterface, LogInterface* logger, uint32_t transactionId)
{
}

void TransferBuyDocTransaction::Open(uint32_t docId)
{
	m_docId = docId;
	/*Do([this](SeasonSessionFacade* facade)
			  {
					IBuyDoc* pDoc = facade->FindBuyDoc(m_docId);
					m_bInvoiced	= 	pDoc->GetInvoiceId() != NULL_ID;			
			  });*/
}


boost::any TransferBuyDocTransaction::ExecuteOperation(uint16_t operation,const OperationArguments& args)
{
	/*switch(operation)
	{
		case TransferBuyDocOperations::Operation_SetHerd:
			m_dstHerdId = args.get<uint32_t>(0));
			break;
		
	}*/
	return boost::any();
}

uint32_t TransferBuyDocTransaction::GetId() const
{
	return 0;// GetTransactionId();
}

void TransferBuyDocTransaction::Commit()
{
	
	/*Do([this](SeasonSessionFacade* facade)
		{	
			facade->TransferBuyDoc(m_docId,m_dstHerdId);
		});	
	Clear();*/
}

void TransferBuyDocTransaction::Abort()
{
	//Clear();
}






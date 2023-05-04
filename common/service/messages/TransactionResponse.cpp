#include "stdafx.h"
#include "TransactionResponse.h"
#include <boost/any.hpp>
#include <SF/any.hpp>

TransactionResponse& TransactionResponse::WithResult(boost::any& result)
{
	m_result = result;
	return *this;
}

const boost::any& TransactionResponse::GetResult() const
{
	return m_result;
}
	
void TransactionResponse::serialize(SF::Archive &archive)
{	
	archive & m_result;
}

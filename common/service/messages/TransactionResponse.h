#pragma once 

#include <boost/shared_ptr.hpp>
#include <SF/Archive.hpp>

class TransactionResponse
{
public:
	TransactionResponse& WithResult(boost::any& result);
	const boost::any& GetResult() const;
	
	void serialize(SF::Archive &archive);
private:
	boost::any m_result;

};

typedef boost::shared_ptr<TransactionResponse> TransactionResponseObj;

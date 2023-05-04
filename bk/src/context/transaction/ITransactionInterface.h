#ifndef __ITRANSACTIONINTERFACE_H__
#define __ITRANSACTIONINTERFACE_H__

#include <data\runtime\OperationArguments.h>
#include <stdint.h>

class ITransactionInterface
{
public:
	virtual boost::any Execute(uint32_t operation,const OperationArguments& args) = 0;
	virtual boost::any Execute(uint32_t operation) = 0;
	virtual void Commit() = 0;
	virtual void Rollback() = 0;
};


#endif
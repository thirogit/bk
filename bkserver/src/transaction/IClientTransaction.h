#pragma once

#include <data/IIdable.h>
#include <data/runtime/OperationArguments.h>

class IClientTransaction : public IIdable
{
public:
	virtual boost::any ExecuteOperation(uint16_t operation,const OperationArguments& args) = 0;
	virtual void Commit()  = 0;
	virtual void Abort() = 0;
	virtual ~IClientTransaction() = 0;	
};

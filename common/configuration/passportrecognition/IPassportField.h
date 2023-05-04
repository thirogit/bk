#ifndef __IPASSPORTFIELD_H__
#define __IPASSPORTFIELD_H__

#include "PassportFieldDescriptor.h"
#include "PassportFieldMetrics.h"

class IPassportField
{
public:
	virtual const PassportFieldMetrics& GetFieldMetrics() const = 0;
	virtual const PassportFieldDescriptor* GetDescriptor() const = 0;
};

#endif 
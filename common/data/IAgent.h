#ifndef __IAGENT_H__
#define __IAGENT_H__

#include "IIdable.h"
#include <string>

class IAgent : public IIdable
{
public:
	virtual const std::wstring& GetAgentCode() const = 0;
	virtual const std::wstring& GetAgentName() const = 0;
	virtual const std::wstring& GetPlateNo() const = 0;
};




#endif
#ifndef __AGENT_H__
#define __AGENT_H__

#include "../IAgent.h"

class SerializableAgent;

class Agent : public IAgent
{
public:
	Agent(uint32_t Id);
	Agent(const IAgent* src);
	
	virtual void CopyFrom(const IAgent* src);
	uint32_t GetId() const;
	const std::wstring& GetAgentCode() const;
	const std::wstring& GetAgentName() const;
	const std::wstring& GetPlateNo() const;

	void SetAgentCode(const std::wstring& sAgentCode);
	void SetAgentName(const std::wstring& sAgentName);
	void SetPlateNo(const std::wstring& sPlateNo);
	
protected:
	uint32_t m_agentId;
	std::wstring m_agentcode;
	std::wstring m_agentname;
	std::wstring m_plateNo;

	friend class SerializableAgent;
};




#endif
#ifndef __DEDUCTIONDEFINITIONREQUEST_H__
#define __DEDUCTIONDEFINITIONREQUEST_H__

#include <boost/shared_ptr.hpp>
#include <configuration/deductions/DeductionDefinition.h>
#include <SF/Archive.hpp>

class DeductionDefinitionRequest
{
public:
	DeductionDefinitionRequest();
	~DeductionDefinitionRequest();

	IDeductionDefinition* GetDefinition() const;
	DeductionDefinition* CreateDefinition(uint32_t definitionId);

	void serialize(SF::Archive &archive);
	
private:
	DeductionDefinition* m_pDefinition;
};

typedef boost::shared_ptr<DeductionDefinitionRequest> DeductionDefinitionRequestObj;

#endif

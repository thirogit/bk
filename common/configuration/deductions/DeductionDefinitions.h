#pragma once 

#include "DeductionDefinition.h"
#include <arrays/Enumerator.h>
#include <vector>

class DeductionDefinitions
{
public:

	typedef std::vector<DeductionDefinition*> deduction_definition_vector;
	typedef deduction_definition_vector::const_iterator const_iterator;
	typedef deduction_definition_vector::iterator iterator;
	
	DeductionDefinitions();
	DeductionDefinitions(const DeductionDefinitions& src);
	DeductionDefinitions(DeductionDefinitions&& src);
	~DeductionDefinitions();
	
	void Add(const IDeductionDefinition* definition);
	PtrEnumerator<IDeductionDefinition> Enum();

	const_iterator begin() const;
	const_iterator end() const;

	iterator begin();
	iterator end();

	size_t size() const;
	
private:
	deduction_definition_vector m_definitions;
};






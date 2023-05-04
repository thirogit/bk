#pragma once

#include "IntegrityProblem.h"
#include <vector>
#include <arrays/Enumerator.h>
#include <data/repo/IPurchase.h>
#include "../../SeasonSessionFacade.h"
#include <boost/uuid/uuid_generators.hpp>
#include <data/runtime/ItemId.h>

class IntegrityProblemIdentifier 
{
public:
	IntegrityProblemIdentifier(const IPurchase* purchase);
	~IntegrityProblemIdentifier();
	void IdentifyProblems(SeasonSessionFacade* facade);
	PtrEnumerator<IntegrityProblem> EnumProblems();
	size_t GetProblemCount() const;
private:
	std::vector<IntegrityProblem*> m_problems;
	const IPurchase* m_Purchase;

	boost::uuids::basic_random_generator<boost::mt19937> m_IdGenerator;
};

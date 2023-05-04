#pragma once

#include <data/runtime/IItem.h>

class ImportProblem : public IItem
{
public:
	ImportProblem(ItemId id);
	virtual ItemId GetId() const;
	virtual ImportProblem* Clone() const = 0;	
	virtual std::wstring DescribeProblem() const = 0;
private:
	ItemId m_id;
	
};

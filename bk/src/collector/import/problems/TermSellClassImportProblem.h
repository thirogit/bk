#pragma once

#include "ImportProblem.h"

class TermSellClassImportProblem : public ImportProblem
{
public:

	TermSellClassImportProblem(ItemId id, const std::wstring& classCd);	
	const std::wstring& GetClassCd() const;
	virtual ImportProblem* Clone() const;
	virtual std::wstring DescribeProblem() const;
private:
	std::wstring m_classCd;
};


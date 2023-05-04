#pragma once

#include "Solution.h"
#include <data/runtime/SolutionType.h>
#include <string>
#include <data/repo/PurchaseHent.h>
#include <data/repo/PurchaseObjPath.h>

class InvoiceHentIntegritySolution : public Solution
{
public:
	InvoiceHentIntegritySolution(SolutionType type, const PurchaseObjPath& path, const IPurchaseHent* problemHent, const IPurchaseHent* fixHent);
	virtual bool IsSolving(IntegrityProblem* problem);
	virtual void Accept(SolutionVisitor* visitor);
	virtual Solution* Generalize() const;
	virtual bool IsImmediate() const;
	
	SolutionType GetType() const;
	IPurchaseHent* GetProblemHent() const;
	IPurchaseHent* GetFixHent() const;
	const PurchaseObjPath& GetPath() const;


	~InvoiceHentIntegritySolution();
	
private:
	SolutionType m_type;
	PurchaseObjPath m_path;
	PurchaseHent* m_problemHent;
	PurchaseHent* m_fixHent;
};


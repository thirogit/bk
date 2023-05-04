#pragma once

#include "SolutionVisitor.h"
#include <data/repo/Purchase.h>

class SolutionApplyingVisitor : public SolutionVisitor
{
public:
	SolutionApplyingVisitor(Purchase* purchaseToFix);

	virtual void visit(AgentIntegritySolution* solution);
	virtual void visit(HerdIntegritySolution* solution);
	virtual void visit(FstOwnerHentIntegritySolution* solution);
	virtual void visit(InvoiceHentIntegritySolution* solution);
	virtual void visit(ClassIntegritySolution* solution);
	virtual void visit(StockIntegritySolution* solution);
	virtual void visit(InStockCowDuplicationSolution* solution);

private:
	Purchase* m_purchaseToFix;
};

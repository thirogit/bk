#pragma once

#include "ProblemVisitor.h"

class EmptyProblemVisitor : public ProblemVisitor
{
public:
	virtual void visit(AgentIntegrityProblem* problem) {};
	virtual void visit(HerdIntegrityProblem* problem) {};
	virtual void visit(FirstOwnerHentIntegrityProblem* problem) {};
	virtual void visit(InvoiceHentIntegrityProblem* problem) {};
	virtual void visit(ClassIntegrityProblem* problem) {};
	virtual void visit(StockIntegrityProblem* problem) {};
	virtual void visit(InStockCowDuplicationProblem* problem) {};
};

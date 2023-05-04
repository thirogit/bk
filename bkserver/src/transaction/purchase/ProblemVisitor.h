#pragma once


class IntegrityProblem;
class AgentIntegrityProblem;
class HerdIntegrityProblem;
class FirstOwnerHentIntegrityProblem;
class InvoiceHentIntegrityProblem;
class ClassIntegrityProblem;
class StockIntegrityProblem;
class InStockCowDuplicationProblem;

class ProblemVisitor
{
public:
	virtual void visit(AgentIntegrityProblem* problem) = 0;
	virtual void visit(HerdIntegrityProblem* problem) = 0;
	virtual void visit(FirstOwnerHentIntegrityProblem* problem) = 0;
	virtual void visit(InvoiceHentIntegrityProblem* problem) = 0;
	virtual void visit(ClassIntegrityProblem* problem) = 0;
	virtual void visit(InStockCowDuplicationProblem* problem) = 0;
	virtual void visit(StockIntegrityProblem* problem) = 0;

};


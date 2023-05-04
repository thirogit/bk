#pragma once

class AgentIntegritySolution;
class HerdIntegritySolution;
class FstOwnerHentIntegritySolution;
class InvoiceHentIntegritySolution;
class ClassIntegritySolution;
class StockIntegritySolution;
class InStockCowDuplicationSolution;

class SolutionVisitor
{
public:
	virtual void visit(AgentIntegritySolution* solution) = 0;
	virtual void visit(HerdIntegritySolution* solution) = 0;
	virtual void visit(FstOwnerHentIntegritySolution* solution) = 0;
	virtual void visit(InvoiceHentIntegritySolution* solution) = 0;
	virtual void visit(ClassIntegritySolution* solution) = 0;
	virtual void visit(StockIntegritySolution* solution) = 0;
	virtual void visit(InStockCowDuplicationSolution* solution) = 0;	
};

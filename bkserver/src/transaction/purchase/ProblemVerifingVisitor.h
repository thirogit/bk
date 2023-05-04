#pragma once

#include "ProblemVisitor.h"
#include "../../SeasonSessionFacade.h"

class ProblemVerifingVisitor : public ProblemVisitor
{
public:
	ProblemVerifingVisitor(SeasonSessionFacade* facade);
	virtual void visit(AgentIntegrityProblem* problem);
	virtual void visit(HerdIntegrityProblem* problem);
	virtual void visit(FirstOwnerHentIntegrityProblem* problem);
	virtual void visit(InvoiceHentIntegrityProblem* problem);
	virtual void visit(ClassIntegrityProblem* problem);
	virtual void visit(StockIntegrityProblem* problem);
	virtual void visit(InStockCowDuplicationProblem* problem);
	bool IsVerified() const;
private:
	SeasonSessionFacade* m_facade;
	bool m_verified;
};

#pragma once

class MainClassImportProblem;

class ProblemVisitor 
{
public:
	virtual void Visit(MainClassImportProblem* problem) = 0;
	
};

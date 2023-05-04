#pragma once

class SolutionVisitor;
class IntegrityProblem;

class Solution
{
public:
	virtual bool IsSolving(IntegrityProblem* problem) = 0;
	virtual void Accept(SolutionVisitor* visitor) = 0;
	virtual Solution* Generalize() const = 0;
	virtual bool IsImmediate() const = 0;
};


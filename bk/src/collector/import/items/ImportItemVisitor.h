#pragma once

class InDocImportItemVisitorFacade;
class CowImportItemVisitorFacade;
class OutDocImportItemVisitorFacade;

class ImportItemVisitor 
{
public:
	virtual void Visit(InDocImportItemVisitorFacade* item) = 0;
	virtual void Visit(CowImportItemVisitorFacade* item) = 0;
	virtual void Visit(OutDocImportItemVisitorFacade* item) = 0;
};

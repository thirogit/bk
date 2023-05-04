#ifndef __PURCHASETRAVERSER_H__
#define __PURCHASETRAVERSER_H__

#include <data\repo\IPurchase.h>

class PurchaseTraverser
{
public:
	PurchaseTraverser(IPurchase* purchase);
private:
	void traverse(PurchaseTraverserVisitor* visitor);
private:
	IPurchase* m_purchase;
};

#endif
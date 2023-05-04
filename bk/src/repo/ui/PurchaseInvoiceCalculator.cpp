#include "stdafx.h"
#include "PurchaseInvoiceCalculator.h"
#include <arrays\Enumerator.h>

PurchaseInvoiceCalculator::PurchaseInvoiceCalculator(IPurchaseInvoice* invoice)
{
	m_invoice = invoice;
}
	
int PurchaseInvoiceCalculator::GetCowCount()
{
	return m_invoice->GetCowCount();
}

Decimal PurchaseInvoiceCalculator::GetTotalWeight()
{
	Decimal totalWeight(0L);
	PtrEnumerator<IPurchaseCow> cowsEnum = m_invoice->EnumCows();
	while (cowsEnum.hasNext())
	{
		totalWeight += (*cowsEnum)->GetWeight();
		cowsEnum.advance();
	}

	return totalWeight;
}

Decimal PurchaseInvoiceCalculator::GetTotalNet()
{
	Decimal totalNet(0L);
	PtrEnumerator<IPurchaseCow> cowsEnum = m_invoice->EnumCows();
	while (cowsEnum.hasNext())
	{
		totalNet += (*cowsEnum)->GetPrice();
		cowsEnum.advance();
	}

	return totalNet;
}

Decimal PurchaseInvoiceCalculator::GetTotalGross()
{
	Decimal totalNet = GetTotalNet();
	return totalNet + totalNet*m_invoice->GetVatRate();
}

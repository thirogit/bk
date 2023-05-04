#include "stdafx.h"
#include "PurchaseTraverser.h"
#include <list>
#include <boost/foreach.hpp>
#include <arrays/DeepDelete.h>

PurchaseTraverser::path_element_value::path_element_value() : purchase(NULL),invoice(NULL),cow(NULL)
{
}
		

PurchaseTraverser::path_element_extractor::path_element_extractor(const std::wstring& elementId) : m_id(elementId)
{
}

const std::wstring& PurchaseTraverser::path_element_extractor::get_id() const
{
	return m_id;
}


PurchaseTraverser::purchase_element_extractor::purchase_element_extractor(const std::wstring& elementId) : path_element_extractor(elementId)
{
}

PurchaseTraverser::path_element_value PurchaseTraverser::purchase_element_extractor::extract(const path_element_value& value)
{
	path_element_value result;
	Purchase* purchase = value.purchase;
	if(purchase)
	{
		if(purchase->GetId() == get_id())
		{
			result.purchase = purchase;
		}
	}	
	return result;
}


PurchaseTraverser::invoice_element_extractor::invoice_element_extractor(const std::wstring& elementId) : path_element_extractor(elementId)
{
}

PurchaseTraverser::path_element_value PurchaseTraverser::invoice_element_extractor::extract(const path_element_value& value)
{
	path_element_value result;
	Purchase* purchase = value.purchase;
	if(purchase)
	{
		result.invoice = purchase->GetInvoice(get_id());
	}	
	return result;
}

PurchaseTraverser::cow_element_extractor::cow_element_extractor(const std::wstring& elementId) : path_element_extractor(elementId)
{
}

PurchaseTraverser::path_element_value PurchaseTraverser::cow_element_extractor::extract(const path_element_value& value)
{
	path_element_value result;
	PurchaseInvoice* purchaseInvoice = value.invoice;
	if(purchaseInvoice)
	{
		result.cow = purchaseInvoice->GetCow(get_id());
	}	
	return result;
}

PurchaseTraverser::PurchaseTraverser(Purchase* purchase) : m_purchase(purchase)
{
}

PurchaseTraverser::path_element_value PurchaseTraverser::Extract(const PurchaseObjPath& path)
{
	std::wstring pathstring = path.ToString();
	std::list<path_element_extractor*> extractors;
	PathEnumerator enumerator = path.EnumPath();
	while (enumerator.hasNext())
	{
		PurchaseObjPathElement element = (*enumerator);
		switch (element.GetType())
		{
		case PurchaseObjType_Root:
			extractors.push_back(new purchase_element_extractor(element.GetId()));
			break;
		case PurchaseObjType_Invoice:
			extractors.push_back(new invoice_element_extractor(element.GetId()));
			break;
		case PurchaseObjType_Cow:
			extractors.push_back(new cow_element_extractor(element.GetId()));
			break;
		}
		enumerator++;
	}

	path_element_value value;
	value.purchase = m_purchase;
	BOOST_FOREACH(path_element_extractor* extractor, extractors)
	{
		value = extractor->extract(value);
	}
	DeepDelete(extractors);

	return value;
}

PurchaseCow* PurchaseTraverser::GetCow(const PurchaseObjPath& path)
{
	return Extract(path).cow;

}

PurchaseInvoice* PurchaseTraverser::GetInvoice(const PurchaseObjPath& path)
{
	return Extract(path).invoice;
}

std::vector<PurchaseCow*> PurchaseTraverser::GetCows()
{
	std::vector<PurchaseCow*> result;
	for (int i = 0, invoicecount = m_purchase->GetInvoiceCount(); i < invoicecount; i++)
	{
		PurchaseInvoice* invoice = m_purchase->GetInvoiceAt(i);

		for (int j = 0, cowcount = invoice->GetCowCount(); j < cowcount; j++)
		{
			result.push_back(invoice->GetCowAt(j));
		}
	}	
	return std::move(result);
}

std::vector<PurchaseInvoice*> PurchaseTraverser::GetInvoices()
{
	std::vector<PurchaseInvoice*> result;
	for (int i = 0, count = m_purchase->GetInvoiceCount(); i < count; i++)
	{
		result.push_back(m_purchase->GetInvoiceAt(i));
	}
	return std::move(result);
}

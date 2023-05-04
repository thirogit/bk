#pragma once

#include <data/repo/Purchase.h>
#include <data/repo/PurchaseObjPath.h>

class PurchaseTraverser
{
	struct path_element_value
	{
		path_element_value();
		
		Purchase* purchase;
		PurchaseInvoice* invoice;
		PurchaseCow* cow;
	};

	struct path_element_extractor
	{
		path_element_extractor(const std::wstring& elementId);
		const std::wstring& get_id() const;
		
		virtual path_element_value extract(const path_element_value& value) = 0;
		
	private:
		std::wstring m_id;
	};
	
	struct purchase_element_extractor : public path_element_extractor
	{		
		purchase_element_extractor(const std::wstring& elementId);
		virtual path_element_value extract(const path_element_value& value);
	};
	
	struct invoice_element_extractor : public path_element_extractor
	{
		invoice_element_extractor(const std::wstring& elementId);
		virtual path_element_value extract(const path_element_value& value);
	};
	
	struct cow_element_extractor : public path_element_extractor
	{
		cow_element_extractor(const std::wstring& elementId);
		virtual path_element_value extract(const path_element_value& value);
	};
	
public:
	PurchaseTraverser(Purchase* purchase);
	PurchaseCow* GetCow(const PurchaseObjPath& path);
	PurchaseInvoice* GetInvoice(const PurchaseObjPath& path);
	std::vector<PurchaseCow*> GetCows();
	std::vector<PurchaseInvoice*> GetInvoices();
private:
	path_element_value Extract(const PurchaseObjPath& path);
private:
	Purchase* m_purchase;
};


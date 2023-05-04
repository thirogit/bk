#include "stdafx.h"
#include "InvoiceScopes.h"

namespace InvoiceScopes
{
	InvoiceScope FromChar(wchar_t cInvoiceScope)
	{
		if (InvoiceScope_Cows == (InvoiceScope)cInvoiceScope)
			return InvoiceScope_Cows;
		if (InvoiceScope_Docs == (InvoiceScope)cInvoiceScope)
			return InvoiceScope_Docs;

		return InvoiceScope_None;
	}

	static const wchar_t* cows_invoice_scope = L"cows";
	static const wchar_t* docs_invoice_scope = L"docs";

	InvoiceScope FromString(const std::wstring sInvoiceScope)
	{
		if (sInvoiceScope.length() == 1)
		{
			return FromChar(sInvoiceScope[0]);
		}
		else
		{
			if (cows_invoice_scope == sInvoiceScope)
				return InvoiceScope_Cows;
			if (docs_invoice_scope == sInvoiceScope)
				return InvoiceScope_Docs;
		}
		return InvoiceScope_None;

	}

	std::wstring ToString(InvoiceScope type)
	{
		switch (type)
		{
		case InvoiceScope_Docs:
			return docs_invoice_scope;
		case InvoiceScope_Cows:
			return cows_invoice_scope;
		}

		return L"";

	}
}
#include "stdafx.h"
#include "InvoiceTypes.h"
#include <boost/algorithm/string.hpp>

namespace InvoiceTypes
{
	InvoiceType FromChar(wchar_t cInvoiceType)
	{
		if (InvoiceType_Regular == (InvoiceType)cInvoiceType)
			return InvoiceType_Regular;
		if (InvoiceType_Lump == (InvoiceType)cInvoiceType)
			return InvoiceType_Lump;

		return InvoiceType_None;
	}

	static const wchar_t* regular_invoice_type = L"regular";
	static const wchar_t* lump_invoice_type = L"lump";

	InvoiceType FromString(const std::wstring sInvoiceType)
	{
		if (sInvoiceType.length() == 1)
		{
			return FromChar(sInvoiceType[0]);
		}
		else
		{
			if (boost::iequals(regular_invoice_type , sInvoiceType))
				return InvoiceType_Regular;
			if (boost::iequals(lump_invoice_type , sInvoiceType))
				return InvoiceType_Lump;
		}
		return InvoiceType_None;

	}

	std::wstring ToString(InvoiceType type)
	{
		switch (type)
		{
		case InvoiceType_Regular:
			return regular_invoice_type;
		case InvoiceType_Lump:
			return lump_invoice_type;
		}

		return L"";

	}
}
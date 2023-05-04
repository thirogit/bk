#include "stdafx.h"
#include "InvoiceDeductions.h"
#include "../../arrays/DeepDelete.h"
#include <boost/foreach.hpp>

InvoiceDeductions::InvoiceDeductions()
{
}

InvoiceDeductions::~InvoiceDeductions()
{
	RemoveAll();
}

count_t InvoiceDeductions::GetCount() const
{
	return m_Deductions.size();
}

PtrEnumerator<IInvoiceDeduction> InvoiceDeductions::EnumDeductions() const
{
	return PtrEnumerator<IInvoiceDeduction>(m_Deductions);
}

PtrEnumerator<InvoiceDeduction> InvoiceDeductions::EnumDeductions()
{
	return PtrEnumerator<InvoiceDeduction>(m_Deductions);
}

InvoiceDeduction* InvoiceDeductions::GetDeductionAt(int index) const
{
	return m_Deductions.at(index);
}

void InvoiceDeductions::RemoveAt(int index)
{
	m_Deductions.erase(m_Deductions.begin() + index);
}

void InvoiceDeductions::RemoveAll()
{
	DeepDelete(m_Deductions);
	m_Deductions.clear();
}

InvoiceDeduction* InvoiceDeductions::AddDeduction(const IDeductionDefinition* definition)
{
	InvoiceDeduction* newDeduction = new InvoiceDeduction();
	newDeduction->SetCode(definition->GetCode());
	newDeduction->SetFraction(definition->GetFraction());
	newDeduction->SetReason(definition->GetReason());
	newDeduction->SetIsEnabled(definition->GetIsEnabledByDefault());
	m_Deductions.push_back(newDeduction);
	return newDeduction;
}

InvoiceDeduction* InvoiceDeductions::AddDeduction(const IInvoiceDeduction* deduction)
{
	InvoiceDeduction* newDeduction = new InvoiceDeduction();
	newDeduction->CopyFrom(deduction);
	AppendDeduction(newDeduction);
	return newDeduction;
}


void InvoiceDeductions::AppendDeduction(InvoiceDeduction* definition)
{
	m_Deductions.push_back(definition);
}


InvoiceDeductions InvoiceDeductions::FromJson(const json::value& value)
{
	InvoiceDeductions result;
	json::array jsonDeductions = value.as_array();

	BOOST_FOREACH(json::value jsonDeduction, jsonDeductions)
	{
		result.m_Deductions.push_back(new InvoiceDeduction(InvoiceDeduction::FromJson(jsonDeduction)));
	}
	return std::move(result);
}

json::value InvoiceDeductions::ToJson() const
{
	std::vector<json::value> jsonDeductions;

	BOOST_FOREACH(InvoiceDeduction* deduction, m_Deductions)
	{
		jsonDeductions.push_back(deduction->ToJson());
	}

	return json::value::array(jsonDeductions);

}

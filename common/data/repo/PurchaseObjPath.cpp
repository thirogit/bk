#include "stdafx.h"
#include "PurchaseObjPath.h"
#include "../IIdable.h"
#include <string/TextUtils.h>

PurchaseObjPathElement::PurchaseObjPathElement()
{
}

PurchaseObjPathElement::PurchaseObjPathElement(PurchaseObjType type, const std::wstring& id) : m_type(type), m_id(id)
{
}

PurchaseObjPathElement::PurchaseObjPathElement(const PurchaseObjPathElement& src) : m_type(src.m_type), m_id(src.m_id)
{
}

PurchaseObjPathElement& PurchaseObjPathElement::operator=(const PurchaseObjPathElement& right)
{
	m_type = right.m_type;
	m_id = right.m_id;
	return *this;
}

PurchaseObjType PurchaseObjPathElement::GetType() const
{
	return m_type;
}

const std::wstring& PurchaseObjPathElement::GetId() const
{
	return m_id;
}

//#######################################################################################

PathEnumerator::PathEnumerator(PurchaseObjPath* path) : m_path(path)
{
}

PathEnumerator::PathEnumerator(const PathEnumerator& src)
{
	m_path = src.m_path;
}

PathEnumerator& PathEnumerator::operator=(const PathEnumerator& right)
{
	m_path = right.m_path;
	return *this;
}


PathEnumerator& PathEnumerator::operator++()
{
	if (m_path->m_Value.is_initialized())
	{
		m_path = m_path->m_Value->m_child;
	}
	else
	{
		m_path = NULL;
	}
	
	return *this;
}

PathEnumerator PathEnumerator::operator++(int)
{
	PathEnumerator tmp(m_path);
	
	this->operator++();

	return tmp;
}

PurchaseObjPathElement PathEnumerator::operator*()
{
	return m_path->m_Value.get_ptr()->m_element;
}

bool PathEnumerator::hasNext()
{
	return m_path != NULL;
}

PurchaseObjPathElement PathEnumerator::operator->()
{
	return m_path->m_Value.get_ptr()->m_element;
}



//#######################################################################################

PurchaseObjPath::InternalRepresentation::InternalRepresentation() : m_element(PurchaseObjType_None, L"")
{
	m_child = NULL;
}

PurchaseObjPath::InternalRepresentation::InternalRepresentation(const PurchaseObjPath::InternalRepresentation& src)
{
	m_element = src.m_element;
	m_child = NULL;

	if (src.m_child)
	{
		m_child = new PurchaseObjPath(*src.m_child);
	}
}

PurchaseObjPath::InternalRepresentation& PurchaseObjPath::InternalRepresentation::operator= (const PurchaseObjPath::InternalRepresentation& right)
{
	m_element = right.m_element;
	delete m_child;

	if (right.m_child)
	{
		m_child = new PurchaseObjPath(*right.m_child);
	}

	return *this;
}

PurchaseObjPath::InternalRepresentation::~InternalRepresentation()
{
	delete m_child;
}

//#######################################################################################

PurchaseObjPath::PurchaseObjPath()
{
	
}

PurchaseObjPath::PurchaseObjPath(PurchaseObjType type, const std::wstring& id)
{
	InternalRepresentation representation;
	representation.m_element = PurchaseObjPathElement(type,id);
	m_Value = representation;

	m_str = ToString();
}

PurchaseObjPath::PurchaseObjPath(const PurchaseObjPath& src)
{
	m_Value = src.m_Value;
	m_str = ToString();
}

PurchaseObjPath& PurchaseObjPath::operator=(const PurchaseObjPath& right)
{
	m_Value = right.m_Value;
	m_str = ToString();
	return *this;
}

PurchaseObjType PurchaseObjPath::GetType() const
{
	if (IsNull())
		return PurchaseObjType_None;

	return m_Value.get().m_element.GetType();

}

std::wstring PurchaseObjPath::GetId() const
{
	if (IsNull())
		return L"";

	return m_Value.get().m_element.GetId();

}

PurchaseObjPath PurchaseObjPath::GetChild() const
{
	if (IsNull())
		return PurchaseObjPath();

	PurchaseObjPath* child = m_Value.get().m_child;
	if (child == NULL)
		return PurchaseObjPath();

	return PurchaseObjPath(*child);
}

bool PurchaseObjPath::HasChild() const
{
	if (IsNull()) return false;

	return m_Value.get().m_child != NULL;
}

std::wstring PurchaseObjPath::ToString() const
{
	std::wstring result;
	if (!IsNull())
	{
		const PurchaseObjPath* element = this;		
		while (element != NULL && !element->IsNull())
		{
			switch (element->GetType())
			{
			case PurchaseObjType_Cow:
				result += L"cow";
				break;
			case PurchaseObjType_Hent:
				result += L"hent";
				break;
			case PurchaseObjType_Invoice:
				result += L"invoice";
				break;
			case PurchaseObjType_Deduction:
				result += L"deduction";
				break;
			}

			result += TextUtils::Format(L"[%s]", element->GetId());
			element = element->m_Value.get_ptr()->m_child;

			if (element != NULL && !element->IsNull())
				result += L"/";
		}
	}
	return result;	
}

PathEnumerator PurchaseObjPath::EnumPath() const
{
	return PathEnumerator(const_cast<PurchaseObjPath*>(this));
}

PurchaseObjPath PurchaseObjPath::operator/(const PurchaseObjPath& right)
{
	if (right.IsNull())
		return *this;

	PurchaseObjPath path(*this);

	PurchaseObjPath* lastElement = &path;
	while (lastElement->m_Value->m_child != NULL)
	{
		lastElement = lastElement->m_Value->m_child;
	}
	lastElement->m_Value->m_child = new PurchaseObjPath(right);

	path.m_str = path.ToString();
	
	return path;
}

void PurchaseObjPath::MakeNull()
{
	m_Value.reset();
	m_str = ToString();
}

bool PurchaseObjPath::IsNull() const
{
	return !m_Value.is_initialized();
}


bool operator==(const PurchaseObjPath& left, const PurchaseObjPath& right)
{
	if (left.IsNull() && right.IsNull())
	{
		return true;
	}

	if (left.IsNull() != right.IsNull())
	{
		return false;
	}

	if (left.GetType() == right.GetType() && left.GetId() == right.GetId())
	{
		bool lHasChild = left.HasChild();
		bool rHasChild = right.HasChild();

		if (!lHasChild && !rHasChild)
		{
			return true;
		}

		if (lHasChild && rHasChild) 
		{
			return left.GetChild() == right.GetChild();
		}
	}

	return false;
}
#ifndef __PURCHASEOBJPATH_H__
#define __PURCHASEOBJPATH_H__

#include "../../types/Nullable.h"
#include <boost/optional.hpp>
#include <stdint.h>

enum PurchaseObjType : wchar_t
{
	PurchaseObjType_Root = L'r',
	PurchaseObjType_Hent = L'h',
	PurchaseObjType_Invoice = L'i',
	PurchaseObjType_Cow = L'c',
	PurchaseObjType_Deduction = L'd',
	PurchaseObjType_None = L'\0'
};

class PurchaseObjPath;

class PurchaseObjPathElement
{
public:
	PurchaseObjPathElement();
	PurchaseObjPathElement(PurchaseObjType type, const std::wstring& id);
	PurchaseObjPathElement(const PurchaseObjPathElement& src);
	PurchaseObjPathElement& operator=(const PurchaseObjPathElement& right);

	PurchaseObjType GetType() const;
	const std::wstring& GetId() const;

private:
	PurchaseObjType m_type;
	std::wstring m_id;
};

class PathEnumerator
{	
	PathEnumerator(PurchaseObjPath* path);
public:		
	
	PathEnumerator(const PathEnumerator& src);
	PathEnumerator& operator=(const PathEnumerator& right);
	
	PathEnumerator& operator++();
	PathEnumerator operator++(int);
	
	PurchaseObjPathElement operator*();
	bool hasNext();
	PurchaseObjPathElement operator->();
	
private:
	PurchaseObjPath* m_path;
	
	friend class PurchaseObjPath;
};

class PurchaseObjPath : public Nullable
{
	struct InternalRepresentation
	{
		InternalRepresentation();
		~InternalRepresentation();
		InternalRepresentation(const InternalRepresentation& src);
		InternalRepresentation& operator=(const InternalRepresentation& right);

		PurchaseObjPathElement m_element;
		PurchaseObjPath* m_child;
	};
	
public:
	PurchaseObjPath();
	
	PurchaseObjPath(PurchaseObjType type, const std::wstring& id);
	PurchaseObjPath(const PurchaseObjPath& src);
	PurchaseObjPath& operator=(const PurchaseObjPath& right);

	PathEnumerator EnumPath() const;
	PurchaseObjType GetType() const;
	std::wstring GetId() const;
	PurchaseObjPathElement Element() const;
	PurchaseObjPath GetChild()  const;
	bool HasChild() const;
	std::wstring ToString() const;
	
	PurchaseObjPath operator/(const PurchaseObjPath& right);
		
	virtual void MakeNull();
	virtual bool IsNull() const;
private:
	boost::optional<InternalRepresentation> m_Value;
	std::wstring m_str;
	friend class PathEnumerator;
};

bool operator==(const PurchaseObjPath& left, const PurchaseObjPath& right);
	

#endif
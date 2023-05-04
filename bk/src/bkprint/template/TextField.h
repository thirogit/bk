#ifndef __TEXTFIELD_H__
#define __TEXTFIELD_H__

#include "Field.h"
#include "TextStyle.h"
#include "Border.h"

namespace bkprint
{

class TextField : public Field
{
public:
	TextField();
	virtual ~TextField();
	
	virtual void Visit(FieldVisitor* visitor);
	virtual Field* Clone();	
	void CopyFrom(const TextField* pSrc);
	virtual void Unmarshal(pugi::xml_node & node);

	void SetText(const std::wstring& sText);
	const std::wstring& GetText() const;

	const Border& GetBorder() const;
	void SetBorder(const Border& border);

	const Color& GetFillColor() const;
	void SetFillColor(const Color& color);

	const TextStyle& GetOwnStyle() const;
	void SetOwnStyle(const TextStyle& ownstyle);

	const std::wstring& GetRefStyle() const;
	void SetRefStyle(const std::wstring& refStyleId);
	
protected:
	std::wstring m_text;
	Border m_border;
	Color m_fillColor;
	TextStyle m_ownStyle;
	std::wstring m_refStyleId;
};

}

#endif
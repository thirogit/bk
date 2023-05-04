#include "stdafx.h"
#include "ExpandableTemplate.h"
#include <set>
#include <boost\foreach.hpp>
#include <arrays/DeepDelete.h>
#include "FieldFactory.h"
#include "util\PaperTypeUtil.h"
#include "util\PaperOrientationUtil.h"
#include "unmarshaler\TypesUnmarshaler.h"

namespace bkprint
{
	ExpandableTemplate::ExpandableTemplate()
	{
		Init0();
	}

	void ExpandableTemplate::Init0()
	{	
		m_Orientation = PaperOrientation_Portrait;
		m_PaperType = PaperType_A4;
	}

	ExpandableTemplate::ExpandableTemplate(const ExpandableTemplate& src)
	{
		

	}

	ExpandableTemplate::ExpandableTemplate(ExpandableTemplate&& src)
	{
		

	}


	ExpandableTemplate::~ExpandableTemplate()
	{
		DestroyTemplate();
	}

	void ExpandableTemplate::Unmarshal(pugi::xml_node & node)
	{
		FieldFactory fieldsFactory;
		
		m_Orientation = PaperOrientationUtil::FromString(node.attribute(L"orientation").as_string());

		m_PaperType = PaperTypeUtil::FromString(node.attribute(L"paper").as_string());
		
		pugi::xml_node stylesnode = node.child(L"styles");
		auto styleIt = stylesnode.begin();

		TypesUnmarshaler unmarshaler;
		while (styleIt != stylesnode.end())
		{
			pugi::xml_node stylenode = *styleIt;
			std::wstring styleid = stylenode.attribute(L"id").as_string();
			TextStyle style = unmarshaler.UnmarshalTextStyle(stylenode);
			m_styles.add_style(styleid,style);
			styleIt++;
		}

		pugi::xml_node documentnode = node.child(L"document");

		pugi::xml_node pageheader = documentnode.child(L"pageheader");
		m_pageHeader.Unmarshal(pageheader);
		
		pugi::xml_node documentheader = documentnode.child(L"documentheader");
		m_documentHeader.Unmarshal(documentheader);


		pugi::xml_node bandsnode = documentnode.child(L"bands");
		auto bandIt = bandsnode.begin();

	
		while (bandIt != bandsnode.end())
		{
			Band* band = new Band(bandIt->attribute(L"name").as_string());
			band->Unmarshal(*bandIt);
			m_Bands.push_back(band);
			bandIt++;
		}

		pugi::xml_node documentfooter = documentnode.child(L"documentfooter");
		if (documentfooter)
		{
			m_documentFooter.Unmarshal(documentfooter);
		}
		

		pugi::xml_node pagefooter = documentnode.child(L"pagefooter");
		if (pagefooter)
		{
			m_pageFooter.Unmarshal(pagefooter);
		}
		

	}

	void ExpandableTemplate::DestroyTemplate()
	{
		m_styles.clear();		
		DeepDelete(m_Bands);
		m_Bands.clear();

		m_documentHeader.Clear();
		m_documentFooter.Clear();
		m_pageHeader.Clear();
		m_pageFooter.Clear();
	}

	void ExpandableTemplate::Visit(TemplateVisitor* visitor)
	{
		visitor->Accept(this);
	}


	FieldsContainer& ExpandableTemplate::HeaderFields()
	{
		return m_documentHeader;
	}

	PtrEnumerator<Band> ExpandableTemplate::EnumBands()
	{
		return PtrEnumerator<Band>(m_Bands);
	}

	DocumentFooter& ExpandableTemplate::FooterFields()
	{
		return m_documentFooter;
	}

	FieldsContainer& ExpandableTemplate::PageFooterFields()
	{
		return m_pageFooter;
	}

	PageHeader& ExpandableTemplate::PageHeaderFields()
	{
		return m_pageHeader;
	}
	

	TextStyle ExpandableTemplate::FindStyle(const std::wstring& styleId)
	{
		return m_styles.find_style(styleId);
	}

	PaperOrientation ExpandableTemplate::GetOrientation() const
	{
		return m_Orientation;
	}

	PaperType ExpandableTemplate::GetPaperType() const
	{
		return m_PaperType;
	}
}
#include "stdafx.h"
#include "FixedTemplate.h"
#include <set>
#include <boost\foreach.hpp>
#include <arrays/DeepDelete.h>
#include "FieldFactory.h"
#include "util\PaperTypeUtil.h"
#include "util\PaperOrientationUtil.h"
#include "unmarshaler\TypesUnmarshaler.h"
#include "unmarshaler\TemplateUnmarshalException.h"
#include <string\TextUtils.h>

namespace bkprint
{
	FixedTemplate::FixedTemplate()
	{
		Init0();
	}

	void FixedTemplate::Init0()
	{	
		m_Orientation = PaperOrientation_Portrait;
		m_PaperType = PaperType_A4;
	}
	
	FixedTemplate::FixedTemplate(const FixedTemplate& src)
	{
		

	}

	FixedTemplate::FixedTemplate(FixedTemplate&& src)
	{
		

	}


	FixedTemplate::~FixedTemplate()
	{
		DestroyTemplate();
	}

	void FixedTemplate::DestroyTemplate()
	{
		m_styles.clear();
		BOOST_FOREACH(TemplatePage* page, m_Pages)
		{
			page->fields->Clear();
			delete page->fields;
			delete page;
		}
		m_Pages.clear();
	}

	void FixedTemplate::Visit(TemplateVisitor* visitor)
	{
		visitor->Accept(this);
	}

	int FixedTemplate::GetPageCount() const
	{
		return m_Pages.size();
	}

	void FixedTemplate::Unmarshal(pugi::xml_node & node)
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
			m_styles.add_style(styleid, style);
			styleIt++;
		}

		pugi::xml_node pagesnode = node.child(L"pages");

		auto pageIt = pagesnode.begin();
		while (pageIt != pagesnode.end())
		{
			std::wstring pageName = pageIt->attribute(L"name").as_string();


			if (GetPage(pageName) != NULL)
			{
				DestroyTemplate();
				BOOST_THROW_EXCEPTION(TemplateUnmarshalException(TextUtils::Format(L"duplicated page with name %s", pageName)));
			}

			TemplatePage* page = new TemplatePage();
			page->name = pageName;
			page->fields = new FieldsContainer();
			page->fields->Unmarshal(*pageIt);
			

			m_Pages.push_back(page);
			pageIt++;
		}
	}

	FieldsContainer* FixedTemplate::GetPage(int index)
	{
		return m_Pages[index]->fields;
	}

	FieldsContainer* FixedTemplate::GetPage(const std::wstring& pageName)
	{
		const boost::multi_index::index<PagesIndex, index_tag_name>::type& indexByName = m_Pages.get<index_tag_name>();

		auto pageIt = indexByName.find(pageName);

		if (pageIt != indexByName.end())
			return (*pageIt)->fields;

		return NULL;
	}
	
	TextStyle FixedTemplate::FindStyle(const std::wstring& styleId)
	{
		return m_styles.find_style(styleId);
	}

	PaperOrientation FixedTemplate::GetOrientation() const
	{
		return m_Orientation;
	}

	PaperType FixedTemplate::GetPaperType() const
	{
		return m_PaperType;
	}
}
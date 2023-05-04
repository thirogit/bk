#ifndef __FIXEDTEMPLATE_H__
#define __FIXEDTEMPLATE_H__

#include <string>
#include <vector>
#include <pugixml.hpp>

#include <arrays/Enumerator.h>
#include "Field.h"
#include "Band.h"
#include "TextStyleIndex.h"
#include "FieldsContainer.h"
#include "../types/PaperOrientation.h"
#include "../types/PaperType.h"
#include "Template.h"
#include "DocumentFooter.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>

namespace bkprint
{
	class TemplateUnmarshaller;

	class FixedTemplate : public Template
	{	
		class TemplatePage
		{
		public:
			FieldsContainer* fields;
			std::wstring name;
		};


		struct index_tag_name{};

		typedef boost::multi_index_container
			<
			TemplatePage*,
			boost::multi_index::indexed_by<
			boost::multi_index::random_access<>,
			boost::multi_index::ordered_unique < boost::multi_index::tag<index_tag_name>, boost::multi_index::member<TemplatePage, std::wstring, &TemplatePage::name> >			
			>
			> PagesIndex;

	private:		
		void DestroyTemplate();
		void Init0();
	public:
		FixedTemplate();		
		FixedTemplate(const FixedTemplate& src);
		FixedTemplate(FixedTemplate&& src);
		virtual ~FixedTemplate();
		
		virtual void Visit(TemplateVisitor* visitor);
		void Unmarshal(pugi::xml_node & node);
		
		TextStyle FindStyle(const std::wstring& styleId);
		
		int GetPageCount() const;
		FieldsContainer* GetPage(int index);
		FieldsContainer* GetPage(const std::wstring& pageName);		
		PaperOrientation GetOrientation() const;
		PaperType GetPaperType() const;
		
	private:
		TextStyleIndex m_styles;		
		PagesIndex m_Pages;
		PaperOrientation m_Orientation;
		PaperType m_PaperType;

		friend class TemplateUnmarshaller;
	};

}
#endif
#ifndef __EXPANDABLETEMPLATE_H__
#define __EXPANDABLETEMPLATE_H__

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
#include "PageHeader.h"

namespace bkprint
{
	class ExpandableTemplate : public Template
	{	
	private:		
		void DestroyTemplate();
		void Init0();
	public:
		ExpandableTemplate();		
		ExpandableTemplate(const ExpandableTemplate& src);
		ExpandableTemplate(ExpandableTemplate&& src);
		virtual ~ExpandableTemplate();
				
		virtual void Visit(TemplateVisitor* visitor);
		void Unmarshal(pugi::xml_node & node);
		TextStyle FindStyle(const std::wstring& styleId);
		FieldsContainer& HeaderFields();
		PtrEnumerator<Band> EnumBands();
		DocumentFooter& FooterFields();
		PageHeader& PageHeaderFields();
		FieldsContainer& PageFooterFields();
		PaperOrientation GetOrientation() const;
		PaperType GetPaperType() const;
		
	private:
		TextStyleIndex m_styles;
		FieldsContainer m_documentHeader;
		std::vector<Band*> m_Bands;
		DocumentFooter m_documentFooter;
		PageHeader m_pageHeader;
		FieldsContainer m_pageFooter;
		PaperOrientation m_Orientation;
		PaperType m_PaperType;

	};

}
#endif
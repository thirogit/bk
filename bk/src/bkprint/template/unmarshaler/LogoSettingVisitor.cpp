#include "stdafx.h"
#include "LogoSettingVisitor.h"
#include "../LogoField.h"
#include "../ExpandableTemplate.h"
#include "../FixedTemplate.h"

namespace bkprint
{
	LogoSettingVisitor::LogoSettingVisitor(const fipImage& img)
	{
		m_img = img;
	}

	void LogoSettingVisitor::Accept(LogoField* field)
	{
		field->SetImage(m_img);
	}

	void LogoSettingVisitor::Accept(ExpandableTemplate* aTemplate)
	{	
		VisitFields(aTemplate->HeaderFields());
		VisitFields(aTemplate->PageFooterFields());
		VisitFields(aTemplate->PageHeaderFields());
	}
	
	void LogoSettingVisitor::Accept(FixedTemplate* aTemplate)
	{
		int pageCount = aTemplate->GetPageCount();
		for (int page = 0; page < pageCount; page++)
		{
			VisitFields(*aTemplate->GetPage(page));
		}
	}

	void LogoSettingVisitor::VisitFields(FieldsContainer& container)
	{
		PtrEnumerator<Field> fieldEnum = container.EnumFields();
		while (fieldEnum.hasNext())
		{
			fieldEnum->Visit(this);
			fieldEnum.advance();
		}
	}

}

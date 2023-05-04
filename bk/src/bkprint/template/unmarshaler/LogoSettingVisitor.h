#ifndef __LOGOSETTINGVISITOR_H__
#define __LOGOSETTINGVISITOR_H__

#include "../NullVisitor.h"
#include "../TemplateVisitor.h"
#include <FreeImagePlus.h>
#include "../FieldsContainer.h"

namespace bkprint
{	
	class LogoSettingVisitor : public NullVisitor, public TemplateVisitor
	{
	public:	
		LogoSettingVisitor(const fipImage& img);
		virtual void Accept(LogoField* field);
		virtual void Accept(ExpandableTemplate* aTemplate);
		virtual void Accept(FixedTemplate* aTemplate);

	private:
		void VisitFields(FieldsContainer& container);
	private:
		fipImage m_img;
	};

}

#endif 

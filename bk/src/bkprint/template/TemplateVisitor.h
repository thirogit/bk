#pragma once

namespace bkprint
{
	class ExpandableTemplate;
	class FixedTemplate;

	class TemplateVisitor
	{
	public:
		virtual void Accept(ExpandableTemplate* aTemplate) = 0;
		virtual void Accept(FixedTemplate* aTemplate) = 0;
	};
}